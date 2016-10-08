//
//  Model.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Model__
#define __RealtimeRending__Model__

#include <iostream>
#include "Vector.hpp"
#include "Color.hpp"
#include <string>
#include <map>
#include <vector>
#include "material.h"
#include "ShaderProgram.h"


struct ModelVertex{
    Vector Pos;
    Vector Normal;
    float u0,v0;
};

struct Vertex
{
    Vertex();
    Vertex( const Vector& p, const Vector& n, float TexS, float TexT);
    Vector Position;
    Vector Normal;
    float  TexcoordS;
    float  TexcoordT;
};

struct Triangle
{
    Triangle();
    Triangle(Vertex a, Vertex b, Vertex c);
    Vertex a, b, c;
};

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox( const Vector& min, const Vector& max);
    void initVertices();
    void initTriangles();
    Vector Min;
    Vector Max;
    Vertex m_pVertices[8];
    Triangle triangles[12];
};
typedef std::map<std::string, std::vector<unsigned int>> textureMap;

/*
 
 Klassenimplementierung von Torben Höhn.
 Es gab Probleme mit der eigenen Klasse aus dem Praktikum bei der Umstellung auf die 
 Darstellung über Vertex- und Indexbuffer.
 
 1. Normalen wurde nicht korrekt berechnet.
 2. Speicherfehler beim Starten des Projekts (jeder 2./3. Startvorgang)
 
 Um nicht viel mehr Zeit zu verlieren, wurde eine externe Klasse genutzt um sich auf die
 Software-Features zu konzentriert...
 
 */
class Model
{
public:
    Model();
    ~Model();
    const BoundingBox& boundingBox() const;
    
    bool loadMaterialFile(const char* Filename);
    bool loadMaterial(std::fstream &osh);
    
    void calcBbox(std::vector<Vector> vertices);
    void vCalcBox(std::vector<ModelVertex> mvList);
    Vertex* getM_pVertices() const;
    void setSize(int size);
    int getSize();
    unsigned int getVertexCount() const;
    
    //VertexBufferMethoden
    bool vLoad(const char* Filename, bool fitSize, const char* bmp);
    void vDraw(bool Boundingbox) const;
    void vDrawWall(unsigned int &material) const;
    void vDrawRoom() const;
    void DrawBox() const;
    bool BBoxIntersection(Vector o, Vector d, Vector *Intersection, Matrix Transform) const;
    
    Vector getCenter(Matrix Transform) const;
    std::vector<Vector> getCorners(Matrix Transform) const;
    void updateDraw(ShaderProgram *sp) const;
    
    textureMap texMap;
protected:
    void createModel(const char* Filename, bool fitSize);
    Material* m_pMaterials;
    unsigned int m_MaterialCount;
    Vertex* m_pVertices;
    Vertex* m_pIndices;
    int vSize;
    unsigned int m_VertexCount;
    BoundingBox m_Box;
    //VertexBuffer Variablen
    GLuint m_VertexBuffer;
    GLuint m_IndexBuffer;
    std::vector<ModelVertex> Vertices;
    unsigned int Indices[180];
    unsigned int IndexCount;
    //Texturen
    Texture tex;
    unsigned int texCount;
};

#endif /* defined(__RealtimeRending__Model__) */
