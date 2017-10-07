#ifndef __cse169__Keyframe__
#define __cse169__Keyframe__

//data holder
class Keyframe {
    
public:
    
    Keyframe();

    float Time;
    float Value;
    float TangentIn,TangentOut;
    
    // Tangent rule
    char RuleIn;
    char RuleOut;
    float A,B,C,D; // Cubic coefficients
};


#endif /* defined(__cse169__Keyframe__) */
