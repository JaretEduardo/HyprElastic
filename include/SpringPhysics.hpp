#pragma once

class SpringPhysics {
private:
    float stiffness = 0.0f;
    float damping   = 0.0f;
    float position  = 0.0f;
    float velocity  = 0.0f;
    float target    = 0.0f;

public:
    SpringPhysics() = default; 
    
    SpringPhysics(float k, float c);

    void update(float deltaTime);
    void setTarget(float newTarget);
    float getPosition();
};