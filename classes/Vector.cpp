//
//  Vector.cpp
//  cg_prak_1
//
//  Created by Lennart Blom on 27.09.15.
//  Copyright © 2015 Lennart Blom. All rights reserved.
//

#include "Vector.hpp"


Vector::Vector(float x, float y, float z){
    this->X = x;
    this->Y = y;
    this->Z = z;
}

Vector::Vector(){
    
}




float Vector::dot(const Vector& v) const{
    return ((this->X*v.getX())+(this->Y*v.getY())+(this->Z*v.getZ()));
}

Vector Vector::cross(const Vector& v) const{
    float x,y,z;
    
    x = ((this->Y*v.getZ())-(this->Z*v.getY()));
    y = ((this->Z*v.getX())-(this->X*v.getZ()));
    z = ((this->X*v.getY())-(this->Y*v.getX()));
    
    return Vector(x,y,z);;
}

Vector Vector::operator+(const Vector& v) const{
    float x,y,z;
    
    x =(this->X + v.getX());
    y =(this->Y + v.getY());
    z =(this->Z + v.getZ());
    
    return Vector(x,y,z);
}

Vector Vector::operator-(const Vector& v) const{
    float x,y,z;
    
    x =(this->X - v.getX());
    y =(this->Y - v.getY());
    z =(this->Z - v.getZ());
    
    return Vector(x,y,z);
}

Vector& Vector::operator+=(const Vector& v){
    this->X =(this->X + v.getX());
    this->Y =(this->Y + v.getY());
    this->Z =(this->Z + v.getZ());
    
    return *this;
}

Vector Vector::operator*(float c) const{
    float x,y,z;
    
    x =(this->X * c);
    y =(this->Y * c);
    z =(this->Z * c);
    
    return Vector(x,y,z);
}

Vector Vector::operator-() const{
    float x,y,z;
    
    x =(this->X * -1);
    y =(this->Y * -1);
    z =(this->Z * -1);
    
    return Vector(x,y,z);
}

Vector& Vector::normalize(){
    float normFactor = this->length();
    
    
    normFactor = 1/normFactor;
    
    *this = (*this)*normFactor;
    
    // ToDO
    
    return *this;
}

float Vector::length() const{
    return sqrt(lengthSquared());
}

float Vector::lengthSquared() const{
    return (pow(this->X, 2)+pow(this->Y, 2) + pow(this->Z, 2));
}

Vector Vector::reflection(const Vector & normal) const{
    float x,y,z;
    
    x = this->X - 2*(this->X*normal.X)*normal.X;
    y = this->Y - 2*(this->Y*normal.Y)*normal.Y;
    z = this->Z - 2*(this->Z*normal.Z)*normal.Z;

    return Vector(x,y,z);
}

Vector Vector::getIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const {
    Vector n = (b-a).cross(c-a).normalize();
    float distance = n.dot(a);
    float no = n.dot(*this);
    
    float nd = n.dot(d);
    
    s = (distance - no) / nd;
    
    Vector p = *this + (d*s);
    return p;
}

bool Vector::triangleIntersection(const Vector& d, const Vector& a, const Vector & b, const Vector& c, float& s) const{
    /* Schnittpunkt mit der Ebene bestimmen in der das Dreieck liegt */
    
    Vector numerator = (b-a).cross(c-a);
    
    float dividor = (b-a).cross(c-a).length();
    
    Vector n = numerator*(1/dividor);
    
    
    float ed = (n.dot(a));
    
    
    Vector o = (*this);
    s = (ed-(n.dot(o))) / (n.dot(d));
    
    
    if(s<0){
        return false;
    }
    
    Vector p = o + d*(s);
    
    float surfaceABC, surfaceABP, surfaceACP, surfaceBCP;
    
    surfaceABC = ((((b-a).cross(c-a))).length())*(0.5);
    surfaceABP = ((((b-a).cross(p-a))).length())*(0.5);
    surfaceACP = ((((c-a).cross(p-a))).length())*(0.5);
    surfaceBCP = ((((c-b).cross(p-b))).length())*(0.5);
    
    float result = (surfaceABC - surfaceABP - surfaceACP - surfaceBCP);
    
    if(result >=-0.00001){
        return true;
    }else{
        return false;
    }
}



