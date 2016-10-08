//
//  RoomDimension.hpp
//  hausarbeit
//
//  Created by Lennart Blom on 08.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#ifndef RoomDimension_hpp
#define RoomDimension_hpp

#include "Wall.hpp"
#include <stdio.h>
#include "Camera.h"
#include "Calculations.hpp"

extern Camera g_Camera;

class RoomDimension{
    
private:
    std::vector<Wall*> g_Walls;
    Model floor;
    Matrix floorMatrix;
    Matrix calculateFloorMatrix();
    void calculatePositions();
    float width;
    float depth;
    float height;

public:
    RoomDimension();
    RoomDimension(float pWidth, float pDepth, float pHeight, int colors[]);
    RoomDimension(Wall *pW1, Wall *pW2, Wall *pW3, Wall *pW4);
    void draw();
    void checkIfWallIsHit(Vector cameraPosition, Vector ray);
    ~RoomDimension();
    
    bool checkXRange(float pXPosition);
    bool checkZRange(float pZPosition);
    int* returnWallColors(int colors[]);
    
    const float getWidth();
    const float getDepth();
    const float getHeight();
    
    void changeWidth(float pFloat);
    void changeHeight(float pFloat);
    void changeDepth(float pFloat);

};

#endif /* RoomDimension_hpp */
