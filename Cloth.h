#ifndef __cse169__Cloth__
#define __cse169__Cloth__

#include "SpringDamper.h"
#include "pTriangle.h"
#include "pSphere.h"
#include "pPlane.h"

//change size of cloth here
static const int clothSize = 15;
static const float ratio = 0.05;
static const int largeSpring = 400;
static const int largeDamp = 12;

class Cloth {
    
public:
    Cloth();
    void Draw(bool mode);
    void Update(Vector3 vair);
    void Reset();
    
    float xMin();
    float xMax();
    float yMin();
    float yMax();
    
    void drag(float x, float y, float z);
    
private:
    Particle * particles[clothSize][clothSize];
    float deltaTime;
    vector<SpringDamper*> springs;
    vector<SpringDamper*> bendingSprings;

    vector<pTriangle> triangles;
    
    pSphere ball;
    pPlane ground;
    

};

#endif /* defined(__cse169__Cloth__) */
