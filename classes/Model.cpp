#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

#include "Model.h"
#include <vector>
#include <assert.h>
#include <math.h>
#include <map>
#include <float.h>
#include <stdio.h>
#include "ShaderProgram.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Standardkonstruktor
Vertex::Vertex()
{
    
}

// Erweiteter Konstruktor
Vertex::Vertex( const Vector& p, const Vector& n, float TexS, float TexT)
{
    Position = p;
    Normal = n;
    TexcoordS = TexS;
    TexcoordT = TexT;
}

// Standardkonstruktor
Triangle::Triangle(){
    
}

// Erweiteter Konstruktor
Triangle::Triangle(Vertex a, Vertex b, Vertex c){
    this->a = a;
    this->b = b;
    this->c = c;
}

// Standardkonstruktor
BoundingBox::BoundingBox()
{
    
}

// Erweiteter Konstruktor
BoundingBox::BoundingBox( const Vector& min, const Vector& max) : Min(min), Max(max){
}

void BoundingBox::initVertices(){
    Vertex a;
    a.Position = Vector(Min.X, Min.Y, Min.Z);
    m_pVertices[0]= a;
    Vertex b;
    b.Position = Vector(Max.X, Min.Y, Min.Z);
    m_pVertices[1]= b;
    Vertex c;
    c.Position = Vector(Min.X, Max.Y, Min.Z);
    m_pVertices[2]= c;
    Vertex d;
    d.Position = Vector(Max.X, Max.Y, Min.Z);
    m_pVertices[3]= d;
    Vertex e;
    e.Position = Vector(Min.X, Min.Y, Max.Z);
    m_pVertices[4]= e;
    Vertex f;
    f.Position = Vector(Max.X, Min.Y, Max.Z);
    m_pVertices[5]= f;
    Vertex g;
    g.Position = Vector(Min.X, Max.Y, Max.Z);
    m_pVertices[6]= g;
    Vertex h;
    h.Position = Vector(Max.X, Max.Y, Max.Z);
    m_pVertices[7]= h;
}

void BoundingBox::initTriangles(){
    triangles[0] = Triangle(m_pVertices[0], m_pVertices[1], m_pVertices[2]);
    triangles[1] = Triangle(m_pVertices[1], m_pVertices[2], m_pVertices[3]);
    triangles[2] = Triangle(m_pVertices[0], m_pVertices[4], m_pVertices[6]);
    triangles[3] = Triangle(m_pVertices[0], m_pVertices[6], m_pVertices[2]);
    triangles[4] = Triangle(m_pVertices[4], m_pVertices[5], m_pVertices[7]);
    triangles[5] = Triangle(m_pVertices[4], m_pVertices[7], m_pVertices[6]);
    triangles[6] = Triangle(m_pVertices[5], m_pVertices[1], m_pVertices[3]);
    triangles[7] = Triangle(m_pVertices[5], m_pVertices[3], m_pVertices[7]);
    triangles[8] = Triangle(m_pVertices[3], m_pVertices[2], m_pVertices[6]);
    triangles[9] = Triangle(m_pVertices[3], m_pVertices[6], m_pVertices[7]);
    triangles[10] = Triangle(m_pVertices[0], m_pVertices[1], m_pVertices[5]);
    triangles[11] = Triangle(m_pVertices[0], m_pVertices[5], m_pVertices[4]);
}

void Model::DrawBox() const{
    for (int i = 0; i <12; i++) {
        glBegin(GL_LINE_STRIP);
        glVertex3f(m_Box.triangles[i].a.Position.X, m_Box.triangles[i].a.Position.Y, m_Box.triangles[i].a.Position.Z);
        glVertex3f(m_Box.triangles[i].b.Position.X, m_Box.triangles[i].b.Position.Y, m_Box.triangles[i].b.Position.Z);
        glVertex3f(m_Box.triangles[i].c.Position.X, m_Box.triangles[i].c.Position.Y, m_Box.triangles[i].c.Position.Z);
        glVertex3f(m_Box.triangles[i].a.Position.X, m_Box.triangles[i].a.Position.Y, m_Box.triangles[i].a.Position.Z);
        glEnd();
    }
    glFlush();
}

bool Model::BBoxIntersection(Vector o, Vector d, Vector *Intersection, Matrix Transform) const{
    for (int i = 0; i<12; i++) {
        Vector a = m_Box.triangles[i].a.Position;
        Vector b = m_Box.triangles[i].b.Position;
        Vector c = m_Box.triangles[i].c.Position;
        a = Transform.transformVec4x4(a);
        b = Transform.transformVec4x4(b);
        c = Transform.transformVec4x4(c);
        float s;
        if (o.triangleIntersection(d, a, b, c, s)) {
            *Intersection = o.getIntersection(d, a, b, c, s);
            return true;
        }
    }
    return false;
}

// Standardkonstruktor
Model::Model() : m_pVertices(NULL), m_MaterialCount(0), m_VertexCount(0), IndexCount(0), Vertices(0)
{
    m_pMaterials = new Material[20];
    
}

// Destruktor
Model::~Model()
{
    if( m_pVertices)
        delete [] m_pVertices;
    if(m_pMaterials)
        delete [] m_pMaterials;
}

// getter
int Model::getSize(){
    return  this->vSize;
}

const BoundingBox& Model::boundingBox() const
{
    return m_Box;
}

// setter
void Model::setSize(int size){
    this->vSize = size;
}

// Lädt Materialfile
bool Model::loadMaterialFile(const char* Filename){
    std::fstream osh(Filename, std::ios::in);
    std::string noun, junk;
    if (osh.good()) {
        while (osh >> noun) {
            if (noun == "newmtl") {
                loadMaterial(osh);
                m_MaterialCount++;
            }
        }
        osh.close();
    }
    return true;
}

// Lädt einzelne Materialien
bool Model::loadMaterial(std::fstream &osh){
    Material m;
    std::string junk;
    std::string name;
    osh >> name;
    m.setName(name);
    osh >> junk;
    if (junk.compare("Ns") == 0) {
        float Ns;
        osh >> Ns;
        m.setSpecularExponent(Ns);
        osh >> junk;
    }
    if (junk.compare("Ka") == 0) {
        float Ka1, Ka2, Ka3;
        osh >> Ka1 >> Ka2 >> Ka3;
        Color ka = Color(Ka1, Ka2, Ka3);
        m.setAmbientColor(ka);
        osh >> junk;
    }
    if (junk.compare("Kd") == 0) {
        float Kd1, Kd2, Kd3;
        osh >> Kd1 >> Kd2 >> Kd3;
        Color kd = Color(Kd1, Kd2, Kd3);
        m.setDiffuseColor(kd);
        osh >> junk;
    }
    if (junk.compare("Ks") == 0) {
        float Ks1, Ks2, Ks3;
        osh >> Ks1 >> Ks2 >> Ks3;
        Color ks = Color(Ks1, Ks2, Ks3);
        m.setSpecularColor(ks);
        osh >> junk;
    }
    if (junk.compare("Ni") == 0) {
        float Ni;
        osh >> Ni;
        osh >> junk;
    }
    if (junk.compare("d") == 0) {
        float d;
        osh >> d;
        osh >> junk;
    }
    if (junk.compare("illum") == 0) {
        int illum;
        osh >> illum;
        osh >> junk;
    }
    if (junk.compare("map_Kd") == 0) {
        std::string map_Kd;
        osh >> map_Kd;
        m.setDiffuseTexture(map_Kd.c_str());
    }
    m_pMaterials[m_MaterialCount] = m;
    return true;
}

Vertex* Model::getM_pVertices() const
{
    return this->m_pVertices;
}

unsigned int Model::getVertexCount() const{
    return this->m_VertexCount;
}

struct triangle{
    int a;
    int b;
    int c;
    Vector normal;
};

// Lädt Objektdatei
bool Model::vLoad(const char* Filename, bool fitSize, const char* bmp){
    FILE* obj = fopen(Filename, "r");
    if(obj == NULL){
        printf("File konnte nicht geoeffnet werden");
        return false;
    }
    char usemtl[30];
    std::cout << Filename << " wird geladen" << std::endl;
    std::vector<ModelVertex> mvList;
    std::vector<unsigned int> inList;
    std::vector<unsigned int> inListHelper;
    std::vector<triangle> triangles;
    int counter = 0;
    while( 1 ){
        char lineHeader[128];
        int res = fscanf(obj, "%s", lineHeader);
        if (res == EOF) break; // else : parse lineHeader
        if (strcmp(lineHeader, "mtllib") == 0) {
            char mtllib[30];
            fscanf(obj, "%s", mtllib);
            this->loadMaterialFile(mtllib);
        } else if ( strcmp( lineHeader, "v" ) == 0 ){
            ModelVertex mv;
            Vector Position;
            fscanf(obj, "%f %f %f\n", &Position.X, &Position.Y, &Position.Z);
            Position.X = Position.X;
            Position.Y = Position.Y;
            Position.Z = Position.Z;
            mv.Pos = Vector(Position.X, Position.Y, Position.Z);
            mv.Normal = Vector(0, 0, 0);
            mv.u0 = 10;
            mv.v0 = 10;
            mvList.push_back(mv);
        } else if (strcmp(lineHeader, "usemtl") == 0){
            if (counter > 0) {
                texMap[usemtl] = inListHelper;
                inListHelper.clear();
            }
            fscanf(obj, "%s\n", usemtl);
        } else if ( strcmp( lineHeader, "f" ) == 0 ){
            unsigned int vertexIndex[4], texIndex[4];
            int matches = fscanf(obj, "%d/%d %d/%d %d/%d %d/%d\n", &vertexIndex[0], &texIndex[0], &vertexIndex[1], &texIndex[1], &vertexIndex[2], &texIndex[2], &vertexIndex[3], &texIndex[3]);
            if (matches == 6){
                inList.push_back(vertexIndex[0]-1);
                inList.push_back(vertexIndex[1]-1);
                inList.push_back(vertexIndex[2]-1);
                inListHelper.push_back(vertexIndex[0]-1);
                inListHelper.push_back(vertexIndex[1]-1);
                inListHelper.push_back(vertexIndex[2]-1);
                triangle t;
                t.a = vertexIndex[0]-1;
                t.b = vertexIndex[1]-1;
                t.c = vertexIndex[2]-1;
                Vector a =mvList[t.a].Pos;
                Vector b= mvList[t.b].Pos;
                Vector c= mvList[t.c].Pos;
                Vector BA = b-a;
                Vector CA = c-a;
                Vector normal = BA.cross(CA);
                normal.normalize();
                t.normal = normal;
                triangles.push_back(t);
            } else if(matches == 8) {
                inList.push_back(vertexIndex[0]-1);
                inList.push_back(vertexIndex[1]-1);
                inList.push_back(vertexIndex[2]-1);
                inListHelper.push_back(vertexIndex[0]-1);
                inListHelper.push_back(vertexIndex[1]-1);
                inListHelper.push_back(vertexIndex[2]-1);
                triangle t;
                t.a = vertexIndex[0]-1;
                t.b = vertexIndex[1]-1;
                t.c = vertexIndex[2]-1;
                Vector a =mvList[t.a].Pos;
                Vector b= mvList[t.b].Pos;
                Vector c= mvList[t.c].Pos;
                Vector BA = b-a;
                Vector CA = c-a;
                Vector normal = BA.cross(CA);
                normal.normalize();
                t.normal = normal;
                triangles.push_back(t);
                inList.push_back(vertexIndex[2]-1);
                inList.push_back(vertexIndex[3]-1);
                inList.push_back(vertexIndex[0]-1);
                inListHelper.push_back(vertexIndex[2]-1);
                inListHelper.push_back(vertexIndex[3]-1);
                inListHelper.push_back(vertexIndex[0]-1);
                t.a = vertexIndex[2]-1;
                t.b = vertexIndex[3]-1;
                t.c = vertexIndex[0]-1;
                a =mvList[t.a].Pos;
                b= mvList[t.b].Pos;
                c= mvList[t.c].Pos;
                BA = b-a;
                CA = c-a;
                
                normal = BA.cross(CA);
                normal.normalize();
                t.normal = normal;
                triangles.push_back(t);
            }
            counter++;
        }
    }
    texMap[usemtl] = inListHelper;
    vCalcBox(mvList);
    if(fitSize){
        float scale = 2/((m_Box.Min - m_Box.Max).length());
        for(int i = 0; i < mvList.size(); i++){
            mvList[i].Pos = mvList[i].Pos * scale;
        }
        vCalcBox(mvList);
    }
    for(int i = 0; i<triangles.size(); i++){
        mvList[triangles[i].a].Normal += triangles[i].normal;
        mvList[triangles[i].b].Normal += triangles[i].normal;
        mvList[triangles[i].c].Normal += triangles[i].normal;
    }
    for(int i = 0; i<mvList.size();i++){
        mvList[i].Normal.normalize();
    }
    IndexCount = inList.size();
    
    // create gpu buffer for vertices
    glGenBuffers( 1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ModelVertex)*mvList.size(), &mvList[0], GL_STATIC_DRAW );
    // create gpu buffer for indices
    glGenBuffers( 1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*inList.size(), &inList[0], GL_STATIC_DRAW);
    
    if(!tex.LoadFromBMP(bmp)) return false;
    
    return true;
}


void Model::vDrawWall(unsigned int &material) const{
    if(material < m_MaterialCount){
        
    
    // inform the client that we want to use array buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    // setup position & normal pointers
    glVertexPointer( 3, GL_FLOAT, sizeof(ModelVertex), BUFFER_OFFSET(0));
    glNormalPointer( GL_FLOAT, sizeof(ModelVertex), BUFFER_OFFSET(sizeof(Vector)));
    
    float diff[4];
    float amb[4];
    float spec[4];
    
    for (auto it = texMap.begin(); it != texMap.end(); ++it) {
                diff[0] = m_pMaterials[material].getDiffuseColor().R;
                diff[1] = m_pMaterials[material].getDiffuseColor().G;
                diff[2] = m_pMaterials[material].getDiffuseColor().B;
                diff[3] = 1;
                
                amb[0] = m_pMaterials[material].getAmbientColor().R;
                amb[1] = m_pMaterials[material].getAmbientColor().G;
                amb[2] = m_pMaterials[material].getAmbientColor().B;
                amb[3] = 1;
                
                spec[0] = m_pMaterials[material].getSpecularColor().R;
                spec[1] = m_pMaterials[material].getSpecularColor().G;
                spec[2] = m_pMaterials[material].getSpecularColor().B;
                spec[3] = 1;
                
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
                glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
                glMateriali(GL_FRONT, GL_SHININESS, m_pMaterials[material].getSpecularExponent());
                glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
        
    }
    
    glDrawElements( GL_TRIANGLES, IndexCount , GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    
    float defDiff[4] = {1,1,1,1};
    float defAmb[4]  = {0.2f,0.2f,0.2f,1};
    float defSpec[4] = {0.5f,0.5f,0.5f,1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, defDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, defSpec);
    glMateriali(GL_FRONT, GL_SHININESS, 30);
    glMaterialfv(GL_FRONT, GL_AMBIENT, defAmb);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }else{
        vDraw(false);
    }
}

// Zeichnet Objekt via GPU
void Model::vDraw(bool Boundingbox) const{
    if(Boundingbox) DrawBox();
    // inform the client that we want to use array buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    // setup position & normal pointers
    glVertexPointer( 3, GL_FLOAT, sizeof(ModelVertex), BUFFER_OFFSET(0));
    glNormalPointer( GL_FLOAT, sizeof(ModelVertex), BUFFER_OFFSET(sizeof(Vector)));
    
    float diff[4];
    float amb[4];
    float spec[4];
    
    for (auto it = texMap.begin(); it != texMap.end(); ++it) {
        for (int i = 0; i < m_MaterialCount; i++) {
            if (it->first.compare(m_pMaterials[i].getName()) == 0) {
                diff[0] = m_pMaterials[i].getDiffuseColor().R;
                diff[1] = m_pMaterials[i].getDiffuseColor().G;
                diff[2] = m_pMaterials[i].getDiffuseColor().B;
                diff[3] = 1;
                
                amb[0] = m_pMaterials[i].getAmbientColor().R;
                amb[1] = m_pMaterials[i].getAmbientColor().G;
                amb[2] = m_pMaterials[i].getAmbientColor().B;
                amb[3] = 1;
                
                spec[0] = m_pMaterials[i].getSpecularColor().R;
                spec[1] = m_pMaterials[i].getSpecularColor().G;
                spec[2] = m_pMaterials[i].getSpecularColor().B;
                spec[3] = 1;
                
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
                glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
                glMateriali(GL_FRONT, GL_SHININESS, m_pMaterials[i].getSpecularExponent());
                glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
            }
        }
    }
    glDrawElements( GL_TRIANGLES, IndexCount , GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    
    float defDiff[4] = {1,1,1,1};
    float defAmb[4]  = {0.2f,0.2f,0.2f,1};
    float defSpec[4] = {0.5f,0.5f,0.5f,1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, defDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, defSpec);
    glMateriali(GL_FRONT, GL_SHININESS, 30);
    glMaterialfv(GL_FRONT, GL_AMBIENT, defAmb);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::vDrawRoom() const{
    // inform the client that we want to use array buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    // setup position & normal pointers
    glVertexPointer( 3, GL_FLOAT, sizeof(ModelVertex), BUFFER_OFFSET(0));
    glNormalPointer( GL_FLOAT, sizeof(ModelVertex), BUFFER_OFFSET(sizeof(Vector)));
    
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(4, GL_FLOAT, sizeof(ModelVertex), BUFFER_OFFSET(sizeof(Vector)*2)); // first uv-set
    
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    tex.apply();
    
    glDrawElements( GL_TRIANGLES, IndexCount , GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
    
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::vCalcBox(std::vector<ModelVertex> mvList){
    m_Box.Max.X = mvList[0].Pos.X;
    m_Box.Max.Y = mvList[0].Pos.Y;
    m_Box.Max.Z = mvList[0].Pos.Z;
    m_Box.Min.X = mvList[0].Pos.X;
    m_Box.Min.Y = mvList[0].Pos.Y;
    m_Box.Min.Z = mvList[0].Pos.Z;
    for(ModelVertex mv : mvList){
        Vector vertex = mv.Pos;
        if(m_Box.Min.X > vertex.X) m_Box.Min.X = vertex.X;
        if(m_Box.Min.Y > vertex.Y) m_Box.Min.Y = vertex.Y;
        if(m_Box.Min.Z > vertex.Z) m_Box.Min.Z = vertex.Z;
        if(m_Box.Max.X < vertex.X) m_Box.Max.X = vertex.X;
        if(m_Box.Max.Y < vertex.Y) m_Box.Max.Y = vertex.Y;
        if(m_Box.Max.Z < vertex.Z) m_Box.Max.Z = vertex.Z;
    }
    m_Box.initVertices();
    m_Box.initTriangles();
}

void Model::updateDraw(ShaderProgram *sp) const
{
    // inform the client that we want to use array buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    // setup position & normal pointers
    glVertexPointer( 3, GL_FLOAT, sizeof(ModelVertex), BUFFER_OFFSET(0));
    glNormalPointer( GL_FLOAT, sizeof(ModelVertex), BUFFER_OFFSET(sizeof(Vector)));
    
    // we draw our plane
    sp->activate();
    glDrawElements( GL_TRIANGLES, IndexCount , GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
    sp->deactivate();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// gibt Mittelpunkt des Objekts zurück
Vector Model::getCenter(Matrix Transform) const{
    Vector max = Transform * m_Box.Max;
    Vector min = Transform * m_Box.Min;
    float x = (max.X + min.X) / 2;
    float z = (max.Z + min.Z) / 2;
    float y = max.Y;//(max.Y + min.Y) / 2;
    return Vector(x, y, z);
}

// gibt Eckpunkte des Objekts zurück
std::vector<Vector> Model::getCorners(Matrix Transform) const{
    Vector max = Transform * m_Box.Max;
    Vector min = Transform * m_Box.Min;
    std::vector<Vector> corners;
    corners.push_back(Vector(min.X, max.Y, min.Z));
    corners.push_back(Vector(min.X, max.Y, max.Z));
    corners.push_back(Vector(max.X, max.Y, min.Z));
    corners.push_back(Vector(max.X, max.Y, max.Z));
    return corners;
}
