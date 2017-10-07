#ifndef __cse169__AnimationPlayer__
#define __cse169__AnimationPlayer__

#include "AnimationClip.h"
#include "Skeleton.h"

class AnimationPlayer {
    
public:
    AnimationPlayer();
    
    void setClip(AnimationClip &clip);
    void setSkel(Skeleton &s);
    const vector<float> &GetPose();
    void Update();
    
    void Reset();
    void drawChannelCurve(int idx, int coord, int offsetX, int offsetY, float r, float g, float b);
    
    void toggleWalk();
    
private:
    float time;
    AnimationClip * anim;
    Skeleton * skel;
    vector<float> pose;
    bool walk;
};

#endif /* defined(__cse169__AnimationPlayer__) */
