#include "Cloth.h"
#include <math.h>

Cloth::Cloth() {
    deltaTime = 0.01f;
    //ball = pSphere();
    ground = pPlane(Vector3(0.0f, -1.0f, 0.0f));
    
    //create particles
    for (int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            Particle * p = new Particle();
            //p->setPosition(i*ratio-(ratio*clothSize/2.0f), j*ratio-(ratio*clothSize/2.0f), 0.2);
            p->setPosition(i*ratio-(ratio*clothSize/2.0f), 0.2, j*ratio-(ratio*clothSize/2.0f));
            p->updatePrevious(p->getPosition());
            particles[i][j] = p;
        }
    }
    for (int i = 0; i < clothSize-1; i++) {
        for (int j = 0; j < clothSize-1; j++) {
            //adjacent side
            SpringDamper * s1 = new SpringDamper(particles[i][j],
                                                 particles[i+1][j],
                                                 ratio);//, largeDamp, largeSpring);
            //adjacent below
            SpringDamper * s2 = new SpringDamper(particles[i][j],
                                                 particles[i][j+1],
                                                 ratio);//, largeDamp, largeSpring);
            //diagonal positive
            SpringDamper * s3 = new SpringDamper(particles[i][j],
                                                 particles[i+1][j+1],
                                                 sqrt(2) * ratio);//, largeDamp, largeSpring);
            springs.push_back(s1);
            springs.push_back(s2);
            springs.push_back(s3);
        }
    }
    for (int i = 1; i < clothSize; i++) {
        for (int j = 0; j < clothSize-1; j++) {
            //diagonal negative
            SpringDamper * s1 = new SpringDamper(particles[i][j],
                                                 particles[i-1][j+1],
                                                 sqrt(2) * ratio);//, largeDamp, largeSpring);
            springs.push_back(s1);
        }
    }
    for (int j = 0; j < clothSize-1; j++) {
        //last column
        SpringDamper * s1 = new SpringDamper(particles[clothSize-1][j],
                                             particles[clothSize-1][j+1],
                                             ratio, largeDamp, largeSpring);
        //last row
        SpringDamper * s2 = new SpringDamper(particles[j][clothSize-1],
                                             particles[j+1][clothSize-1],
                                             ratio, largeDamp, largeSpring);
        springs.push_back(s1);
        springs.push_back(s2);
    }
    //set triangles
    for (int i = 0; i < clothSize-1; i++) {
        for (int j = 0; j < clothSize-1; j++) {
            pTriangle c = pTriangle();
            c.setVertex(particles[i][j],
                        particles[i][j+1],
                        particles[i+1][j+1]);
            triangles.push_back(c);
        }
    }
    for (int i = 1; i < clothSize; i++) {
        for (int j = 1; j < clothSize; j++) {
            pTriangle c = pTriangle();
            c.setVertex(particles[i][j],
                        particles[i][j-1],
                        particles[i-1][j-1]);
            triangles.push_back(c);
        }
    }
    
    //add shears for elasticity deformation
    int i = 0;
    while (i < clothSize-2) {
        int j = 0;
        while (j < clothSize-2) {
            //adjacent side
            SpringDamper * s1 = new SpringDamper(particles[i][j],
                                                 particles[i+2][j],
                                                 ratio*3, largeDamp, largeSpring);
            //adjacent below
            SpringDamper * s2 = new SpringDamper(particles[i][j],
                                                 particles[i][j+2],
                                                 ratio*3, largeDamp, largeSpring);
            bendingSprings.push_back(s1);
            bendingSprings.push_back(s2);
            j = j + 2;
        }
        i = i + 2;
    }
    int j = 0;
    while (j < clothSize-2) {
        //last column
        SpringDamper * s1 = new SpringDamper(particles[clothSize-1][j],
                                             particles[clothSize-1][j+2],
                                             ratio*3, largeDamp, largeSpring);
        //last row
        SpringDamper * s2 = new SpringDamper(particles[j][clothSize-1],
                                             particles[j+2][clothSize-1],
                                             ratio*3, largeDamp, largeSpring);
        bendingSprings.push_back(s1);
        bendingSprings.push_back(s2);
        j = j + 2;
    }

    //set which particles to be fixed, default top row
    for (int i = 0; i < clothSize; i++) {
        particles[i][clothSize-1]->setFixed(true);
    }
    
}

void Cloth::Update(Vector3 vair) {
    // add gravity force
    Vector3 gravity(0,-9.8,0);
    for(int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            Vector3 force = gravity * particles[i][j]->getMass(); // f=mg
            particles[i][j]->ApplyForce(force);
        }
    }
    //add spring damper force
    for (int i = 0; i < springs.size(); i++) {
        springs[i]->ComputeForce();
    }
    //add spring damper force for elasticity deformation fix
    for (int i = 0; i < bendingSprings.size(); i++) {
        //bendingSprings[i]->ComputeForce();
    }
    //add aerodynamic force
    for (int i = 0; i < triangles.size(); i++) {
        triangles[i].ComputeForce(vair);
        //ncross.Normalize();
        //triangles[i].setNormal(ncross);
    }
    
    // Integrate
    for(int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            particles[i][j]->Update(deltaTime);
        }
    }
    
    //Check for collisions
    for(int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            Segment seg;
            Intersection inter;
            seg.A = particles[i][j]->getPrevPosition();
            seg.B = particles[i][j]->getPosition();

            //check for collision with ground
            if (ground.TestSegment(seg, inter)) {
                particles[i][j]->setPosition(inter.Position.x, inter.Position.y, inter.Position.z);
                Vector3 v = particles[i][j]->getVelocity();
                v.x = 0.7 * v.x;
                v.y = -0.0001 * v.y;
                v.z = 0.7 * v.z;
                particles[i][j]->setVelocity(v.x, v.y, v.z);
            }

            particles[i][j]->updatePrevious(particles[i][j]->getPosition());
             
             
        }
    }
}

void Cloth::Draw(bool mode) {
    
    glPushMatrix();
    //glTranslatef(ball.getCenter().x, ball.getCenter().y, ball.getCenter().z);
    //glutWireSphere(ball.getRad(),10, 10);
    glPopMatrix();
    
    if (!mode) {
        /*glDisable(GL_LIGHTING);
        glColor3f(1, 0, 0);
        for (int i = 0; i < clothSize; i++) {
            particles[i][clothSize-1]->Draw();
        }
        glEnable(GL_LIGHTING);*/
        
        /*glPushMatrix();
        glTranslatef(ball.getCenter().x, ball.getCenter().y, ball.getCenter().z);
        glutSolidSphere(ball.getRad(),10, 10);
        glPopMatrix();*/
        
        glBegin(GL_TRIANGLES);
        GLfloat purple[] = {0.5f, 0.0f, 0.6f, 1.0f};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, purple);
        for (int i = 0; i < triangles.size(); i++) {
            Vector3 p1 = triangles[i].getVertex(0)->getPosition();
            Vector3 p2 = triangles[i].getVertex(1)->getPosition();
            Vector3 p3 = triangles[i].getVertex(2)->getPosition();
            Vector3 n = triangles[i].getNormal();
            
            glNormal3f(n.x, n.y, n.z);
            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
            glVertex3f(p3.x, p3.y, p3.z);
        }
        GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        glEnd();
        
        ground.DrawSolid();
        return;
        
    }
    
    glDisable(GL_LIGHTING);
    glColor3f(0, 0, 1);
    for (int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            particles[i][j]->Draw();
        }
    }
    
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    for (int i = 0; i < springs.size(); i++) {
        Vector3 p1 = springs[i]->P1->getPosition();
        Vector3 p2 = springs[i]->P2->getPosition();
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
    }
    glEnd();
    
    //draw plane
    //ground.DrawWire();
    ground.DrawSolid();
    glEnable(GL_LIGHTING);
}

void Cloth::Reset() {
    //reset particle positions
    for (int i = 0; i < clothSize; i++) {
        for (int j = 0; j < clothSize; j++) {
            particles[i][j]->Reset();
            //particles[i][j]->setPosition(i*ratio-(ratio*clothSize/2.0f), j*ratio-(ratio*clothSize/2.0f), 0.2);
            particles[i][j]->setPosition(i*ratio-(ratio*clothSize/2.0f), 0.2, j*ratio-(ratio*clothSize/2.0f));
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void Cloth::drag(float x, float y, float z) {
    //drag fixed particles, default top row
    for (int i = 0; i < clothSize; i++) {
        Vector3 prev =particles[i][clothSize-1]->getPosition();
        particles[i][clothSize-1]->setPosition(prev.x+x, prev.y+y, prev.z+z);
    }
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

