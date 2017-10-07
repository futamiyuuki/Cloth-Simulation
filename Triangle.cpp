#include "Triangle.h"

Triangle::Triangle() {
    //default constructor
}

void Triangle::Init(Vertex * v0, Vertex * v1, Vertex * v2) {
    Vtx[0] = v0;  //a
    Vtx[1] = v1;  //b
    Vtx[2] = v2;  //c
}

void Triangle::Init(int x, int y, int z) {
    index[0] = x;
    index[1] = y;
    index[2] = z;
}


Vertex * Triangle::GetVtx(int idx) {
    return Vtx[idx];
}

int Triangle::getIdx(int i) {
    return index[i];
}