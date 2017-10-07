#include "Channel.h"

Channel::Channel() {
    startTime = 0;
    endTime = 0;
    cyclic_offset = 0;
    forwards = true;
}

Channel::Channel(float start, float end) {
    startTime = start;
    endTime = end;
}

Keyframe Channel::getKeyframe(int i) {
    return keyframes[i];
}

int Channel::numKeys() {
    return keyframes.size();
}

////////////////////////////////////////////////////////////////////////////////

void Channel::preCompute() {
    //base case for single keyframe
    if (keyframes.size() == 1) {
        if (keyframes[0].RuleIn == 'f') return;
        keyframes[0].TangentIn = keyframes[0].Value;
        keyframes[0].TangentOut = keyframes[0].Value;
        return;
    }
    
    //set tangents for non-fixed values
    for (int i = 1; i < keyframes.size(); i++) {
        //default value, if not already fixed
        if (keyframes[i].RuleIn != 'f') {
            keyframes[i].TangentIn = keyframes[i].Value;
            keyframes[i].TangentOut = keyframes[i].Value;
        }
        Keyframe k1 = keyframes[i];
        Keyframe k0 = keyframes[i-1];
        
        if (keyframes[i].RuleIn == 'l') {
            //linear tangent  (p1 - p0) / (t1 - t0)
            keyframes[i-1].TangentOut = (k1.Value - k0.Value)/(k1.Time - k0.Time);
            keyframes[i].TangentIn = keyframes[i-1].TangentOut;
            
        } else if (keyframes[i].RuleIn == 's') {
            if (i == keyframes.size()-1) {
                //use linear rule
                keyframes[i-1].TangentOut = (k1.Value - k0.Value)/(k1.Time - k0.Time);
                keyframes[i].TangentIn = keyframes[i-1].TangentOut;
                
            } else {
                //smooth tangent (p2 - p0) / (t2 - t0)
                Keyframe k2 = keyframes[i+1];
                keyframes[i].TangentOut = (k2.Value - k0.Value)/(k2.Time - k0.Time);
                keyframes[i].TangentIn = keyframes[i].TangentOut;
            }
        }
    }
    
    //calculate cubic co-efficents
    for (int i = 0; i < keyframes.size()-1; i++) {
        float p0 = keyframes[i].Value;
        float p1 = keyframes[i+1].Value;
        float v0 =(keyframes[i+1].Time - keyframes[i].Time) * keyframes[i].TangentOut;
        float v1 = (keyframes[i+1].Time - keyframes[i].Time) * keyframes[i+1].TangentIn;
        keyframes[i].A = (2*p0) + (-2*p1) + v0 + v1;
        keyframes[i].B = (-3*p0) + (3*p1) + (-2*v0) + (-v1);
        keyframes[i].C = v0;
        keyframes[i].D = p0;
    }
}
////////////////////////////////////////////////////////////////////////////////

float Channel::Evaluate(float time) {
    //no keys in channel
    if (keyframes.size() == 0) return 0;
    
    //reset offset each eval
    cyclic_offset = 0;
    
    //cout << "current: " << time << endl;
    
    int cycles = 0;
    if (time > endTime) {
        //constant and linear cases, no cycling
        if (extrap[1] == 'k') return keyframes[keyframes.size()-1].Value;
        else if (extrap[1] == 'l') return keyframes[keyframes.size()-1].TangentOut;
        
        //cycle through, just mod time
        else if (extrap[1] == 'c') {
            cycles = floor ((time - startTime) / (endTime - startTime)) -1;
            time = time - endTime + startTime - (cycles * (endTime - startTime));
            
        //cycle through with offset
        } else if (extrap[1] == 'o') {
            //offset multiplied by number of cycles passed
            cycles = floor ((time - startTime) / (endTime - startTime)) -1;
            
            cyclic_offset = keyframes[keyframes.size()-1].Value * float((cycles+1)*2);
            time = time - endTime + startTime - (cycles * (endTime - startTime));
            
        //repeat alternating backwards and forwards
        } else if (extrap[1] == 'b') {
            cycles = floor ((time - startTime) / (endTime - startTime));
            time = time - endTime + startTime - (cycles * (endTime - startTime));
            
            //if on odd cycle go backwards
            if ( cycles % 2 != 0) time = endTime - (time - startTime);
        }
    } else if (time < startTime) {
        //constant and linear cases, no cycling
        if (extrap[0] == 'k') return keyframes[0].Value;
        else if (extrap[0] == 'l') return keyframes[0].TangentIn;
        
        //cycle through, just mod time
        else if (extrap[0] == 'c') {
            cycles = floor ((time - startTime) / (endTime - startTime)) -1;
            time = time - endTime + startTime - (cycles * (endTime - startTime));
            
        //cycle through with offset
        } else if (extrap[0] == 'o') {
            //offset multiplied by number of cycles passed
            cycles = floor ((time - startTime) / (endTime - startTime)) -1;

            cyclic_offset = keyframes[keyframes.size()-1].Value * float((cycles+1)*2);
            time = time - endTime + startTime - (cycles * (endTime - startTime));
            
        //repeat alternating backwards and forwards
        } else if (extrap[0] == 'b') {
            cycles = floor ((time - startTime) / (endTime - startTime));
            time = time - endTime + startTime - (cycles * (endTime - startTime));

            //if on odd cycle go backwards
            if ( cycles % 2 != 0) time = endTime - (time - startTime);
        }
        
    }
    //just one key, return key value
    if (keyframes.size() == 1) return keyframes[0].Value + cyclic_offset;
    //optimize with binary search
    int idx = binarySearch(time, 0, keyframes.size()-1);
    //change time to 0..1 span
    float u = (time - keyframes[idx].Time) / (keyframes[idx+1].Time - keyframes[idx].Time);
    //equal to keyframe value
    if (time == keyframes[idx].Time) return keyframes[idx].Value + cyclic_offset;
    //evaluate span
    return cyclic_offset + (keyframes[idx].A * u*u*u) + (keyframes[idx].B * u*u) +
    (keyframes[idx].C*u) + keyframes[idx].D;
}

int Channel::binarySearch(float time, int min, int max) {
    if (max == min) return min;
    if (max-min == 1) return min;
    
    int mid = (min + max) / 2;
    
    if (keyframes[mid].Time < time) {
        return binarySearch(time, mid, max);
    } else if (keyframes[mid].Time > time) {
        return binarySearch(time, min, mid);
    } else {
        return mid;
    }
    
}

////////////////////////////////////////////////////////////////////////////////

bool Channel::Load(Tokenizer &token) {
    char temp[256];
    token.FindToken("extrapolate");
    
    for (int j = 0; j < 2; j++) {
        token.GetToken(temp);
        if (strcmp(temp, "constant") == 0) {
            extrap[j] = 'k';
        } else if (strcmp(temp, "linear") == 0) {
            extrap[j] = 'l';
        } else if (strcmp(temp, "cycle") == 0) {
            extrap[j] = 'c';
        } else if (strcmp(temp, "cycle_offset") == 0) {
            extrap[j] = 'o';
        } else if (strcmp(temp, "bounce") == 0) {
            extrap[j] = 'b';
        }
    }
    
    token.FindToken("keys");
    int numKeys = token.GetInt();
    token.FindToken("{");
    for (int i = 0; i < numKeys; i++) {
        Keyframe newKeyframe = Keyframe();
        newKeyframe.Time = token.GetFloat();
        newKeyframe.Value = token.GetFloat();
        
        token.GetToken(temp);
        if (strcmp(temp, "flat") == 0) {
            newKeyframe.RuleIn = 'f';
            newKeyframe.TangentIn = 0;
        } else if (strcmp(temp, "linear") == 0) {
            newKeyframe.RuleIn = 'l';
        } else if (strcmp(temp, "smooth") == 0) {
            newKeyframe.RuleIn = 's';
        } else {
            newKeyframe.RuleIn = 'f';
            newKeyframe.RuleIn = atof(temp);
        }
        
        token.GetToken(temp);
        if (strcmp(temp, "flat") == 0) {
            newKeyframe.RuleOut = 'f';
            newKeyframe.TangentOut = 0;
        } else if (strcmp(temp, "linear") == 0) {
            newKeyframe.RuleOut = 'l';
        } else if (strcmp(temp, "smooth") == 0) {
            newKeyframe.RuleOut = 's';
        } else {
            newKeyframe.RuleOut = 'f';
            newKeyframe.TangentOut = atof(temp);
        }
        keyframes.push_back(newKeyframe);
    }
    
    startTime = keyframes[0].Time;
    endTime = keyframes[keyframes.size()-1].Time;
    
    preCompute();
    return true;

}