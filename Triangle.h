#ifndef __cse169__Triangle__
#define __cse169__Triangle__

#include <stdio.h>
#include "Vertex.h"

class Triangle {
public:
    Triangle();
    
    void Init(Vertex*,Vertex*,Vertex*);
    void Init(int,int,int);
    
    Vertex * GetVtx(int);
    
    Vector3 FindMaxBound();
    Vector3 FindMinBound();
    
    int getIdx(int);
    

private:
    Vertex * Vtx[3];
    int index[3];
    
};

#endif /* defined(__cse169__Triangle__) */
