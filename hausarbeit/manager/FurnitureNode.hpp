//
//  FurnitureNode.hpp
//  hausarbeit
//
//  Created by Lennart Blom on 09.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#ifndef FurnitureNode_hpp
#define FurnitureNode_hpp

#include <stdio.h>
#include "Model.h"
#include "FurnitureNode.hpp"
#include <set>

class FurnitureNode{
    
    
public:
    FurnitureNode();
    
    /* getter */
    const std::string& getName() const;
    const Matrix& getLocalTransform() const;
    Matrix getGlobalTransform() const;
    const Vector& getScaling() const;
    FurnitureNode* getParent() const;
    const std::set<FurnitureNode*>& getChildren() const;
    const Vector& getPosition() const;
    const Vector getGlobalPosition() const;
    const Vector getParentPosition() const;
    const std::string getModelName();
    const Model* getModel() const;
    float getRotation() const;
    
    
    /* Setter */
    void setName( const std::string& pName);
    void setModel( Model* pModel);
    void setLocalTransform( const Vector & pTranslation, const float pRoationAngle);
    void setLocalTransform( const Matrix& pLocalTransform);
    void setScaling( const Vector& pScaling);
    void setModelName(std::string pModelPath);
    void setParent( FurnitureNode* pNode);
    void setPosition( const Vector &pVector);
    
    /* Other... */
    void addChild(FurnitureNode* pChild);
    void removeChild(FurnitureNode* pChild);
    
    void changePositionX(float pFactor);
    void changePositionY(float pFactor);
    void changePositionZ(float pFactor);
    
    void increaseScaling();
    void decreaseScaling();
    
    void rotate();
    
protected:
    float rotation;
    Vector position;
    std::string name;
    std::string modelPath;
    Model* model;
    FurnitureNode* parent;
    std::set<FurnitureNode*> children;
    Matrix localTransform;
    Vector scaling;
    
private:
    void updateLocalTransform();
    
    
};
#endif /* FurnitureNode_hpp */
