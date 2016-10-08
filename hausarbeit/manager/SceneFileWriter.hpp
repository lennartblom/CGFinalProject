//
//  SceneFileWriter.hpp
//  hausarbeit
//
//  Created by Lennart Blom on 13.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#ifndef SceneFileWriter_hpp
#define SceneFileWriter_hpp

#include <stdio.h>
#include "FurnitureNode.hpp"
#include "RoomDimension.hpp"
#include <iostream>
#include <fstream>

///
/// Helpclass for saving the current scene-arrangement
///
class SceneFileWriter{
    
public:
    void writeSceneInFile(FurnitureNode& pRoot, RoomDimension &pRoom, char *pFileName);
    void writeObject(std::ofstream &sceneFStream, FurnitureNode *pNode);
    void writeRoomDimensions(std::ofstream &sceneFStream, RoomDimension &pRoom);
    void writeBracketOpen(std::ofstream &sceneFStream);
    void writeTabulator(std::ofstream &sceneFStream);
    void writeBracketClosed(std::ofstream &sceneFStream);
};

#endif /* SceneFileWriter_hpp */
