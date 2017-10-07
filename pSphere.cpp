#include "pSphere.h"

pSphere::pSphere() {
    center.Zero();
    radius = 0.2;
}

pSphere::pSphere(Vector3 cen, float rad) {
    center = cen;
    radius = rad;
}

bool pSphere::TestSegment(const Segment &s, Intersection &i) {
    
    Vector3 v = s.B - center;
    float l = v.Mag2();
    
    //check if point inside the ball
    if (l < radius*radius) {
        Vector3 x = v.Normalize() * radius + center;
        i.Position.Set(x.x, x.y, x.z);
        i.Normal = v;
        return true;
    }
    
    return false;
}

Vector3 pSphere::getCenter() {
    return center;
}

void pSphere::setCenter(Vector3 c) {
    center = c;
}

float pSphere::getRad() {
    return radius;
}

void pSphere::setRadius(float r) {
    radius = r;
}