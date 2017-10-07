#ifndef __cse169__BMPImage__
#define __cse169__BMPImage__

#include "core.h"

class BMPImage {
    
public:
    ~BMPImage ()
    {
        delete [] data;
    }
    
    bool load (const char* filename);
    
    const char * filename;
    
    unsigned long sizeX = 0;
    unsigned long sizeY = 0;
    char *data;
    
    // storage for one texture
    GLuint texture[1];
    
    void swapTwoBytes (unsigned short* s);
    void swapFourBytes (unsigned long* dw);
    void LoadGLTextures (char * filename);
    
    unsigned char* loadPPM(const char* filename, int& width, int& height);
};

#endif /* defined(__cse169__BMPImage__) */
