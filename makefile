LIBS = -framework GLUT -framework OpenGl -framework Cocoa

default: animator_mac

mac: animator_mac

animator_mac: AnimationClip.h AnimationClip.cpp AnimationPlayer.h AnimationPlayer.cpp BMPImage.h BMPImage.cpp camera.h camera.cpp Channel.h Channel.cpp Cloth.h Cloth.cpp core.h pPlane.h pPlane.cpp pSphere.h pSphere.cpp pTriangle.h pTriangle.cpp core.cpp cube.h cube.cpp DOF.h DOF.cpp Joint.h Joint.cpp Keyframe.h Keyframe.cpp Matrix34.h Matrix34.cpp Particle.h Particle.cpp Skeleton.h Skeleton.cpp Skin.h Skin.cpp skinWeight.h skinWeight.cpp SpringDamper.h SpringDamper.cpp tester.h tester.cpp token.h token.cpp Triangle.h Triangle.cpp Vector3.h Vector3.cpp Vertex.h Vertex.cpp
	g++ -o animator -std=c++11 AnimationClip.cpp AnimationPlayer.cpp BMPImage.cpp camera.cpp Channel.cpp Cloth.cpp pPlane.cpp pSphere.cpp pTriangle.cpp core.cpp cube.cpp DOF.cpp Joint.cpp Keyframe.cpp Matrix34.cpp Particle.cpp Skeleton.cpp Skin.cpp skinWeight.cpp SpringDamper.cpp tester.cpp token.cpp Triangle.cpp Vector3.cpp Vertex.cpp $(LIBS)

clean:
	rm animator.exe
	rm animator

CC=/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-g++

LIBS_WIN = -mwindows glut32.lib -lopengl32 -lglu32

FLAGS= -std=c++11

win: animator_win

animator_win: AnimationClip.h AnimationClip.cpp AnimationPlayer.h AnimationPlayer.cpp BMPImage.h BMPImage.cpp camera.h camera.cpp Channel.h Channel.cpp Cloth.h Cloth.cpp pPlane.h pPlane.cpp pSphere.h pSphere.cpp pTriangle.h pTriangle.cpp core.h core.cpp cube.h cube.cpp DOF.h DOF.cpp Joint.h Joint.cpp Keyframe.h Keyframe.cpp Matrix34.h Matrix34.cpp Particle.h Particle.cpp Skeleton.h Skeleton.cpp Skin.h Skin.cpp skinWeight.h skinWeight.cpp SpringDamper.h SpringDamper.cpp tester.h tester.cpp token.h token.cpp Triangle.h Triangle.cpp Vector3.h Vector3.cpp Vertex.h Vertex.cpp
	$(CC) -o animator.exe $(FLAGS) AnimationClip.cpp AnimationPlayer.cpp BMPImage.cpp camera.cpp Channel.cpp Cloth.cpp pPlane.cpp pSphere.cpp pTriangle.cpp core.cpp cube.cpp DOF.cpp Joint.cpp Keyframe.cpp Matrix34.cpp Particle.cpp Skeleton.cpp Skin.cpp skinWeight.cpp SpringDamper.cpp tester.cpp token.cpp Triangle.cpp Vector3.cpp Vertex.cpp $(LIBS_WIN)

LIBS_LIN = -lGL -lglut -lGLU

lin: animator_lin


animator_lin: AnimationClip.h AnimationClip.cpp AnimationPlayer.h AnimationPlayer.cpp BMPImage.h BMPImage.cpp camera.h camera.cpp Channel.h Channel.cpp Cloth.h Cloth.cpp pPlane.h pPlane.cpp pSphere.h pSphere.cpp pTriangle.h pTriangle.cpp core.h core.cpp cube.h cube.cpp DOF.h DOF.cpp Joint.h Joint.cpp Keyframe.h Keyframe.cpp Matrix34.h Matrix34.cpp Particle.h Particle.cpp Skeleton.h Skeleton.cpp Skin.h Skin.cpp skinWeight.h skinWeight.cpp SpringDamper.h SpringDamper.cpp tester.h tester.cpp token.h token.cpp Triangle.h Triangle.cpp Vector3.h Vector3.cpp Vertex.h Vertex.cpp
	g++ -o animator $(FLAGS) AnimationClip.cpp AnimationPlayer.cpp BMPImage.cpp camera.cpp Channel.cpp Cloth.cpp pPlane.cpp pSphere.cpp pTriangle.cpp core.cpp cube.cpp DOF.cpp Joint.cpp Keyframe.cpp Matrix34.cpp Particle.cpp Skeleton.cpp Skin.cpp skinWeight.cpp SpringDamper.cpp tester.cpp token.cpp Triangle.cpp Vector3.cpp Vertex.cpp $(LIBS_LIN)