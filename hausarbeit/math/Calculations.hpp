//
//  Calculations.hpp
//  hausarbeit
//
//  Created by Lennart Blom on 09.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#ifndef Calculations_hpp
#define Calculations_hpp

#include <stdio.h>
#include "Vector.hpp"
#include "Camera.h"


extern int mouse_x;
extern int mouse_y;
extern Camera g_Camera;

class Calculations{
public:
    static float calculateWidth(int x){
        double factor = (2.0f/1024);
        return factor* (x - (1024*0.5f));
    }
    
    static float calculateHeight(int y){
        double factor = (2.0/768);
        return -((factor)* (y - (768*0.5f)));
    }
    
    static Vector calcMousePlaneXYPos(int pX, int pY){
        return Vector(calculateWidth(pX),calculateHeight(pY) ,0);
    }
    
    static Vector getRayInWorldSpace(){
        if(mouse_x > -1 && mouse_y > -1){
        Vector normalisedDeviceSpace = calcMousePlaneXYPos(mouse_x, mouse_y);
        
        normalisedDeviceSpace.setZ(0.0f);
        
        Matrix pm = g_Camera.getProjectionMatrix();
        Matrix vm = g_Camera.getViewMatrix();
        
        pm.invert();
        vm.invert();
        
        Vector eyeSpacePosition = pm.transformVec4x4(normalisedDeviceSpace);
        
        eyeSpacePosition.normalize();
        
            return vm.transformVec3x3(eyeSpacePosition).normalize();
        }else{
            return Vector(0, 0, 0);
        }
    }
    
    
    static Vector getPositionOnFloorWithMouseCoords(){
        if(mouse_x > -1 && mouse_y > -1){
            Vector normalisedDeviceSpace = calcMousePlaneXYPos(mouse_x, mouse_y);
            Vector cameraPosition = g_Camera.getPosition();
            
            normalisedDeviceSpace.setZ(0.0f);
            
            Matrix pm = g_Camera.getProjectionMatrix();
            Matrix vm = g_Camera.getViewMatrix();
            
            pm.invert();
            vm.invert();
            
            Vector eyeSpacePosition = pm.transformVec4x4(normalisedDeviceSpace);
            
            eyeSpacePosition.normalize();
            
            Vector rayInWorldSpace = getRayInWorldSpace();
            
            float factor = cameraPosition.getY() / rayInWorldSpace.getY();
            
            return (cameraPosition - rayInWorldSpace*factor);
            
        }else{
            return Vector(0, 0, 0);
        }
    }
};

#endif /* Calculations_hpp */
