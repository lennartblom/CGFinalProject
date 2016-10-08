//
//  Color.cpp
//  cg_prak_1
//
//  Created by Lennart Blom on 27.09.15.
//  Copyright © 2015 Lennart Blom. All rights reserved.
//

#include "Color.hpp"

Color::Color(){
    this->R = 0.0;
    this->G = 0.0;
    this->B = 0.0;
}
Color::Color(float r, float g, float b){
    if (r > 1.0){
        r = 1.0;
    }else if(r < 0.0){
        r = 0.0;
    }
    if (g > 1.0){
        g = 1.0;
    }else if(g < 0.0){
        g = 0.0;
    }
    if (b > 1.0){
        b = 1.0;
    }else if(b < 0.0){
        b = 0.0;
    }
    this->R = r;
    this->G = g;
    this->B = b;
}
Color Color::operator*(const Color& c) const{
    float r = this->R * c.R;
    float g = this->G * c.G;
    float b = this->B * c.B;
    return Color(r,g,b);
}
Color Color::operator*(const float factor) const{
    float r = this->R * factor;
    float g = this->G * factor;
    float b = this->B * factor;
    return Color(r,g,b);
}
Color Color::operator+(const Color& c) const{
    float r = this->R + c.R;
    float g = this->G + c.G;
    float b = this->B + c.B;
    if (r > 1.0){
        r = 1.0;
    }
    if (g > 1.0){
        g = 1.0;
    }
    if (b > 1.0){
        b = 1.0;
    }
    return Color(r,g,b);
}
Color& Color::operator+=(const Color& c){
    float r = this->R + c.R;
    float g = this->G + c.G;
    float b = this->B + c.B;
    if (r > 1.0){
        r = 1.0;
    }
    if (g > 1.0){
        g = 1.0;
    }
    if (b > 1.0){
        b = 1.0;
    }
    this->R = r;
    this->G = g;
    this->B = b;
    return *this;
}
