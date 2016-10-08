//
//  FurnitureManager.hpp
//  hausarbeit
//
//  Created by Lennart Blom on 08.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#ifndef FurnitureManager_hpp
#define FurnitureManager_hpp

#include "RoomDimension.hpp"
#include "FurnitureNode.hpp"
#include "SceneFileWriter.hpp"
#include "MoveObjectWithMouseController.hpp"
#include <stdio.h>
#include "AxisModel.hpp"

class FurnitureManager{
private:
    
    /* Help classes and Objects */
    
    // Help class for calculating stuff
    Calculations mathClass;
    
    // Configurations of the room
    RoomDimension roomDimension;

    
    FurnitureNode m_Root;
    ShaderProgram *shader;
    FurnitureNode *currentFurnitureObject;
    
    // Help class for moving object with mouse
    MoveObjectWithMouseController moController;
    
    AxisModel axisModel;
    
    /* Modes */
    bool hierarchyMode;
    bool eraseHierarchyMode;
    bool chanceRoomGeometryMode;
    bool addTvMode;
    bool deleteMode;
    
    /* Moving Axes */
    bool moveAxes;
    int movingAxis;
    
    /* Other stuff */
    unsigned int tvCounter;
    std::map<std::string, Model*> models;
    std::map<std::string, FurnitureNode*> objects;
    
    
    void setupRoomDimension(std::fstream &sceneFStream);
    void addFurnitureNodeFromStream(std::fstream &sceneFStream);
    void clearHierachyFromNode(FurnitureNode *pNode);
    bool addSceneFile( const char* pSceneFile);
    
    bool checkXRangeFromCurrentNode(float pFactor);
    bool checkZRangeFromCurrentNode(float pFactor);
    
    
public:
  
    
    FurnitureManager();
    FurnitureManager(const char *sceneFile);
    ~FurnitureManager();
    
    
    /* getter */
    bool hasCurrentFurnitureElement();
    bool isInHierachyMode();
    bool isInEraseHierachyMode();
    bool isInRoomGeometryMode();
    bool isInMovingAxisMode();
    bool isInAddTvMode();
    
    /* setter */
    void deactiveMoveAxisMode();
    void deleteCurrentNode();
    void activateHierachyMode();
    void deactivateHierachyMode();
    void activateEraseHierachyMode();
    void deactivateEraseHierachyMode();
    void activateChanceRoomGeometryMode();
    void deactivateChanceRoomGeometryMode();
    void safeScene();
    
    /* other */
    void moveCurrentNodeWithMouse(int x, int y, int Button, int State);
    
    void checkPicking();
    bool objectPicking(Vector cameraPosition, Vector ray);
    bool checkAxesFromCurrentNode();
    void changePaper();
    bool addFurnitureNode();
    
    
    
    
    void addObject();
    void activateAddTvMode();
    void deactivateAddTvMode();
    void clearCurrentFurnitureNode();
    
    /* Positions, Scalation and Rotation from CurrentNode*/
    void changeRotationFromCurrentNode();
    void changeXPositionFromCurrentNode(float pFactor);
    void changeYPositionFromCurrentNode(float pFactor);
    void changeZPositionFromCurrentNode(float pFactor);
    void increaseScaleFromCurrentNode();
    void decreaseScaleFromCurrentNode();
    
    
    /* Room Dimensions */
    void changeRoomWidthDimension(float pFactor);
    void changeRoomDepthDimension(float pFactor);
    void changeRoomHeightDimension(float pFactor);
    
    void drawNode(FurnitureNode* pNode);
    void draw();
    
    
};

#endif /* FurnitureManager_hpp */
