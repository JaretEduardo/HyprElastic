#include "wobblywindow.h"

#include "globals.h"
#include "renderpasses.h"

CWobblyWindow::CWobblyWindow() {
    auto&& verts = CRenderWobblyWindowPassElement::s_baseVerts;

    // copy directly
    m_particlePositions = verts;

    // init empty
    m_particleVelocities.resize(verts.size() / 2);

    m_targetPositions.reserve(verts.size() / 2);
    for (unsigned int i {}; i < verts.size(); i += 2) {
        m_targetPositions.push_back(Vector2D {verts[i], verts[i + 1]});
    }
}

bool CWobblyWindow::step(Time::steady_tp time) {
    const auto usec =
        std::chrono::duration_cast<std::chrono::microseconds>(time - m_lastTime).count();
    const float dt = std::min(0.016f, usec * 0.000001f);
    m_lastTime = time;

    Vector2D totalVel {};

    for (unsigned int i {}; i < m_particlePositions.size() / 2; i++) {
        // TODO: avoid float -> double promotion
        const auto& pos = Vector2D {m_particlePositions[i * 2], m_particlePositions[i * 2 + 1]};
        auto&& vel = m_particleVelocities[i];
        auto&& targetPosition = m_targetPositions[i];

        const Vector2D directionToTarget = targetPosition - pos;

        if (m_grabPosition.has_value()) {
            const float distanceToDragPosition = m_grabPosition.value().distance(pos);
            const float dragStrength = std::clamp(distanceToDragPosition, 0.f, 1.f);

            m_particlePositions[i * 2] += m_windowMovement.x * dragStrength;
            m_particlePositions[i * 2 + 1] += m_windowMovement.y * dragStrength;
        }

        const Vector2D springForce = directionToTarget * s_springStrength - vel * s_dampingStrength;
        vel += springForce * dt;

        // Apply drag
        const Vector2D dragForceVector = (vel * vel) * -s_drag;

        // * Area
        auto dir = vel;
        float magnitude = dir.normalize();

        if (magnitude != 0)
            vel += (dragForceVector * dir) * dt;

        // Apply velocity
        m_particlePositions[i * 2] += vel.x * dt;
        m_particlePositions[i * 2 + 1] += vel.y * dt;

        // Keep vertices near their targets to avoid mesh inversions.
        Vector2D clampedPos = {m_particlePositions[i * 2], m_particlePositions[i * 2 + 1]};
        Vector2D delta = clampedPos - targetPosition;
        const float deltaSize = delta.size();
        if (deltaSize > s_maxDisplacement && deltaSize != 0) {
            delta = delta * (s_maxDisplacement / deltaSize);
            clampedPos = targetPosition + delta;
            m_particlePositions[i * 2] = clampedPos.x;
            m_particlePositions[i * 2 + 1] = clampedPos.y;
        }

        totalVel += vel;
    }

    const bool shouldEnd = m_windowMovement.size() == 0
        and totalVel.size() / (m_particlePositions.size() / 2.f) < .001f;

    if (shouldEnd)
        ++m_settledFrames;
    else
        m_settledFrames = 0;

    // HACK: why is it ever nan??
    if (std::isnan(m_particlePositions[0])) {
        // reset
        m_particlePositions = CRenderWobblyWindowPassElement::s_baseVerts;

        m_particleVelocities.clear();
        m_particleVelocities.resize(m_particlePositions.size() / 2);
    }
    //
    // std::println(
    //     "Top left: ({}, {}), totalVel: {}",
    //     m_particlePositions[0],
    //     m_particlePositions[1],
    //     totalVel.size() / m_particlePositions.size() * 2
    // );
    m_windowMovement = Vector2D {};

    // Require a few consecutive settled frames to avoid visible on/off flicker.
    return m_settledFrames > 12;
}

void CWobblyWindow::applyMovement(const Vector2D& movement) {
    const Vector2D amplifiedMovement = movement * s_movementAmplification;
    m_windowMovement = amplifiedMovement;
    m_settledFrames = 0;

    constexpr Vector2D kCenter = {0.5, 0.5};

    for (unsigned int i = 0; i < m_particlePositions.size() / 2; i++) {
        const Vector2D pos = {m_particlePositions[i * 2], m_particlePositions[i * 2 + 1]};
        const Vector2D target = m_targetPositions[i];

        float influence = 0.45f;
        if (m_grabPosition.has_value()) {
            const float distance = m_grabPosition.value().distance(pos);
            const float t = std::clamp(1.f - distance * 1.2f, 0.f, 1.f);
            influence = 0.15f + 0.85f * (t * t * (3.f - 2.f * t));
        } else {
            const float centerDistance = kCenter.distance(pos);
            const float t = std::clamp(1.f - centerDistance * 1.4f, 0.f, 1.f);
            influence = 0.25f + 0.35f * t;
        }

        Vector2D newPos = pos - amplifiedMovement * influence;
        Vector2D delta = newPos - target;
        float d = delta.size();
        if (d > s_maxDisplacement && d != 0) {
            delta = delta * (s_maxDisplacement / d);
            newPos = target + delta;
        }

        m_particlePositions[i * 2] = newPos.x;
        m_particlePositions[i * 2 + 1] = newPos.y;
        m_particleVelocities[i] -= amplifiedMovement * (s_velocityImpulse * influence);
    }
}

Hyprutils::Math::CBox CWobblyWindow::getBox() const {
    Vector2D min {};
    Vector2D max {};

    for (unsigned int i = 0; i < m_particlePositions.size() / 2; i++) {
        auto&& x = m_particlePositions[i * 2];
        auto&& y = m_particlePositions[i * 2 + 1];

        if (x > max.x)
            max.x = x;
        if (x < min.x)
            min.x = x;

        if (y > max.y)
            max.y = y;
        if (y < min.y)
            min.y = y;
    }

    return CBox(min, max - min);
}
