////////////////////////////////////////
// tester.h
////////////////////////////////////////

#ifndef CSE169_TESTER_H
#define CSE169_TESTER_H

#include "camera.h"
#include "Skeleton.h"
#include "Skin.h"
#include "AnimationPlayer.h"
#include "Cloth.h"
#include "core.h"
#include "Shader.h"

////////////////////////////////////////////////////////////////////////////////

class Tester {
public:
	Tester(int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Draw();

	void Quit();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);
    void specialKeys(int key, int x, int y);
    
    //project selection and display
    void displayText();
    void drawText(int x, int y, string text, float r, float g, float b);
    void drawChannels();
    void selectProject();
    void LoadGLTextures();

private:
	// Window management
	int WindowHandle;
	int WinX,WinY;
    
    bool drawSkel;
    bool drawSkin;
    bool drawTex;
    bool morph;
    bool animate;
    bool skelMode;
    bool showText;
    bool clothSim;

	// Input
	bool LeftDown,MiddleDown,RightDown;
	int MouseX,MouseY;
    
	// Components
	Camera Cam;
    Skeleton skeleton;
    Skin skin;
    AnimationPlayer player;
    AnimationClip clip;
    Cloth cloth;
    Vector3 airVelocity;
    
    //project selection and display
    string skelname;
    string skinname;
    string morphname;
    string animName;
    char buffer [100];
    int selectedDof;
    Joint * selected;
    
    int fileSelect;
    size_t currentJoint;
    string project;
    
    GLuint texture[1];
    
};

////////////////////////////////////////////////////////////////////////////////

/*
The 'Tester' is a simple top level application class. It creates and manages a
window with the GLUT extension to OpenGL and it maintains a simple 3D scene
including a camera and some other components.
*/

#endif
