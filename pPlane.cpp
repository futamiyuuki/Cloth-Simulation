#include "pPlane.h"

pPlane::pPlane() {
    point = Vector3::ORIGIN;
    normal = Vector3::YAXIS;
}

pPlane::pPlane(Vector3 p) {
    point = p;
    normal = Vector3::YAXIS;
    distance = normal.Dot(point);
}

bool pPlane::TestSegment(const Segment &s, Intersection &i) {
    Vector3 v = s.B - point;
    
    float l = v.Dot(normal);
    
    //behind the plane
    if (l < 0) {
        Vector3 dir = s.B - s.A;
        dir.Normalize();
        float dn = dir.Dot(normal);
        float t = (distance - s.A.Dot(normal)) / dn;
        Vector3 x = s.A + (t * dir);
        
        i.Position.Set(x.x, x.y, x.z);
        i.Normal = v;
        return true;
    }
    
    return false;
}

void pPlane::SetPoint(Vector3 &p) {
    point = p;
    distance = normal.Dot(point);
}

void pPlane::SetNormal(Vector3 &n) {
    normal = n;
    distance = normal.Dot(point);
}

Vector3 pPlane::GetNormal() {
    return normal;
}

float pPlane::getD() {
    return distance;
}

void pPlane::DrawWire() {
    glPushMatrix();
    glTranslatef(point.x, point.y, point.z);
    glBegin(GL_LINES);
    glVertex3f(len, -0.0, len);
    glVertex3f(len, -0.0, -len);
    
    glVertex3f(-len, 0.0, len);
    glVertex3f(-len, 0.0, -len);
    
    glVertex3f(-len, 0.0, len);
    glVertex3f(len, 0.0, len);
    
    glVertex3f(-len, 0.0, -len);
    glVertex3f(len, 0.0, -len);
    
    glEnd();
    glPopMatrix();
}

void pPlane::DrawSolid() {
    glPushMatrix();
    glTranslatef(point.x, point.y, point.z);
    glBegin(GL_QUADS);
    glNormal3f(normal.x, normal.y, normal.z);
    glColor3f(0.5f, 0.0f, 0.5f);
    glVertex3f(len, -0.1, len);
    glVertex3f(len, -0.1, -len);
    glVertex3f(-len, -0.1, -len);
    glVertex3f(-len, -0.1, len);
    glEnd();
    glPopMatrix();
}