//
//  FurnitureNode.cpp
//  hausarbeit
//
//  Created by Lennart Blom on 09.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#include "FurnitureNode.hpp"

FurnitureNode::FurnitureNode(){
    this->position = Vector(0,0,0);
    this->rotation = 0;
}

const std::string& FurnitureNode::getName() const{
    return this->name;
}


const Matrix& FurnitureNode::getLocalTransform() const{
    return this->localTransform;
}

const Vector FurnitureNode::getParentPosition() const{
    if(this->getParent() != NULL){
        if(this->getParent()->getPosition().length() > 0)
            return this->getParent()->getPosition();
        return Vector(0,0,0);
    }else{
        return Vector(0,0,0);
    }
}

const Vector FurnitureNode::getGlobalPosition() const{
    if(this->getParent() != NULL){
        return this->getParent()->getGlobalPosition()+this->position;
    }else{
        
        return this->position;
    }
}


void FurnitureNode::setPosition( const Vector &pVector){
    this->position = pVector;
    this->updateLocalTransform();
}


Matrix FurnitureNode::getGlobalTransform() const{
    if(this->getParent() != NULL){
        return this->getParent()->getGlobalTransform()*localTransform;
        
    }else{
        return localTransform;
    }
}

void FurnitureNode::setModelName(std::string pModelPath){
    this->modelPath = pModelPath;
}


const std::string FurnitureNode::getModelName(){
    return this->modelPath;
}


const Vector& FurnitureNode::getPosition() const{
    return this->position;
}

void FurnitureNode::increaseScaling(){
    this->scaling = this->scaling * 1.1f;
    this->updateLocalTransform();
}

void FurnitureNode::decreaseScaling(){
    this->scaling = this->scaling * 0.9f;
    this->updateLocalTransform();
}


void FurnitureNode::rotate(){
    this->rotation += M_PI_2;
    this->updateLocalTransform();	
}

const Vector& FurnitureNode::getScaling() const{
    return this->scaling;
}


float FurnitureNode::getRotation() const{
    return this->rotation;
}

void FurnitureNode::changePositionX(float pFactor){
    Matrix RM, TM;
    
    this->position.X += pFactor;
    
    RM.rotationY(this->rotation);
    TM.translation(this->position.getX(), this->position.getY(), this->position.getZ());
    
    this->localTransform = TM*RM;
}

void FurnitureNode::changePositionY(float pFactor){
    if(!((this->position.getY() + pFactor) < 0)){
        Matrix RM, TM;
        
        this->position.Y += pFactor;
        
        RM.rotationY(this->rotation);
        TM.translation(this->position.getX(), this->position.getY(), this->position.getZ());
        
        this->localTransform = TM*RM;
    }
    
}

void FurnitureNode::changePositionZ(float pFactor){
    Matrix RM, TM;
    
    this->position.Z += pFactor;
    
    RM.rotationY(this->rotation);
    TM.translation(this->position.getX(), this->position.getY(), this->position.getZ());
    
    this->localTransform = TM*RM;
}

FurnitureNode* FurnitureNode::getParent() const{
    return this->parent;
}

const std::set<FurnitureNode*>& FurnitureNode::getChildren() const{
    return this->children;
}

const Model* FurnitureNode::getModel() const{
    return this->model;
}

void FurnitureNode::setName( const std::string& pName){
    this->name = pName;
}

void FurnitureNode::setModel( Model* pModel){
    this->model = pModel;
}

void FurnitureNode::updateLocalTransform(){
    this->setLocalTransform(this->position, this->rotation);
}

void FurnitureNode::setLocalTransform( const Vector & pTranslation, const float pRotationAngle){
    Matrix rotationMatrix;
    Matrix transformMatrix;
    
    rotationMatrix.rotationY(pRotationAngle);
    this->rotation = pRotationAngle;
    this->position = pTranslation;
    transformMatrix.translation(pTranslation);
    
    this->localTransform = transformMatrix*rotationMatrix;
}

void FurnitureNode::setLocalTransform( const Matrix& pLocalTransform){
    this->localTransform = pLocalTransform;
}

void FurnitureNode::setScaling( const Vector& pScaling){
    this->scaling = pScaling;
}

void FurnitureNode::setParent( FurnitureNode* pNode){
    this->parent = pNode;
}

void FurnitureNode::addChild(FurnitureNode* pChild){
    this->children.insert(pChild);
}

void FurnitureNode::removeChild(FurnitureNode* pChild){
    this->children.erase(pChild);
}