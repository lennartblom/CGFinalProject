//
//  AxisModel.cpp
//  hausarbeit
//
//  Created by Lennart Blom on 15.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#include "AxisModel.hpp"

AxisModel::AxisModel(){
    Matrix SM;
    
    this->arrow = new Model();
    this->scale = SM.scale(0.005);
    
    this->arrow->vLoad("objects/arrow.obj", false, "textures/tapete2.bmp");
    
    Matrix tranformXAxis;
    Matrix transformZAxis;
    this->spColor = new ShaderProgram();
    
    if(!this->spColor->load("shader/color.vs", "shader/color.fs")){
        std::cout << " +++++++ " << std::endl;
        std::cout << "Shader did not load." << std::endl;
        std::cout << " +++++++ " << std::endl;
    }else{
        
    }
    
}

///
/// Check which axis is hit
/// 1 = xAxis | 2 = yAxis | 3 = zAxis
/// -1 = no axis is hit
///
int AxisModel::checkIfAxisIsHit(const Vector& globalPosition, const Vector &o, const Vector &d){
    Matrix TM;
    Matrix TM2;
    
    TM.translation(globalPosition);
    TM2.translation(0, 0.1, 0);
    
    
    Matrix RM1, RM2;
    
    RM1.rotationZ(-M_PI_2);
    RM2.rotationX(-M_PI_2);
    
    glPushMatrix();
    Vector intersection;
    
    Matrix xAxisMatrix, yAxisMatrix, zAxisMatrix;
    
    xAxisMatrix = TM * TM2 * RM1 * this->scale;
    yAxisMatrix = TM * TM2 * scale;
    zAxisMatrix = TM * TM2 * RM2 * this->scale;
    
    if(this->arrow->BBoxIntersection(o, d, &intersection, xAxisMatrix)){
        return 1;
    }else if(this->arrow->BBoxIntersection(o, d, &intersection, yAxisMatrix)){
        return 2;
    }else if(this->arrow->BBoxIntersection(o, d, &intersection, zAxisMatrix)){
        return 3;
    }else{
        return -1;
    }
}

///
/// Drawing of the axes.
/// Pointing via shader. Issues while delivering a vec3 for changing the axis color. Color is hard-coded in the shader-file
/// -parameters:
///   - globalPosition: Position of currentFurnitureNode = new Position of the axisModel
///   - pRotation: Rotation of currentFurnitureNode
///
void AxisModel::draw(const Vector& globalPosition, const float pRotation){
   
    //
    // ToDo: Considering the rotation in an hierarchy-structure
    //
    
    Matrix TM;
    Matrix TM2;
    Matrix RM;
    
    RM.rotationY(pRotation);
    TM.translation(globalPosition);
    TM2.translation(0, 0.1, 0);
    
    glMatrixMode(GL_MODELVIEW);
    
    Matrix RM1, RM2;
    
    RM1.rotationZ(-M_PI_2);
    RM2.rotationX(-M_PI_2);
    
    
    // Id is always -1 or 0...?
    GLuint pId = this->spColor->getParameterID("colorUniform");
    
    glPushMatrix();
    
        this->spColor->setParameter(this->spColor->getParameterID("colorUniform"), Vector(1.0, 0.0, 0.0));
        glMultMatrixf(TM*TM2*this->scale);
        this->arrow->updateDraw(spColor);
    
    glPopMatrix();
    glPushMatrix();
    
        this->spColor->setParameter(this->spColor->getParameterID("colorUniform"), Vector(0.0, 1.0, 0.0));
    
        glMultMatrixf(TM*TM2*RM1*this->scale);
        this->arrow->updateDraw(spColor);
    
    glPopMatrix();
    glPushMatrix();
    
        this->spColor->setParameter(this->spColor->getParameterID("color"), Vector(0.0, 0.0, 1.0));
    
        glMultMatrixf(TM*TM2*RM2*this->scale);
        this->arrow->updateDraw(spColor);
    
    glPopMatrix();
    
}