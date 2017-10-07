#ifndef __cse169__Skin__
#define __cse169__Skin__

#include "Vertex.h"
#include "Triangle.h"
#include "skinWeight.h"
#include "Skeleton.h"
#include "BMPImage.h"

class Skin {
    
public:
    Skin();
    Skin(Skeleton * s);
    Skin(Skeleton * s, const char *filename, bool tex);

    bool Load(const char *file);
    bool morph(const char * file);
    
    void Update();
    void Reset();
    void Draw();
    
    bool tex;
    
    string texFileName;
    
private:
    Skeleton * skel;
    
    vector<Vertex> vertices;
    vector<Triangle> triangles;
    vector< vector <skinWeight> > weights;
    vector<Matrix34> bindings;
    vector<Vertex> draw;
    vector<Matrix34> skinMtx;
    
    Matrix34 localMtx;
    
};

#endif /* defined(__cse169__Skin__) */
