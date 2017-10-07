#ifndef __cse169__Skeleton__
#define __cse169__Skeleton__

#include "Joint.h"

class Skeleton {

public:
    Skeleton();
    Skeleton(const char *filename);
    
    bool Load(const char *file);
    
    Matrix34 getWorldMatrix(int);
    
    void Update();
    void Reset();
    void Draw(bool mode);
    
    float poseX;
    float poseY;
    float poseZ;
    float rotX;
    float rotY;
    float rotZ;
    
    vector<Joint*> joints;
    
private:
    Joint * Root;
    
    // Variables
    Matrix34 LocalMtx;
    Matrix34 rotateX;
    Matrix34 rotateY;
    Matrix34 rotateZ;
};


#endif /* defined(__cse169__Skeleton__) */
