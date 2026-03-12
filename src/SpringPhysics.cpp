#include "SpringPhysics.hpp"

SpringPhysics::SpringPhysics(float k, float c) : stiffness(k), damping(c), position(0), velocity(0), target(0) {}

void SpringPhysics::update(float deltaTime) {
    float displacement = position - target;

    float force = -stiffness * displacement - damping * velocity;

    velocity += force * deltaTime;
    position += velocity * deltaTime;
}

void SpringPhysics::setTarget(float newTarget) {
    target = newTarget;
}

float SpringPhysics::getPosition() {
    return position;
}