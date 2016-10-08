//
//  Color.hpp
//  cg_prak_1
//
//  Created by Lennart Blom on 27.09.15.
//  Copyright © 2015 Lennart Blom. All rights reserved.
//

#ifndef Color_hpp
#define Color_hpp

#include <stdio.h>

class Color{
public:
    float R;
    float G;
    float B;
    
    Color();
    Color(float r, float g, float b);
    
    Color operator*(const Color& c) const;
    Color operator*(const float Factor) const;
    Color operator+(const Color& c) const;
    Color& operator+=(const Color&c);
    
};

#endif /* Color_hpp */
