#ifndef __cse169__DOF__
#define __cse169__DOF__

#include "Vector3.h"

class DOF {
    
public:
    
    DOF();
        
    void Initialize();
    
    float getRotLimitMin();
    float getRotLimitMax();
    float getPose();
    
    void setRotLimitMin(float m);
    void setRotLimitMax(float m);
    void setPose(float pos);
    
private:
    float min;
    float max;
    
    float pose;
};

#endif /* defined(__cse169__DOF__) */
