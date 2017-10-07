#ifndef __cse169__SpringDamper__
#define __cse169__SpringDamper__

#include "Particle.h"

static const int SPRING_CONST = 1000;
static const int DAMP_CONST = 14;

class SpringDamper {
public:
    SpringDamper();
    SpringDamper(Particle * p1, Particle * p2);
    SpringDamper(Particle * p1, Particle * p2, float length);
    SpringDamper(Particle * p1, Particle * p2, float length, int damp, int spring);
    
    void ComputeForce();
    
    Particle *P1,*P2;
    
private:
    float SpringConstant,DampingFactor;
    float RestLength;

};


#endif /* defined(__cse169__SpringDamper__) */
