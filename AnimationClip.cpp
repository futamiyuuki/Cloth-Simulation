#include "AnimationClip.h"

AnimationClip::AnimationClip() {
    startTime = 0;
    endTime = 0;
}

////////////////////////////////////////////////////////////////////////////////

void AnimationClip::Evaluate(float time, vector<float> & pose) {
    pose.clear();
    for (Channel* chan : channels) {
        float chanRes = chan->Evaluate(time);
        pose.push_back(chanRes);
    }
}

////////////////////////////////////////////////////////////////////////////////

bool AnimationClip::Load(const char *filename) {
    Tokenizer token;
    token.Open(filename);
    
    int numChannels;
    
    token.FindToken("range");
    startTime = token.GetFloat();
    endTime = token.GetFloat();
    
    token.FindToken("numchannels");
    numChannels = token.GetInt();
    
    for (int i = 0; i < numChannels; i++) {
        Channel * newChannel = new Channel(startTime, endTime);

        token.FindToken("channel");
        newChannel->Load(token);
        channels.push_back(newChannel);
    }
    token.Close();
    return true;
}


////////////////////////////////////////////////////////////////////////////////

void AnimationClip::Draw (int idx) {
} 