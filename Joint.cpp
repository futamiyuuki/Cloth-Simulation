#include "Joint.h"

////////////////////////////////////////////////////////////////////////////////
//constructors
////////////////////////////////////////////////////////////////////////////////

Joint::Joint() {
    name = "";
    // Initialize constant data
    boxMin = Vector3::ORIGIN;
    boxMax = Vector3::ORIGIN;
    myOffset = Vector3::ORIGIN;
    // Resets variable data
    Reset();
}

Joint::Joint(string n) {
    name = n;
    // Initialize constant data
    boxMin = Vector3::ORIGIN;
    boxMax = Vector3::ORIGIN;
    myOffset = Vector3::ORIGIN;
    // Resets variable data
    Reset();
    
}


////////////////////////////////////////////////////////////////////////////////
//Load data from tokens
////////////////////////////////////////////////////////////////////////////////
bool Joint::Load(Tokenizer &token) {
    token.FindToken("{");
    while(1) {
        char temp[256];
        token.GetToken(temp);
        
        if(strcmp(temp,"offset")==0) {
            myOffset.x = token.GetFloat();
            myOffset.y = token.GetFloat();
            myOffset.z = token.GetFloat();
        }
        else // Check for other tokens
            if(strcmp(temp,"boxmin")==0){
                float x = token.GetFloat();
                float y = token.GetFloat();
                float z = token.GetFloat();
                boxMin.Set(x, y, z);
            }
            else if(strcmp(temp,"boxmax")==0){
                float x = token.GetFloat();
                float y = token.GetFloat();
                float z = token.GetFloat();
                boxMax.Set(x, y, z);
            }
            else if(strcmp(temp,"rotxlimit")==0){
                dofX.setRotLimitMin(token.GetFloat());
                dofX.setRotLimitMax(token.GetFloat());
            }
            else if(strcmp(temp,"rotylimit")==0){
                dofY.setRotLimitMin(token.GetFloat());
                dofY.setRotLimitMax(token.GetFloat());
            }
            else if(strcmp(temp,"rotzlimit")==0){
                dofZ.setRotLimitMin(token.GetFloat());
                dofZ.setRotLimitMax(token.GetFloat());
            }
            else if(strcmp(temp,"pose")==0){
                dofX.setPose(token.GetFloat());
                dofY.setPose(token.GetFloat());
                dofZ.setPose(token.GetFloat());
            }
            else if(strcmp(temp,"balljoint")==0) {
                token.GetToken(temp);
                Joint *jnt = new Joint(temp);
                jnt->Load(token);
                AddChild(jnt);
            }
            else if(strcmp(temp,"}")==0) return true;
        else token.SkipLine(); // Unrecognized token
    }
}

////////////////////////////////////////////////////////////////////////////////

void Joint::AddChild(Joint *j) {
    children.push_back(j);
}

Matrix34 & Joint::getMtx() {
    return LocalMtx;
}

void Joint::dfs(vector<Joint *> &out) {
    visited = true;
    out.push_back(this);
    for (Joint* child : children) {
        if (!child->visited) child->dfs(out);
    }
    
}

////////////////////////////////////////////////////////////////////////////////

void Joint::Update(Matrix34 mtx) {
    // Update (animate) any variable properties
    translate.MakeTranslate(myOffset);
    
    rotateX.MakeRotateX(dofX.getPose());
    rotateY.MakeRotateY(dofY.getPose());
    rotateZ.MakeRotateZ(dofZ.getPose());
    
    LocalMtx.Dot(mtx, translate);
    //LocalMtx.Dot(WorldMtx, translate);
    LocalMtx.Dot(LocalMtx, rotateZ);
    LocalMtx.Dot(LocalMtx, rotateY);
    LocalMtx.Dot(LocalMtx, rotateX);
    
    for (Joint* child : children) {
        child->Update(LocalMtx);
    }
}

////////////////////////////////////////////////////////////////////////////////

void Joint::Reset() {
    // Reset dynamic variables to a default state
    Angle=0.0f;
    visited = false;
    
    LocalMtx.Identity();
    rotateX.Identity();
    rotateY.Identity();
    rotateZ.Identity();
    translate.Identity();
    
    color[0] = float(rand() % 100 / 100.0f);
    color[1] = float(rand() % 100 / 200.0f);
    color[2] = float(rand() % 100 / 100.0f);
    color[3] = 1.0f;
    
    for (int i = 0; i < 4; i++) {
        prevColor[i] = color[i];
    }
}

////////////////////////////////////////////////////////////////////////////////

void Joint::Draw(Matrix34 mtx, bool mode) {
    glMatrixMode(GL_MODELVIEW);
    //mtx.Dot(mtx, LocalMtx);
    glLoadMatrixf(LocalMtx);

    glPushMatrix();
        glTranslatef(0.5f*(boxMin.x+boxMax.x),0.5f*(boxMin.y+boxMax.y),0.5f*(boxMin.z+boxMax.z));
        glScalef(boxMax.x-boxMin.x,boxMax.y-boxMin.y,boxMax.z-boxMin.z);
        glColor4f(color[0],color[1],color[2],color[3]);

        if (mode) glutWireCube(1.0f);
        else glutSolidCube(1.0f);
    glPopMatrix();

    for (auto& child : children) {
        child->Draw(mtx, mode);
    }
}
