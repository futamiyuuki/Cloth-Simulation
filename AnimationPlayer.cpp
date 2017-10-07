#include "AnimationPlayer.h"

AnimationPlayer::AnimationPlayer() {
    anim = new AnimationClip();
    skel = new Skeleton();
    time = 0;
}

////////////////////////////////////////////////////////////////////////////////

void AnimationPlayer::setClip(AnimationClip &clip) {
    anim = &clip;
}

void AnimationPlayer::setSkel(Skeleton &s) {
    walk = true;
    skel = &s;
}

////////////////////////////////////////////////////////////////////////////////

const vector<float> & AnimationPlayer::GetPose() {
    return this->pose;
}

void AnimationPlayer::toggleWalk() {
    walk = !walk;
}

////////////////////////////////////////////////////////////////////////////////

void AnimationPlayer::Update() {
    if (time > 13) time = -20;
    time = time + 0.01;
    anim->Evaluate(time, pose);
    
    //translate root
    if (walk) skel->joints[0]->myOffset.Set(pose[0], pose[1], pose[2]);
    else skel->joints[0]->myOffset.Set(0, 0, 0);
    
    //rotate each joint
    for (int i = 1; i < pose.size()/3; i++) {
        skel->joints[i-1]->dofX.setPose(pose[i*3]);
        skel->joints[i-1]->dofY.setPose(pose[(i*3)+1]);
        skel->joints[i-1]->dofZ.setPose(pose[(i*3)+2]);
    }
}

////////////////////////////////////////////////////////////////////////////////

void AnimationPlayer::Reset() {
    time = 0;
}

////////////////////////////////////////////////////////////////////////////////
void AnimationPlayer::drawChannelCurve(int idx, int coord, int offsetX, int offsetY, float r, float g, float b) {

    glBegin(GL_LINE_STRIP);
    glColor3f(r, g, b);
    float i = anim->startTime;
    while (i <= anim->endTime) {
        float dof = anim->channels[idx*3+coord]->Evaluate(i);
        glVertex2f(offsetX+(i*100), offsetY+(dof*100));
        i = i + 0.01f;
    }
    
    glEnd();
    
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glColor3f(r, g, b);
    for (int i = 0; i < anim->channels[idx*3+coord]->numKeys(); i++) {
        glVertex2f(offsetX + (anim->channels[idx*3+coord]->getKeyframe(i).Time*100),
                   offsetY+ anim->channels[idx*3+coord]->getKeyframe(i).Value*100 );
    }
    glEnd();
    
}
////////////////////////////////////////////////////////////////////////////////