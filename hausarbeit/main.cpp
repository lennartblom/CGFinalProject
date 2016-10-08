//
//  main.cpp
//  hausarbeit
//
//  Created by Lennart Blom on 08.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include "Camera.h"
#include "Model.h"
#include "Matrix.h"
#include "ShaderProgram.h"
#include "Furnituremanager.hpp"
#include "AxisModel.hpp"

const unsigned int g_WindowWidth=1024;
const unsigned int g_WindowHeight=768;
const Vector g_LightPos = Vector(0,10,0);

Camera g_Camera;
FurnitureManager *manager;
Model *testModel;

static int lastFrameTime = 0;

int g_MouseButton = 0;
int g_MouseState = 0;
int mouse_x = 0;
int mouse_y = 0;

unsigned int mouseClicked = 0;

void SetupDefaultGLSettings();
void DrawScene();
void MouseCallback(int Button, int State, int x, int y);
void MouseMoveCallback(int x, int y);
void KeyboardCallback( unsigned char key, int x, int y);
void MousePassiveMoveCallback( int x, int y);
void SpecialKeyboardCallback( int key, int x, int y);
void SpecialKeyboardUpCallback( int key, int x, int y);

int main(int argc, char * argv[])
{
    
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    glutCreateWindow("CG Hausarbeit | Lennart Blom");
    
    SetupDefaultGLSettings();
    
    glutDisplayFunc(DrawScene);
    glutMouseFunc(MouseCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutMotionFunc(MouseMoveCallback);
    glutPassiveMotionFunc(MousePassiveMoveCallback);
    glutSpecialFunc(SpecialKeyboardCallback);
    glutSpecialUpFunc(SpecialKeyboardUpCallback);
    
    
    g_Camera.setPosition(Vector(5, 3, 0));
    
    
    manager = new FurnitureManager("scene.txt");
    testModel = new Model();
    //testModel->load("objects/cube.obj", true);
    
    
    glutMainLoop();
    
}


void getMyCurrentMousePosition(int x, int y){
    if(x < 0){
        x = 0;
    }
    if(x > g_WindowWidth){
        x = g_WindowWidth;
    }
    
    if(y < 0){
        y = 0;
    }
    if(y > g_WindowHeight){
        y = g_WindowHeight;
    }
    
    mouse_x = x;
    mouse_y = y;
    
}

void SpecialKeyboardCallback( int key, int x, int y)
{
    // function is called if a special keyboard button is pressed (e. g. Up-arrow-Key)
    if(!manager->isInRoomGeometryMode()){
        if(GLUT_KEY_UP == key){
            manager->changeXPositionFromCurrentNode(0.1f);
        
        }else if(GLUT_KEY_DOWN == key){
            manager->changeXPositionFromCurrentNode(-0.1f);
        
        }else if(GLUT_KEY_RIGHT == key){
            manager->changeZPositionFromCurrentNode(0.1f);
        
        }else if(GLUT_KEY_LEFT == key){
            manager->changeZPositionFromCurrentNode(-0.1f);
        
        }
    }else{
        if(GLUT_KEY_UP == key){
            manager->changeRoomDepthDimension(0.5f);
            
        }else if(GLUT_KEY_DOWN == key){
            manager->changeRoomDepthDimension(-0.5f);
            
        }else if(GLUT_KEY_RIGHT == key){
            manager->changeRoomWidthDimension(0.5f);
            
        }else if(GLUT_KEY_LEFT == key){
            manager->changeRoomWidthDimension(-0.5f);
            
        }
    }
    
    
}


void SetupDefaultGLSettings()
{
    glClearColor(0, 0, 0, 255);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (double)g_WindowWidth / (double)g_WindowHeight, 0.045f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Setup Light Color
    GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    
    // Setup default material
    float diff[4] = { 1,1,1,1 };
    float amb[4] = { 0.2f,0.2f,0.2f,1 };
    float spec[4] = { 0.5f,0.5f,0.5f,1 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMateriali(GL_FRONT, GL_SHININESS, 30);
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    
    
}

void DrawGroundGrid()
{
    const float GridSize=10.0f;
    const unsigned int GridSegments=20;
    const float GridStep=GridSize/(float)GridSegments;
    const float GridOrigin=-GridSize*0.5f;
    
    glDisable( GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    for( unsigned int i=0; i<GridSegments+1; i++)
    {
        float itpos=GridOrigin + GridStep*(float)i;
        glVertex3f(itpos, 0, GridOrigin);
        glVertex3f(itpos, 0, GridOrigin+GridSize);
        
        glVertex3f(GridOrigin, 0, itpos);
        glVertex3f(GridOrigin+GridSize, 0, itpos);
        
    }
    glEnd();
    glEnable(GL_LIGHTING);
    
}

void MouseCallback(int Button, int State, int x, int y)
{
    mouseClicked++;
    g_MouseButton = Button;
    g_MouseState = State;
    g_Camera.mouseInput(x,y,Button,State);
    
    if(manager->isInAddTvMode()){
        if(mouseClicked%2 == 0)
            manager->addObject();
    }else{
        if(manager->hasCurrentFurnitureElement()){
            if(mouseClicked%2 == 1)
                manager->checkAxesFromCurrentNode();
            else
                manager->deactiveMoveAxisMode();
        }
        if(mouseClicked%2 == 0)
            manager->checkPicking();
    }
}

void MouseMoveCallback( int x, int y)
{
    if(manager->isInMovingAxisMode()){
        manager->moveCurrentNodeWithMouse(x, y, g_MouseButton, g_MouseState);
    }else{
        g_Camera.mouseInput(x,y,g_MouseButton,g_MouseState);
    }
    
}

void MousePassiveMoveCallback( int x, int y)
{
    mouse_x = x;
    mouse_y = y;
}

void KeyboardCallback( unsigned char key, int x, int y)
{
    
    if('h' == key){
        if(manager->hasCurrentFurnitureElement() && !manager->isInHierachyMode())
            manager->activateHierachyMode();
    }else if('r' == key){
        manager->changeRotationFromCurrentNode();
    }else if('m' == key){
        manager->safeScene();
    }else if('w' == key){
        manager->changeYPositionFromCurrentNode(0.1f);
    }else if('s' == key){
        manager->changeYPositionFromCurrentNode(-0.1f);
    }else if('+' == key){
        manager->increaseScaleFromCurrentNode();
    }else if('-' == key){
        manager->decreaseScaleFromCurrentNode();
    }else if('c' == key){
        manager->clearCurrentFurnitureNode();
    }else if('g' == key){
        if(!manager->isInRoomGeometryMode())
            manager->activateChanceRoomGeometryMode();
        
    }else if('e' == key){
        
        if(!manager->isInEraseHierachyMode()){
            manager->clearCurrentFurnitureNode();
            manager->activateEraseHierachyMode();
        }
        
    }else if('1' == key){
        if(!manager->isInAddTvMode()){
            manager->activateAddTvMode();
        }
    }else if('d' == key){
        manager->deleteCurrentNode();
    }else if('p' == key){
        manager->changePaper();
    }
}


void SpecialKeyboardUpCallback( int key, int x, int y){
    if('h' == key){
        if(manager->isInHierachyMode())
            manager->deactivateHierachyMode();
    }else if('e' == key){
        if(manager->isInEraseHierachyMode()){
            manager->deactivateEraseHierachyMode();
        }
    }else if('g' == key){
        if(manager->isInRoomGeometryMode()){
            manager->deactivateChanceRoomGeometryMode();
        }
    }else if('1' == key){
        if(manager->isInAddTvMode()){
            manager->deactivateAddTvMode();
        }
    }
}

void DrawScene(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat lpos[4];
    lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    
    g_Camera.apply();
    
    manager->draw();
    
    glutSwapBuffers();
    glutPostRedisplay();
    
}
