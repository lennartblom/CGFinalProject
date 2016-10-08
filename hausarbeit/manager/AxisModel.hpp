//
//  AxisModel.hpp
//  hausarbeit
//
//  Created by Lennart Blom on 15.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#ifndef AxisModel_hpp
#define AxisModel_hpp

#include <stdio.h>
#include "Model.h"
#include "matrix.h"
#include "ShaderProgram.h"

class AxisModel{
    
private:
    Model *arrow;
    Matrix scale;
    
    ShaderProgram *spColor;
    Matrix tranformXAxis;
    Matrix transformZAxis;
    
public:
    AxisModel();
    void draw(const Vector& globalPosition, const float pRotation);
    int checkIfAxisIsHit(const Vector& globalPosition, const Vector &o, const Vector &d);
    
};

#endif /* AxisModel_hpp */
