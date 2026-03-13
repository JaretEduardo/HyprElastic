#ifndef WW_WOBBLYWINDOW_H
#define WW_WOBBLYWINDOW_H

#include <hyprland/src/Compositor.hpp>

class CWobblyWindow {
    struct SParticle {
        Vector2D position;
        Vector2D velocity;
    };

    Time::steady_tp m_lastTime {Time::steadyNow()};
    int m_settledFrames = 0;

  public:
    // static wobble parameters
    static inline float s_springStrength = 620.f;
    static inline float s_dampingStrength = 20.f;
    static inline float s_drag = 0.42f;
    static inline float s_movementAmplification = 0.42f;
    static inline float s_velocityImpulse = 0.58f;
    static inline float s_maxDisplacement = 0.050f;

    std::vector<float> m_particlePositions;
    std::vector<Vector2D> m_particleVelocities;

    std::vector<Vector2D> m_targetPositions;
    std::optional<Vector2D> m_grabPosition {std::nullopt};
    Vector2D m_windowMovement;

    CWobblyWindow();

    bool step(Time::steady_tp time);
    CBox getBox() const;

    void applyMovement(const Vector2D& movement);
};
#endif
