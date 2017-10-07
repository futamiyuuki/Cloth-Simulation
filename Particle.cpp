#include "Particle.h"

////////////////////////////////////////////////////////////////////////////////
// constructors
////////////////////////////////////////////////////////////////////////////////

Particle::Particle() {
    Mass = 1.0f;
    Position.Zero();
    Velocity.Zero();
    Force.Zero();
    fixed = false;
}

////////////////////////////////////////////////////////////////////////////////
// update, draw and reset functions
////////////////////////////////////////////////////////////////////////////////

void Particle::Particle::Update(float deltaTime) {
    //particle is unmovable, fixed position
    if (fixed) return;
    
    // Compute acceleration (Newton’s second law)
    Vector3 Accel=(1.0/Mass) * Force;
    
    // Compute new position & velocity
    Velocity += Accel * deltaTime;
    Position += Velocity * deltaTime;
    
    // Zero out Force vector
    Force.Zero();
}

void Particle::Draw() {
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex3f(Position.x, Position.y, Position.z);
    glEnd();
}

void Particle::Reset() {
    Force.Zero();
    Velocity.Zero();
}

////////////////////////////////////////////////////////////////////////////////
// other
////////////////////////////////////////////////////////////////////////////////

void Particle::ApplyForce(Vector3 &f) {
    Force = Force + f;
}

void Particle::updatePos(float x, float y) {
    Position.x = Position.x + x;
    Position.y = Position.y + y;
}

void Particle::updatePrevious(Vector3 prev) {
    prevPos = prev;
}

Vector3& Particle::getPrevPosition() {
    return prevPos;
}

////////////////////////////////////////////////////////////////////////////////
// setters
////////////////////////////////////////////////////////////////////////////////

void Particle::setVelocity(float x, float y, float z) {
    Velocity.Set(x, y, z);
}

void Particle::setPosition(float x, float y, float z) {
    Position.Set(x, y, z);
}

void Particle::setFixed(bool f) {
    fixed = f;
}

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////

Vector3 Particle::getVelocity() {
    return Velocity;
}

Vector3& Particle::getPosition() {
    return Position;
}

float Particle::getMass() {
    return Mass;
}