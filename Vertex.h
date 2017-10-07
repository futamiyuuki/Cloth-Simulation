#ifndef __cse169__Vertex__
#define __cse169__Vertex__

#include "Vector3.h"

class Vertex {
public:
    
    Vertex();
    
    void Set(Vector3 &p,Vector3 &n,Vector3 &t);
    
    Vector3 getPosition();
    Vector3 getNormal();
    Vector3 getTexCoord();
    
    void setPosition(Vector3 p);
    void setNormal(Vector3 n);
    void setTexCoord(Vector3 t);
    
private:
    Vector3 Position;
    Vector3 Normal;
    Vector3 TexCoord;
};

#endif /* defined(__cse169__Vertex__) */
