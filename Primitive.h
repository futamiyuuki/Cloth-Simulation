#ifndef cse169_Primitive_h
#define cse169_Primitive_h

#include "Segment.h"
#include "Intersection.h"
#include "Vector3.h"

class Primitive {
    virtual bool TestSegment(const Segment &s, Intersection &i)=0;
};

#endif
