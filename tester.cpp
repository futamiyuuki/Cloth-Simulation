////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

#define MAXFILES 10

////////////////////////////////////////////////////////////////////////////////

string appName = "animator";
string APP_PATH = "/Users/Yuuki/Desktop/HW/CSE169/PA4/";

Shader *shaderd;

static Tester *TESTER;

int   wireframe = 0;
int   dof = 9;
int   main_window;

// Load Bitmaps And Convert To Textures
void Tester::LoadGLTextures () {
    // Load Texture
    BMPImage image1;
    
    string imgName = APP_PATH + "data/head/head.bmp";
    if (!image1.load (imgName.c_str()))
        exit (1);
    
    // Create Texture
    glGenTextures (1, &texture[0]);
    glBindTexture (GL_TEXTURE_2D, texture[0]);
    
    // note that BMP images store their data in BRG order, not RGB
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D (GL_TEXTURE_2D, 0, 3, image1.sizeX, image1.sizeY, 0, GL_BGR,
                  GL_UNSIGNED_BYTE, image1.data);
    
    glBindTexture (GL_TEXTURE_2D, 0);
};

int main(int argc, char **argv) {
    //APP_PATH = argv[0];
    
    /*if (APP_PATH.find("Xcode") == std::string::npos) {
        std::string::size_type i = APP_PATH.find_last_of(appName);
        if (i != std::string::npos) APP_PATH.erase(i-appName.length()+1, i);
        APP_PATH = APP_PATH + "/";
    } else {
        APP_PATH = "";
    }*/
    
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);
	glutMainLoop();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}
static void specialkeys(int key, int x, int y)         {TESTER->specialKeys(key, x, y); }

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
    showText = true;
    
    fileSelect = 10;
    currentJoint = 0;
    selectedDof = 0;
    project = "Cloth";
    skelMode = false;
    clothSim = true;
    
    /*animName = "/Users/Yuuki/Desktop/HW/CSE169/PA4/data/wasp/wasp_walk.anim";
    cout << APP_PATH << endl;
    clip = AnimationClip();
    clip.Load(animName.c_str());
    player.setClip(clip);*/
    airVelocity.Zero();
    cloth = Cloth();
    
	WinX=1000;
	WinY=645;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;
    
    
    // Create the window
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( WinX, WinY );
    glutInitWindowPosition( 0, 0 );
    WindowHandle = glutCreateWindow( "Final Project" );
    glutSetWindow( WindowHandle );
    
    glShadeModel(GL_SMOOTH);
    
    glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
    //glEnable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);       	      // clear depth buffer
    
    // Background color
    glClearColor( 0., 0., 0., 1. );
    
    // Callbacks
    glutDisplayFunc( display );
    glutIdleFunc( idle );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mousebutton );
    glutMotionFunc( mousemotion );
    glutPassiveMotionFunc( mousemotion );
    glutReshapeFunc( resize );
    glutSpecialFunc( specialkeys );
    
	// Initialize components
    //LoadGLTextures();
	Cam.SetAspect(float(WinX)/float(WinY));
    Cam.SetDistance(2.08799f);
    Cam.SetAzimuth(190.0f);
    Cam.SetIncline(11.0f);
    
    shaderd = new Shader("/Users/Yuuki/Desktop/HW/CSE169/PA4/diffuse_shading.vert","/Users/Yuuki/Desktop/HW/CSE169/PA4/diffuse_shading.frag",true);
    
    //shaderd->bind();
    
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
    
	//skeleton.Update();
    //if (!drawSkel) skin.Update();
    //if (animate) player.Update();
    cloth.Update(airVelocity);
    
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
    

    Cam.SetDistance(2.08799f);
    Cam.SetAzimuth(190.0f);
    Cam.SetIncline(11.0f);

    
	skeleton.Reset();
    skin.Reset();
    player.Reset();
    cloth.Reset();
    airVelocity.Zero();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
    
    
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //shaderd->bind();
    // Generate light source:
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    
    GLfloat light_diffuse0[] = {0.0, 1.0, 0.0, 1.0};
    GLfloat light_position0[] = {-1.0, 2.5, -1.0, 0.0};
    
    GLfloat light_diffuse1[] = {1.0, 0.0, 1.0, 1.0};
    GLfloat light_diffuse2[] = {1.0, 1.0, 1.0, 1.0};
    
    GLfloat light_position1[] = {1.0, 1.0, -1.0, 1.0};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_diffuse1);    //<ambient/diffuse/specular>);
    /*glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    
    if (drawTex) glLightfv(GL_LIGHT1, GL_AMBIENT, light_diffuse2);
    else glLightfv(GL_LIGHT1, GL_AMBIENT, light_diffuse1);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 100.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0); */

    //shaderd->bind();
    
	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices
	
    if (drawSkel) {
        glDisable(GL_LIGHTING);
        skeleton.Draw(skelMode);
        glEnable(GL_LIGHTING);
    }
    if (drawSkin) skin.Draw();
    if (showText) displayText();
    if (clothSim) cloth.Draw(skelMode);
    
    //shaderd->unbind();
    
	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::displayText() {
    //setup window for 2d drawing
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, WinX, 0.0, WinY);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    

    //drawText(WinX-284, WinY - 45, "project 4 aerodynamics: wind speed adjustment", 1.0f, 1.0f, 1.0f);
    //drawText(WinX-284, WinY - 60, "use the spacebar to select a dof below", 1.0f, 1.0f, 1.0f);
    //drawText(WinX-284, WinY - 75, "use the left/right arrow keys to adjust the speed", 1.0f, 1.0f, 1.0f);
        
    float toggle = 1.0f;
    sprintf(buffer, "%f", airVelocity.x);
    if (selectedDof == 0) toggle = 0.0f;
    drawText(WinX-120, WinY / 2 + 30, "X wind: " + string(buffer), 1.0f, toggle, toggle);
    toggle = 1.0f;
    sprintf(buffer, "%f", airVelocity.y);
    if (selectedDof == 1) toggle = 0.0f;
    drawText(WinX-120, WinY / 2, "Y wind: " + string(buffer), 1.0f, toggle, toggle);
    toggle = 1.0f;
    sprintf(buffer, "%f", airVelocity.z);
    if (selectedDof == 2) toggle = 0.0f;
    drawText(WinX-120, WinY / 2 - 30, "Z wind: " + string(buffer), 1.0f, toggle, toggle);

    //finish 2d drawing
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::drawText(int x, int y, string text, float r, float g, float b) {
    
    glColor3f(r, g, b);//needs to be called before RasterPos
    glRasterPos2i(x, y);
    void * font = GLUT_BITMAP_HELVETICA_12;
    
    for (std::string::iterator i = text.begin(); i != text.end(); ++i) {
        char c = *i;
        //this does nothing, color is fixed for Bitmaps when calling glRasterPos
        //glColor3f(1.0, 0.0, 1.0);
        glutBitmapCharacter(font, c);
    }


}

////////////////////////////////////////////////////////////////////////////////

void Tester::drawChannels() {
    int offsetX = WinX - 440;
    int offsetY = 210;
    drawText(WinX-240, WinY - 45, "project 3 extra credit: ", 1.0f, 1.0f, 1.0f);
    drawText(WinX-240, WinY - 60, "display animation channel curves", 1.0f, 1.0f, 1.0f);
    
    drawText(offsetX-10,  offsetY-10, "0", 1.0f, 1.0f, 1.0f);
    drawText(offsetX+90,  offsetY-10, "1", 1.0f, 1.0f, 1.0f);
    drawText(offsetX+190, offsetY-10, "2", 1.0f, 1.0f, 1.0f);
    drawText(offsetX+290, offsetY-10, "3", 1.0f, 1.0f, 1.0f);
    drawText(offsetX+390, offsetY-10, "4", 1.0f, 1.0f, 1.0f);
    drawText(offsetX+405, offsetY-10, "time", 1.0f, 1.0f, 1.0f);
    
    drawText(WinX-240, WinY-155, "press M to toggle walk movement", 1.0f, 1.0f, 1.0f);
    
    glBegin(GL_LINES);
    glVertex2f(offsetX, offsetY-200);
    glVertex2f(offsetX, offsetY+200);
    glVertex2f(offsetX-100, offsetY);
    glVertex2f(offsetX+400, offsetY);
    glEnd();
    
    player.drawChannelCurve(currentJoint, 0, offsetX, offsetY, 1.0f, 0.0f, 0.0f);
    player.drawChannelCurve(currentJoint, 1, offsetX, offsetY, 0.0f, 1.0f, 0.0f);
    player.drawChannelCurve(currentJoint, 2, offsetX, offsetY, 0.0f, 0.0f, 1.0f);
    
    sprintf(buffer, "%f", selected->dofX.getPose());
    drawText(WinX-150, WinY - 100, "X: " + string(buffer), 1.0f, 0.0f, 0.0f);
    
    sprintf(buffer, "%f", selected->dofY.getPose());
    drawText(WinX-150, WinY - 115, "Y: " + string(buffer), 0.0f, 1.0f, 0.0f);
    
    sprintf(buffer, "%f", selected->dofZ.getPose());
    drawText(WinX-150, WinY - 130, "Z: " + string(buffer), 0.0f, 0.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
        case 't':
            if (fileSelect < 4 || clothSim) skelMode = !skelMode;
            //if (fileSelect > 3 && fileSelect < 6) drawSkel = !drawSkel;
            break;
		case 'r':
			Reset();
			break;
        case 'h':
            showText = !showText;
            break;
        /*case ',':
            currentJoint = 0;
            if (fileSelect == 0) fileSelect = MAXFILES;
            else fileSelect = fileSelect - 1;
            selectProject();
            break;
        case '.':
            currentJoint = 0;
            if (fileSelect == MAXFILES) fileSelect = 0;
            else fileSelect = fileSelect + 1;
            selectProject();
            break;*/
        case 'a':
            if (clothSim) cloth.drag(0.1f, 0.0f, 0.0f);
            break;
        case 'd':
            if (clothSim) cloth.drag(-0.1f, 0.0f, 0.0f);
            break;
        case 'w':
            if (clothSim) cloth.drag(0.0f, 0.1f, 0.0f);
            break;
        case 's':
            if (clothSim) cloth.drag(0.0f, -0.1f, 0.0f);
            break;
        case 'z':
            if (clothSim) cloth.drag(0.0f, 0.0f, 0.1f);
            break;
        case 'x':
            if (clothSim) cloth.drag(0.0f, 0.0f, -0.1f);
            break;
    }
}

void Tester::specialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_LEFT:
            if (selectedDof == 0) airVelocity.x -= 5.0f;
            if (selectedDof == 1) airVelocity.y -= 5.0f;
            if (selectedDof == 2) airVelocity.z -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            if (selectedDof == 0) airVelocity.x += 5.0f;
            if (selectedDof == 1) airVelocity.y += 5.0f;
            if (selectedDof == 2) airVelocity.z += 5.0f;
            break;
        case GLUT_KEY_UP:
            if (selectedDof > 0) selectedDof--;
            else selectedDof = 2;
            break;
        case GLUT_KEY_DOWN:
            if (selectedDof < 2) selectedDof++;
            else selectedDof = 0;
            break;
    }
}

/*void Tester::selectProject() {
    if (drawTex) glBindTexture (GL_TEXTURE_2D, 0);
    
    //reset variables
    drawSkel = false;
    drawTex = false;
    morph = false;
    animate = false;
    drawSkin = false;
    skelMode = true;
    clothSim = false;
    airVelocity.Zero();
    

    project = "4 - Final Project - Cloth Simulation";
    clothSim = true;
    cloth.Reset();
    Cam.SetDistance(2.08799f);
    Cam.SetAzimuth(152.0f);
    Cam.SetIncline(11.0f);
    
    
    skeleton = Skeleton(skelname.c_str());
    selected = skeleton.joints[currentJoint];
    if (drawSkin) skin = Skin(&skeleton, skinname.c_str(), drawTex);
    if (morph) skin.morph(morphname.c_str());
    if (drawTex) glBindTexture (GL_TEXTURE_2D, texture[0]);
    if (animate) player.setSkel(skeleton);
}*/

////////////////////////////////////////////////////////////////////////////////
void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
        const float rate=1.0f;
        Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
        Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
    
    //cout << "Azim " << Cam.GetAzimuth() << endl;
    //cout << "Dist " << Cam.GetDistance() << endl;
    //cout << "Incl " << Cam.GetIncline() << endl;
    
}

////////////////////////////////////////////////////////////////////////////////

