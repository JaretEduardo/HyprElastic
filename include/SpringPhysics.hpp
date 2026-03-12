#pragma once

class SpringPhysics {
private:
    float stiffness;
    float damping;
    float position;
    float velocity;
    float target;

public:
    SpringPhysics(float k, float c);

    void update(float deltaTime);
    void setTarget(float newTarget);
    float getPosition();
};