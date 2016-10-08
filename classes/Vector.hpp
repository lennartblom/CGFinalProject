//
//  Vector.hpp
//  cg_prak_1
//
//  Created by Lennart Blom on 27.09.15.
//  Copyright © 2015 Lennart Blom. All rights reserved.
//

#ifndef Vector_hpp
#define Vector_hpp

#include <stdio.h>
#include <iostream>
#include <math.h>

class Vector{

public:
    float X;
    float Y;
    float Z;
    
    Vector(float x, float y, float z);
    Vector();
    
    float dot(const Vector& v) const;
    Vector cross(const Vector& v) const;
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector& operator+=(const Vector& v);
    Vector operator*(float c) const;
    Vector operator-() const;
    friend std::ostream& operator<<(std::ostream &strm, const Vector &a){
        return strm << "Vector(" << a.X << ", " << a.Y << ", " << a.Z<< ")";
    }
    
    Vector& normalize();
    float length() const;
    float lengthSquared() const;
    Vector getIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const;
    Vector reflection(const Vector & normal) const;
    bool triangleIntersection(const Vector& d, const Vector& a, const Vector & b, const Vector& c, float& s) const;
    
    void setX(float x) { X = x; }
    void setY(float y) { Y = y; }
    void setZ(float z) { Z = z; }
    float getX()const {return this->X;}
    float getY()const {return this->Y;}
    float getZ()const {return this->Z;}
    
};

#endif /* Vector_hpp */
