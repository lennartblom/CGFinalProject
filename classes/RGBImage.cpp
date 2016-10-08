//
//  RGBImage.cpp
//  cg_prak_1
//
//  Created by Lennart Blom on 27.09.15.
//  Copyright © 2015 Lennart Blom. All rights reserved.
//

#include "RGBImage.hpp"

RGBImage::RGBImage(unsigned int Width, unsigned Height){
    
    this->m_Height = Height;
    this->m_Width = Width;
    this->m_Image = new Color[Width*Height];
}

RGBImage::~RGBImage(){
    //delete [] m_Image;
}

void RGBImage::setPixelColor(unsigned int x, unsigned y, const Color& c){
    this->m_Image[this->width()*y+x] = c;
}

const Color& RGBImage::getPixelColor(unsigned int x, unsigned y) const{
    return this->m_Image[this->width()*y+x];
}


float RGBImage::getGrayScaleValue(unsigned int x, unsigned int y) const{
    Color tmp = this->getPixelColor(x,y);
    //return 0.30*tmp.R + 0.59*tmp.G + 0.11*tmp.B)
    //std::cout << "2.: " << (0.30*tmp.R + 0.59*tmp.G + 0.11*tmp.B)*6 << std::endl;
    return tmp.R;
}

bool RGBImage::saveToDisk(const char* Filename){
    
    typedef struct{
        unsigned int   bfSize;
        unsigned short bfReserved1;
        unsigned short bfReserved2;
        unsigned int   bfOffBits;
    } BITMAPFILEHEADER;
    
    typedef struct {
        unsigned int   biSize;
        int            biWidth;
        int            biHeight;
        unsigned short biPlanes;
        unsigned short biBitCount;
        unsigned int   biCompression;
        unsigned int   biSizeImage;
        int            biXPelsPerMeter;
        int            biYPelsPerMeter;
        unsigned int   biClrUsed;
        unsigned int   biClrImportant;
    } BITMAPINFOHEADER;
    
    
    unsigned short bfType = 19778;
    
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfSize = 2+sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+this->m_Height*this->m_Width*3;
    fileHeader.bfOffBits = 0x36;
    
    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = this->m_Width;
    infoHeader.biHeight = this->m_Height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biCompression = 0;
    infoHeader.biSizeImage = 0;
    infoHeader.biXPelsPerMeter = 5000;
    infoHeader.biYPelsPerMeter = 5000;
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;
    
    FILE *file = fopen(Filename, "wb");
    
    
    fwrite(&bfType,1,sizeof(bfType),file);
    fwrite(&fileHeader,1,sizeof(fileHeader), file);
    fwrite(&infoHeader,1,sizeof(infoHeader), file);
    
    Color tmp;
    
    for (int y = this->m_Height-1; y>=0; y--) {
        for (int x = 0; x < this->m_Width; x++){
            tmp = this->getPixelColor(x, y);
            
            unsigned char r = convertColorCHannel(tmp.R);
            unsigned char g = convertColorCHannel(tmp.G);
            unsigned char b = convertColorCHannel(tmp.B);
            
            fwrite(&r, 1, 1, file);
            fwrite(&g, 1, 1, file);
            fwrite(&b, 1, 1, file);
        }
    }
    fclose(file);
    
    return true;
}

unsigned int RGBImage::width() const{
    return this->m_Width;
}

unsigned int RGBImage::height() const{
    return this->m_Height;
}

unsigned char RGBImage::convertColorCHannel(float f){
    if( f < 0.0 ){
        f = 0;
    }else if( f > 1.0 ){
        f = 1.0;
    }
    return f * 255;
}