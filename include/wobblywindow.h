#ifndef WW_WOBBLYWINDOW_H
#define WW_WOBBLYWINDOW_H

#include <hyprland/src/Compositor.hpp>

class CWobblyWindow {
    struct SParticle {
        Vector2D position;
        Vector2D velocity;
    };

    Time::steady_tp m_lastTime {Time::steadyNow()};

  public:
    // static wobble parameters
    static inline float s_springStrength = 500.f;
    static inline float s_dampingStrength = 12.f;
    static inline float s_drag = 0.4f;

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
