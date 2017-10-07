#ifndef __cse169__Channel__
#define __cse169__Channel__

#include "Keyframe.h"
#include "token.h"
#include "Matrix34.h"

class Channel {
    
public:
    Channel();
    Channel(float start, float end);
    
    void preCompute();
    
    float Evaluate(float time);
    bool Load(Tokenizer &t);
    
    int binarySearch(float time, int min, int max);

    Keyframe getKeyframe(int i);
    int numKeys();
    
private:
    //for extrapolation
    float cyclic_offset;
    bool forwards;
    
    char extrap[2];
    float startTime;
    float endTime;
    
    vector<Keyframe> keyframes;
};

#endif /* defined(__cse169__Channel__) */
