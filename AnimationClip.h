#ifndef __cse169__AnimationClip__
#define __cse169__AnimationClip__

#include "DOF.h"
#include "Channel.h"

class AnimationClip {
    
public:
    AnimationClip();
    
    void Evaluate(float time, vector<float> & pose);
    void Draw(int idx);
    bool Load(const char *filename);

//private:
    float startTime;
    float endTime;
    vector<Channel*> channels;
};

#endif /* defined(__cse169__AnimationClip__) */
