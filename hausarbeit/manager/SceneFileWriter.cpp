//
//  SceneFileWriter.cpp
//  hausarbeit
//
//  Created by Lennart Blom on 13.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#include "SceneFileWriter.hpp"

void SceneFileWriter::writeSceneInFile(FurnitureNode &pRoot, RoomDimension &pRoom, char *pFileName){
    
    std::ofstream myFile;
    
    myFile.open(pFileName);
    
    this->writeRoomDimensions(myFile, pRoom);
    
    typedef std::set<FurnitureNode*>::iterator it_type;
    
    for(it_type i = pRoot.getChildren().begin();
        i != pRoot.getChildren().end(); i++){
        
        FurnitureNode* element = *i;
        
        this->writeObject(myFile, element);
        
    }
    
    myFile.close();
}

///
/// Saving of one furnitureObject and it's children
///
void SceneFileWriter::writeObject(std::ofstream &sceneFStream, FurnitureNode *pNode){
    sceneFStream << "object " << pNode->getName() << "\n";
    this->writeBracketOpen(sceneFStream);
    
    this->writeTabulator(sceneFStream);
    Vector nodePosition = pNode->getPosition();
    sceneFStream << "translation " << nodePosition.getX() << " " << nodePosition.getY() << " " << nodePosition.getZ() << "\n";
    
    this->writeTabulator(sceneFStream);
    sceneFStream << "rotation 0 1 0 " << pNode->getRotation() << "\n";
    
    Vector scaling = pNode->getScaling();
    this->writeTabulator(sceneFStream);
    sceneFStream << "scaling " << scaling.getX() << " " << scaling.getY() << " " << scaling.getZ() << "\n";
    
    this->writeTabulator(sceneFStream);
    sceneFStream << "model " << pNode->getModelName() << "\n";
    
    this->writeTabulator(sceneFStream);
    
    if(pNode->getParent() == NULL)
        sceneFStream << "parent " << "NULL" << "\n";
    else{
        if(pNode->getParent()->getName() == "Boden")
            sceneFStream << "parent " << "NULL" << "\n";
        else
            sceneFStream << "parent " << pNode->getParent()->getName() << "\n";
    }
    
    this->writeBracketClosed(sceneFStream);
    
    if(pNode->getChildren().size() > 0){
        typedef std::set<FurnitureNode*>::iterator it_type;
        
        for(it_type i = pNode->getChildren().begin();
            i != pNode->getChildren().end(); i++){
            
            FurnitureNode* element = *i;
            
            this->writeObject(sceneFStream, element);
            
        }
    }
    
    
}

///
/// Saving the roomDimensions
///
void SceneFileWriter::writeRoomDimensions(std::ofstream &sceneFStream, RoomDimension &pRoom){
    sceneFStream << "room\n";
    this->writeBracketOpen(sceneFStream);
    
    this->writeTabulator(sceneFStream);
    sceneFStream << "size " << pRoom.getWidth() << " " << pRoom.getDepth() << " " << pRoom.getHeight() << "\n";
    
    
    // Saving the wall-colors
    int colors[4];
    pRoom.returnWallColors(colors);
    
    this->writeTabulator(sceneFStream);
    sceneFStream << "colors " << colors[0] << " " << colors[1] << " " << colors[2] << " " << colors[3] << "\n";
    
    this->writeTabulator(sceneFStream);
    sceneFStream << "wallpaper\n";
    
    this->writeTabulator(sceneFStream);
    this->writeBracketOpen(sceneFStream);
    
    this->writeTabulator(sceneFStream);
    this->writeTabulator(sceneFStream);
    sceneFStream << "texture textures/tapete2.bmp\n";
    
    this->writeTabulator(sceneFStream);
    this->writeTabulator(sceneFStream);
    sceneFStream << "tiling 3 1\n";
    
    this->writeTabulator(sceneFStream);
    this->writeBracketClosed(sceneFStream);
    
    this->writeTabulator(sceneFStream);
    sceneFStream << "floor\n";
    
    this->writeTabulator(sceneFStream);
    this->writeTabulator(sceneFStream);
    sceneFStream << "texture textures/floor.bmp\n";
    
    this->writeTabulator(sceneFStream);
    this->writeTabulator(sceneFStream);
    sceneFStream << "tiling 3 1\n";
    
    this->writeTabulator(sceneFStream);
    this->writeBracketClosed(sceneFStream);
    
    
    this->writeBracketClosed(sceneFStream);
    
}

void SceneFileWriter::writeBracketOpen(std::ofstream &sceneFStream){
    sceneFStream << "{\n";
}
void SceneFileWriter::writeTabulator(std::ofstream &sceneFStream){
    sceneFStream << "\t";
}

void SceneFileWriter::writeBracketClosed(std::ofstream &sceneFStream){
    sceneFStream << "}\n";
}
