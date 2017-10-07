#include "Vertex.h"

Vertex::Vertex() {
    Position.Zero();
    Normal.Zero();
    TexCoord.Zero();
}

void Vertex::Set(Vector3 &p,Vector3 &n,Vector3 &t) {
    Position.Set(p.x, p.y, p.z);
    Normal.Set(n.x, n.y, n.z);
    TexCoord.Set(t.x, t.y, t.z);
}

Vector3 Vertex::getPosition() {
    return Position;
}

Vector3 Vertex::getNormal() {
    return Normal;
}

Vector3 Vertex::getTexCoord() {
    return TexCoord;
}


void Vertex::setPosition(Vector3 p) {
    Position.Set(p.x, p.y, p.z);
}

void Vertex::setNormal(Vector3 n) {
    Normal.Set(n.x, n.y, n.z);
}

void Vertex::setTexCoord(Vector3 t) {
    TexCoord.Set(t.x, t.y, t.z);
}