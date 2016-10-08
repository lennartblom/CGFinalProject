//
//  RoomDimension.cpp
//  hausarbeit
//
//  Created by Lennart Blom on 08.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#include "RoomDimension.hpp"

RoomDimension::RoomDimension(){
    
}

RoomDimension::RoomDimension(float pWidth, float pDepth, float pHeight, int colors[]){
    g_Walls.push_back(new Wall(pHeight, pWidth));
    g_Walls.push_back(new Wall(pHeight, pDepth));
    g_Walls.push_back(new Wall(pHeight, pWidth));
    g_Walls.push_back(new Wall(pHeight, pDepth));
    
    this->width = pWidth;
    this->depth = pDepth;
    this->height = pHeight;
    
    g_Walls.at(1)->activateRotation();
    g_Walls.at(3)->activateRotation();
    
    for(int i=0; i<4;i++)
        g_Walls.at(i)->setCurrentMaterial(colors[i]);
    
    
    this->floor.vLoad("objects/boxModellingBasis.obj", false, "textures/floor.bmp");
    
    this->calculatePositions();
    this->floorMatrix = this->calculateFloorMatrix();
}

RoomDimension::RoomDimension(Wall *pW1, Wall *pW2, Wall *pW3, Wall *pW4){
    
    g_Walls.push_back(pW1);
    g_Walls.push_back(pW2);
    g_Walls.push_back(pW3);
    g_Walls.push_back(pW4);
    
}


const float RoomDimension::getWidth(){
    return this->width;
}

const float RoomDimension::getDepth(){
    return this->depth;
}

const float RoomDimension::getHeight(){
    return this->height;
}

///
/// Checking if a wall was hit. If so, switch Material!
///
void RoomDimension::checkIfWallIsHit(Vector cameraPosition, Vector ray){
    
        bool intersect = false;
        int wallHit = -1;
        float nearest = std::numeric_limits<float>::max();
        float tmp;
        for(int i = 0; i<this->g_Walls.size(); i++){
            
            tmp = this->g_Walls.at(i)->getHitDistance(cameraPosition, ray);
            
            if(tmp < std::numeric_limits<float>::max()){
                std::cout << i+1 << ". Wand getroffen. " << std::endl;
                
                if(tmp < nearest){
                    nearest = tmp;
                    wallHit = i;
                }
            }
        }
    
    if(wallHit > -1)
        this->g_Walls.at(wallHit)->switchMaterial();
        
        
    
}


///
/// Returning the four different wall colors for saving it in the new scene.txt
///
int* RoomDimension::returnWallColors(int colors[]){
    for(int i=0; i<4;i++)
        colors[i] = this->g_Walls.at(i)->getCurrentMaterial();
    
    return colors;
}

void RoomDimension::changeWidth(float pFloat){
    
    if(this->width + pFloat > 1){
        float currentWidthUneven = g_Walls.at(0)->getWidth();
        g_Walls.at(0)->setWidth(currentWidthUneven+pFloat);
        g_Walls.at(2)->setWidth(currentWidthUneven+pFloat);
    
        this->width += pFloat;
    
        for(int i=0; i<g_Walls.size(); i++){
            g_Walls.at(i)->updateTransformationMatrix();
        }
    
        this->calculatePositions();
        this->floorMatrix = this->calculateFloorMatrix();
    }
}



void RoomDimension::changeDepth(float pFloat){
    
    if(this->depth + pFloat > 1){
        float currentWidthEven = g_Walls.at(1)->getWidth();
    
    
        g_Walls.at(1)->setWidth(currentWidthEven+pFloat);
        g_Walls.at(3)->setWidth(currentWidthEven+pFloat);
    
    
        this->depth += pFloat;
    
        for(int i=0; i<g_Walls.size(); i++){
            g_Walls.at(i)->updateTransformationMatrix();
        }
    
        this->calculatePositions();
        this->floorMatrix = this->calculateFloorMatrix();
    }
}


void RoomDimension::changeHeight(float pFloat){
    float currentWidth = g_Walls.at(1)->getWidth();
    
    
    this->g_Walls.at(1)->setWidth(currentWidth+pFloat);
    this->g_Walls.at(3)->setWidth(currentWidth+pFloat);
    
    this->calculatePositions();
    this->floorMatrix = this->calculateFloorMatrix();
}


///
/// Calculation of the floorMatrix with the positions and dimensions of the corresponding walls
///
Matrix RoomDimension::calculateFloorMatrix(){
    Matrix M1, M2, M3;
    
    float maxFloorWidth = 0.0;
    float maxFloorDepth = 0.0;
    
    for(int i=0; i<g_Walls.size(); i++){
        if(g_Walls.at(i)->isRotated()){
            maxFloorDepth = std::max(maxFloorDepth, g_Walls.at(i)->getWidth());
        }else{
            maxFloorWidth = std::max(maxFloorWidth, g_Walls.at(i)->getWidth());
        }
    }
    M1.scale(maxFloorWidth + 1 , 1, 1);
    M2.scale(1, 1, maxFloorDepth + 1);
    M3.scale(1, 0.01, 1);
    
    return M3*M2*M1;
}


///
/// Calculation of the new position, when a wall was made wider or smaller
///
void RoomDimension::calculatePositions(){
    g_Walls.at(0)->setZ(g_Walls.at(1)->getWidth());
    g_Walls.at(2)->setZ(-g_Walls.at(3)->getWidth());
    
    g_Walls.at(1)->setX(g_Walls.at(2)->getWidth());
    g_Walls.at(3)->setX(-g_Walls.at(0)->getWidth());
    
    for(int i=0; i<this->g_Walls.size(); i++){
        g_Walls.at(i)->setY(g_Walls.at(i)->getHeight());
    }
    
}


bool RoomDimension::checkXRange(float pXPosition){
    float maxX = 0;
    float minX = 0;
    
    for(int i=0; i<this->g_Walls.size();i++){
        if(!this->g_Walls.at(i)->isRotated()){
            maxX = this->g_Walls.at(i)->getWidth();
            minX = -this->g_Walls.at(i)->getWidth();
            
            return ((pXPosition > minX) && (pXPosition < maxX));
        }
    }
    return false;
}
bool RoomDimension::checkZRange(float pZPosition){
    float maxZ = 0;
    float minZ = 0;
    
    for(int i=0; i<this->g_Walls.size();i++){
        if(this->g_Walls.at(i)->isRotated()){
            maxZ = this->g_Walls.at(i)->getWidth();
            minZ = -this->g_Walls.at(i)->getWidth();
            
            return ((pZPosition > minZ) && (pZPosition < maxZ));
        }
    }
    return false;
}

void RoomDimension::draw(){
    
    for(int i=0; i<4; i++){
        g_Walls.at(i)->draw();
    }
    
    
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    
        glMultMatrixf(this->floorMatrix);
    
        this->floor.vDrawRoom();
    
    glPopMatrix();
    
}

RoomDimension::~RoomDimension(){
    
    
    
}

