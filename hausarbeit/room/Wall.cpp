//
//  Wall.cpp
//  hausarbeit
//
//  Created by Lennart Blom on 08.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#include "Wall.hpp"

Wall::Wall(){
    this->m_Object.vLoad("objects/boxModellingBasis.obj", false, "textures/tapete2.bmp");
}

Wall::Wall(float pHeight, float pWidth){
    
    this->deactiveRoation();
    this->height = pHeight;
    this->width = pWidth;
    this->position = Vector(0,0,0);
    this->currentMaterial = 0;
    
    this->updateTransformationMatrix();

    this->m_Object.vLoad("objects/boxModellingBasis.obj", false, "textures/tapete.bmp");
}

void Wall::updateTransformationMatrix(){
    
    Matrix heightScale, widthScale, depthScale;
    
    depthScale.scale(1, 1, 0.1f);
    heightScale.scale(1, this->height, 1);
    widthScale.scale(this->width, 1, 1);
    
    this->m_transformationMatrix = depthScale*heightScale*widthScale;
}

float Wall::getHitDistance(Vector pCameraposition, Vector pRay){
    
    Vector intersection;
    
    Matrix TM, globalTransformation;
    //std::cout << "Position: " << this->position << std::endl;
    TM.translation(this->position);
    
    if(this->rotate){
        Matrix tmp;
        tmp.rotationY(M_PI_2);
        globalTransformation = TM*tmp*this->m_transformationMatrix;
    }else{
        globalTransformation = TM*this->m_transformationMatrix;
    }
    
    if(this->m_Object.BBoxIntersection(pCameraposition, pRay, &intersection, globalTransformation)){
        
        return (intersection-pCameraposition).length();
    }
    
    return std::numeric_limits<float>::max();
}

void Wall::setWidth(float pWidth){
    this->width = pWidth;
}

void Wall::activateRotation(){
    this->rotate = true;
}
void Wall::deactiveRoation(){
    this->rotate = false;
}

bool Wall::isRotated(){
    return this->rotate;
}

float Wall::getHeight(){
    return this->height;
}
float Wall::getWidth(){
    return this->width;
}

Vector Wall::getPosition(){
    return this->position;
}

void Wall::setPosition(Vector pPosition){
    this->position = pPosition;
}

void Wall::setX(float pX){
    this->position.X = pX;
}

void Wall::setY(float pY){
    this->position.Y = pY;
}

void Wall::setZ(float pZ){
    this->position.Z = pZ;
}


void Wall::switchMaterial(){
    this->currentMaterial = (this->currentMaterial + 1) % 6;
}


void Wall::setCurrentMaterial(unsigned int pColor){
    this->currentMaterial = pColor;
}

int Wall::getCurrentMaterial(){
    return this->currentMaterial;
}

void Wall::draw(){
    Matrix TM;
    glMatrixMode(GL_MODELVIEW);
    
    TM.translation(this->position);
    
    glPushMatrix();
    
    
        if(this->rotate){
            Matrix tmp;
            tmp.rotationY(M_PI_2);
            glMultMatrixf(TM*tmp*this->m_transformationMatrix);
        }else{
            glMultMatrixf(TM*this->m_transformationMatrix);
        }
    
        this->m_Object.vDrawWall(this->currentMaterial);
    
    glPopMatrix();
    
}


