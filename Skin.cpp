#include "Skin.h"

////////////////////////////////////////////////////////////////////////////////
//constructors
////////////////////////////////////////////////////////////////////////////////

Skin::Skin() {
    
}

Skin::Skin(Skeleton * s) {
    skel = s;
}

Skin::Skin(Skeleton * s, const char *filename, bool t) {
    skel = s;
    tex = t;
    Load(filename);
    Reset();
}


////////////////////////////////////////////////////////////////////////////////
// load .skin file
////////////////////////////////////////////////////////////////////////////////

bool Skin::Load(const char *file) {
    //if (tex) texture = BMPImage();
    
    Tokenizer token;
    token.Open(file);
    int idx;

    token.FindToken("positions");

    idx = token.GetInt();
    token.FindToken("{");
    
    float x;
    float y;
    float z;
    for (int i = 0; i < idx; i++) {
        vertices.push_back(Vertex());
        draw.push_back(Vertex());
        x = token.GetFloat();
        y = token.GetFloat();
        z = token.GetFloat();
        vertices[i].setPosition(Vector3(x, y, z));
        draw[i].setPosition(Vector3());
    }

    token.FindToken("normals");
    token.FindToken("{");
    for (int i = 0; i < idx; i++) {
        x = token.GetFloat();
        y = token.GetFloat();
        z = token.GetFloat();
        vertices[i].setNormal(Vector3(x, y, z));
        draw[i].setNormal(Vector3());
    }

    if (tex) {
        token.FindToken("texcoords");
        token.FindToken("{");
        for (int i = 0; i < idx; i++) {
            x = token.GetFloat();
            y = token.GetFloat();
            vertices[i].setTexCoord(Vector3(x, y, 0));
            draw[i].setTexCoord(Vector3(x, y, 0));
        }
    }
    
    token.FindToken("skinweights");
    token.FindToken("{");
    for (int i = 0; i < idx; i++) {
        int numJoints = token.GetInt();
        vector<skinWeight> inner;
        for (int j = 0; j < numJoints; j++) {
            int joint = token.GetInt();
            float w = token.GetFloat();
            inner.push_back(skinWeight(joint, w));
        }
        weights.push_back(inner);
    }

    if (tex) {
        token.FindToken("material");
        token.FindToken("{");

        token.FindToken("texture");
        char temp[256];
        token.GetToken(temp);
        texFileName = temp;
        //LoadGLTextures(load);
    }
    
    token.FindToken("triangles");
    idx = token.GetInt();
    token.FindToken("{");
    for (int i = 0; i < idx; i++) {
        triangles.push_back(Triangle());
        x = token.GetInt();
        y = token.GetInt();
        z = token.GetInt();
        triangles[i].Init(&draw[x], &draw[y], &draw[z]);
        triangles[i].Init(x, y, z);
    }


    token.FindToken("bindings");
    idx = token.GetInt();
    token.FindToken("{");
    for (int i = 0; i < idx; i++) {
        
        token.FindToken("{");

        float ax = token.GetFloat();
        float ay = token.GetFloat();
        float az = token.GetFloat();

        
        float bx = token.GetFloat();
        float by = token.GetFloat();
        float bz = token.GetFloat();
        
        float cx = token.GetFloat();
        float cy = token.GetFloat();
        float cz = token.GetFloat();
        
        float dx = token.GetFloat();
        float dy = token.GetFloat();
        float dz = token.GetFloat();
        
        bindings.push_back(Matrix34 (ax, bx, cx, dx,
                                     ay, by, cy, dy,
                                     az, bz, cz, dz) );
        bindings[i].Inverse();
    }
    
    // Finish
    token.Close();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// morph the skin
////////////////////////////////////////////////////////////////////////////////

bool Skin::morph(const char *file) {
    Tokenizer token;
    token.Open(file);
    int count;
     
    token.FindToken("positions");
    count = token.GetInt();
    token.FindToken("{");
     
    float x;
    float y;
    float z;
    int vIdx;
    for (int i = 0; i < count; i++) {
        vIdx = token.GetInt();
        x = token.GetFloat();
        y = token.GetFloat();
        z = token.GetFloat();
        vertices[vIdx].setPosition(Vector3(x, y, z));
    }
     
    token.FindToken("normals");
    token.FindToken("{");
    for (int i = 0; i < count; i++) {
        vIdx = token.GetInt();
        x = token.GetFloat();
        y = token.GetFloat();
        z = token.GetFloat();
        vertices[vIdx].setNormal(Vector3(x, y, z));
    }
     
    // Finish
    token.Close();
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void Skin::Update() {
    //compute blended vertices and normals
    skinMtx.clear();
    
    int i = 0;
    for (auto& bind : bindings) {
        localMtx.Identity();
        localMtx.Dot(skel->getWorldMatrix(i), bind);
        skinMtx.push_back(localMtx);
        i++;
    }
    
    i = 0;
    for (auto& vertex : vertices) {
        Vector3 sumP = Vector3();
        Vector3 sumN = Vector3();
        for (auto& w : weights[i]) {
            Vector3 pos = vertex.getPosition();
            Vector3 norm = vertex.getNormal();
            
            localMtx.MakeScale(w.weight, w.weight, w.weight);
            localMtx.Dot(localMtx, skinMtx[w.joint]);
            
            localMtx.Transform(pos, pos);
            sumP = sumP + pos;
        
            localMtx.Transform3x3(norm, norm);
            sumN = sumN + norm;
        }
        sumN.Normalize();
        
        draw[i].setNormal(sumN);
        draw[i].setPosition(sumP);
        i++;
    }
    skinMtx.clear();
}

////////////////////////////////////////////////////////////////////////////////

void Skin::Reset() {
    // Reset dynamic variables to a default state
    localMtx.Identity();
    
}

////////////////////////////////////////////////////////////////////////////////

void Skin::Draw() {
    glLoadIdentity();
    //loop through indices and draw skin
    
    glEnable(GL_TEXTURE_2D);

    for (auto& tri : triangles) {
        glBegin(GL_TRIANGLES);
        //for each vertex
        for (int j = 0; j < 3; j++) {
            
            Vector3 pos = draw[tri.getIdx(j)].getPosition();
            Vector3 norm = draw[tri.getIdx(j)].getNormal();

            glNormal3f(norm.x, norm.y, norm.z);
            
            Vector3 texCoord = draw[tri.getIdx(j)].getTexCoord();
            glTexCoord2f(texCoord.x, texCoord.y);
    
            glVertex3f(pos.x, pos.y, pos.z);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
}

