#ifndef __cse169__Joint__
#define __cse169__Joint__

#include "token.h"
#include "Matrix34.h"
#include "DOF.h"

class Joint {

public:
    
    Joint();
    Joint(string);
    Joint(Matrix34 p);

    void Update(Matrix34);
    void Reset();
    void Draw(Matrix34 mtx, bool mode);
    
    bool Load(Tokenizer &t);
    void AddChild(Joint *j);
    
    void dfs( vector<Joint*>& out);
    
    Matrix34 & getMtx();
    
    DOF dofX;
    DOF dofY;
    DOF dofZ;
    
    string name;
    
    Vector3 myOffset;
    
    float color [4];
    float prevColor [4];
    
    // Constants
    Vector3 boxMin;
    Vector3 boxMax;
    
    
    
private:
    // Variables
    float Angle;
    
    Matrix34 LocalMtx;
    Matrix34 translate;
    Matrix34 rotateX;
    Matrix34 rotateY;
    Matrix34 rotateZ;
    
protected:
    vector<Joint*> children;
    bool visited;

    
};



#endif /* defined(__cse169__Joint__) */
