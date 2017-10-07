#include "Skeleton.h"

////////////////////////////////////////////////////////////////////////////////
//constructors
////////////////////////////////////////////////////////////////////////////////

Skeleton::Skeleton() {
    Root = new Joint();
}

Skeleton::Skeleton(const char *filename) {
    // Resets variable data
    Reset();
    Load(filename);
}

////////////////////////////////////////////////////////////////////////////////
// load .skel file
////////////////////////////////////////////////////////////////////////////////

bool Skeleton::Load(const char *file) {
    
    Tokenizer token;
    token.Open(file);
    token.FindToken("balljoint");
    
    char temp[256];
    token.GetToken(temp);
    
    // Parse tree
    Root = new Joint(temp);
    Root->Load(token);
    
    joints.clear();
    Root->dfs(joints);

    // Finish
    token.Close();

    return true;
}


////////////////////////////////////////////////////////////////////////////////

void Skeleton::Update() {
    LocalMtx.MakeTranslate(poseX, poseY, poseZ);

    rotateX.MakeRotateX(rotX);
    rotateY.MakeRotateY(rotY);
    rotateZ.MakeRotateZ(rotZ);
    
    LocalMtx.Dot(LocalMtx, rotateZ);
    LocalMtx.Dot(LocalMtx, rotateY);
    LocalMtx.Dot(LocalMtx, rotateX);
    
    
    // Update (animate) any variable properties
    Root->Update(LocalMtx);
    
}

////////////////////////////////////////////////////////////////////////////////

void Skeleton::Reset() {
    // Reset dynamic variables to a default state
    LocalMtx.Identity();
    
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    
    poseX = 0;
    poseY = 0;
    poseZ = 0;
    
    rotateX.Identity();
    rotateY.Identity();
    rotateZ.Identity();
}

////////////////////////////////////////////////////////////////////////////////

void Skeleton::Draw(bool mode) {
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(LocalMtx);
    Root->Draw(LocalMtx, mode);
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////

Matrix34 Skeleton::getWorldMatrix(int i) {
    Matrix34 temp;
    temp.Identity();
    if (joints.empty()) return temp;
    return joints[i]->getMtx();
}
