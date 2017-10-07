#ifndef __cse169__pPlane__
#define __cse169__pPlane__

#include "Primitive.h"

class pPlane : public Primitive {
public:
    pPlane();
    pPlane(Vector3 p);
    bool TestSegment(const Segment &s, Intersection &i);
    
    void SetPoint(Vector3&);
    void SetNormal(Vector3&);
    
    void DrawWire();
    void DrawSolid();
    
    Vector3 GetNormal();
    float getD();
    
private:
    Vector3 point;
    Vector3 normal;
    float distance;
    
    float len = 2;
};

#endif /* defined(__cse169__pPlane__) */
