#include "pTriangle.h"

pTriangle::pTriangle() {
    
}

bool pTriangle::TestSegment(const Segment &s, Intersection &i) {
    Vector3 v0 = p1->getPosition();
    Vector3 v1 = p2->getPosition();
    Vector3 v2 = p3->getPosition();
    
    float da = (s.A - v0).Dot(normal);
    float db = (s.B - v0).Dot(normal);
    
    //on same side of triangle
    if (da > 0 && db > 0) return false;
    //inside, place on surface
    if (da < 0 && db < 0) return false;
    
    //avoid divide by 0
    if ((da - db) == 0) return false;
    Vector3 x = ((da*s.B)-(db*s.B)) / (da - db);
    
    //convert to barycentric coordinates
    Vector3 xPrime = x - v0;
    Vector3 e1 = v1 - v0;
    Vector3 e2 = v2 - v0;
    
    float denom = e1.Dot(e2);
    //avoid divide by 0
    if (denom == 0) return false;
    
    float alpha = (xPrime.Dot(e2)) / denom;
    float beta = (xPrime.Dot(e1)) / denom;
    
    if (alpha < 0 || beta < 0 || (alpha+beta) > 1) return false;
    
    Vector3 temp = s.B - v0;
    float d = temp.Dot(normal);
    if (d < 0) {
        //point is behind the triangle face
        d = -d;
        x = x * (1- d/x.Mag());
    }
    
    i.Position = x;
    i.Normal = normal;
    
    return true;
}

bool pTriangle::TestTriangle(pTriangle &t, Intersection &i) {
    Vector3 v0 = p1->getPosition();
    Vector3 v1 = p2->getPosition();
    Vector3 v2 = p3->getPosition();
    
    Vector3 n2 = v1 - v0;
    Vector3 cross = v2 - v0;
    n2.Cross(n2, cross);
    float d2 = -n2.Dot(v0);
    
    float di[3];
    for (int i = 0; i < 3; i++) {
        di[i] = n2.Dot(t.getVertex(i)->getPosition()) + d2;
    }
    
    if (di[0] > 0 && di[1] > 0 && di[2] > 0) return false;
    if (di[0] < 0 && di[1] < 0 && di[2] < 0) return false;
    
    return false;
}

void pTriangle::setVertex(Particle * v1, Particle * v2, Particle * v3) {
    p1 = v1;
    p2 = v2;
    p3 = v3;
}

Particle * pTriangle::getVertex(int idx) {
    if (idx == 0) return p1;
    if (idx == 1) return p2;
    return p3;
}

void pTriangle::initNormal() {
    //find triangle unnormalized normal, n*
    Vector3 ra = (p2->getPosition() - p1->getPosition());
    Vector3 rb = (p3->getPosition() - p1->getPosition());

    normal.Cross(ra, rb);
    normal.Normalize();
}

Vector3 pTriangle::getNormal() {
    return normal;
}

Vector3 pTriangle::getVelocity() {
    return velocity;
}

void pTriangle::ComputeForce(Vector3 vair) {
    //find the tirangle velocity
    velocity = (p1->getVelocity() + p2->getVelocity() + p3->getVelocity()) / 3 ;
    //assume no air velocity for now
    Vector3 v = velocity - vair;
    
    Vector3 r1 = p1->getPosition();
    Vector3 r2 = p2->getPosition();
    Vector3 r3 = p3->getPosition();
    //find triangle unnormalized normal, n*
    Vector3 ra = (r2 - r1);
    Vector3 rb = (r3 - r1);
    Vector3 ncross;
    ncross.Cross(ra, rb);
    
    //constants
    float p = 1.225f;
    float cd = 1.225f;
    
    //force = -1/2 * p * |v|^2 * cd * a * n
    Vector3 v2an = ( (v.Mag()*v.Dot(ncross))/(2.0f*ncross.Mag()) ) * ncross;
    Vector3 aeroForce = (-1.0f/2.0f) * p * cd * v2an;
    aeroForce = aeroForce / 3;
    p1->ApplyForce(aeroForce);
    p2->ApplyForce(aeroForce);
    p3->ApplyForce(aeroForce);
    normal = ncross.Normalize();
}