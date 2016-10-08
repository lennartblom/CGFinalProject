//
//  RGBImage.hpp
//  cg_prak_1
//
//  Created by Lennart Blom on 27.09.15.
//  Copyright © 2015 Lennart Blom. All rights reserved.
//

#ifndef RGBImage_hpp
#define RGBImage_hpp

#include <stdio.h>
#include "Color.hpp"
#include <iostream>
#include <fstream>

class RGBImage{
public:
    RGBImage(unsigned int Width, unsigned Height);
    ~RGBImage();
    void setPixelColor(unsigned int x, unsigned y, const Color& c);
    const Color& getPixelColor(unsigned int x, unsigned y) const;
    bool saveToDisk(const char* Filename);
    unsigned int width() const;
    unsigned int height() const;
    float getGrayScaleValue(unsigned int x, unsigned int y) const;
    
    static unsigned char convertColorCHannel(float f);
protected:
    Color* m_Image;
    unsigned int m_Height;
    unsigned int m_Width;
};





#endif /* RGBImage_hpp */
