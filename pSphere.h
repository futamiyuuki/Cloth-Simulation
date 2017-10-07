#ifndef __cse169__pSphere__
#define __cse169__pSphere__

#include "Primitive.h"

class pSphere : public Primitive {
    
public:
    pSphere();
    pSphere(Vector3 cen, float rad);
    
    bool TestSegment(const Segment &s, Intersection &i);
    
    Vector3 getCenter();
    void setCenter(Vector3 c);
    
    float getRad();
    void setRadius(float r);
    
private:
    Vector3 center;
    float radius;
};

#endif /* defined(__cse169__pSphere__) */
