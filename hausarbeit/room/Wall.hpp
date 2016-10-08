//
//  Wall.hpp
//  hausarbeit
//
//  Created by Lennart Blom on 08.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#ifndef Wall_hpp
#define Wall_hpp

#include <stdio.h>
#include "Model.h"
#include "Matrix.h"


class Wall{
    
    
public:
    Wall();
    Wall(float pHeight, float pWidth);
    void draw();
    void activateRotation();
    void deactiveRoation();
    Vector getPosition();
    void setPosition(Vector pPosition);
    void setX(float pX);
    void setY(float pY);
    void setZ(float pZ);
    bool isRotated();
    void setWidth(float pWidth);
    float getHitDistance(Vector pCameraposition, Vector pRay);
    void switchMaterial();
    
    int getCurrentMaterial();
    void setCurrentMaterial(unsigned int pColor);
    
    float getHeight();
    float getWidth();
    void updateTransformationMatrix();
    
    
private:
    float height;
    float width;
    Vector position;
    bool rotate;
    unsigned int currentMaterial;
    
    Model m_Object;
    Matrix m_transformationMatrix;
};

#endif /* Wall_hpp */
