//
//  MoveObjectWithMouseController.cpp
//  hausarbeit
//
//  Created by Lennart Blom on 15.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#include "MoveObjectWithMouseController.hpp"


MoveObjectWithMouseController::MoveObjectWithMouseController(){
    this->lastXMousePos = -1;
    this->lastYMousePos = -1;
}

MoveObjectWithMouseController::MoveObjectWithMouseController(unsigned int pX, unsigned int pY){
    this->lastXMousePos = pX;
    this->lastYMousePos = pY;
}

void MoveObjectWithMouseController::setLastXMousePos(unsigned int pX){
    this->lastXMousePos = pX;
}
void MoveObjectWithMouseController::setLastYMousePos(unsigned int pY){
    this->lastYMousePos = pY;
}

unsigned int MoveObjectWithMouseController::getLastXMousePos() const{
    return this->lastXMousePos;
}
unsigned int MoveObjectWithMouseController::getLastYMousePos() const{
    return this->lastYMousePos;
}

bool MoveObjectWithMouseController::coordsAreSet() const{
    return (this->lastXMousePos > -1 && this->lastYMousePos > -1);
}

