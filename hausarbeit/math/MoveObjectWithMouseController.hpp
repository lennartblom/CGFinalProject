//
//  MoveObjectWithMouseController.hpp
//  hausarbeit
//
//  Created by Lennart Blom on 15.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#ifndef MoveObjectWithMouseController_hpp
#define MoveObjectWithMouseController_hpp

#include <stdio.h>
#include <iostream>

class MoveObjectWithMouseController{
private:
    unsigned int lastXMousePos;
    unsigned int lastYMousePos;
    
public:
    MoveObjectWithMouseController();
    MoveObjectWithMouseController(unsigned int pX, unsigned int pY);
    void setLastXMousePos(unsigned int pX);
    void setLastYMousePos(unsigned int pY);
    
    unsigned int getLastXMousePos() const;
    unsigned int getLastYMousePos() const;
    
    bool coordsAreSet() const;
};

#endif /* MoveObjectWithMouseController_hpp */
