//
//  FurnitureManager.cpp
//  hausarbeit
//
//  Created by Lennart Blom on 08.02.16.
//  Copyright © 2016 Lennart Blom. All rights reserved.
//

#include "FurnitureManager.hpp"
#include <sstream>


FurnitureManager::FurnitureManager(){
    this->currentFurnitureObject = NULL;
}

FurnitureManager::FurnitureManager(const char *sceneFile){
    
    
    this->currentFurnitureObject = NULL;
    
    /* Deactivating modes and other stuff */
    this->hierarchyMode = false;
    this->eraseHierarchyMode = false;
    this->moveAxes = false;
    this->deactivateAddTvMode();
    this->deactivateChanceRoomGeometryMode();
    this->movingAxis = -1;
    tvCounter = 0;
    
    /* Setting up Room and Floor */
    Model *tmp = new Model();
    tmp->vLoad("objects/plane", false, "textures/tapete2.bmp");
    
    this->models.insert(std::pair<std::string, Model*>("plane", tmp));
    
    this->m_Root.setName("Boden");
    this->m_Root.setModel(this->models.at("plane"));
    this->m_Root.setParent(NULL);
    
    Matrix M;
    M.translation(0, 0, 0);
    
    this->m_Root.setLocalTransform(M);
    
    /* Setting up Scene */
    this->addSceneFile(sceneFile);

}

FurnitureManager::~FurnitureManager(){
    
}

void FurnitureManager::safeScene(){
    SceneFileWriter sfw;
    
    sfw.writeSceneInFile(m_Root, this->roomDimension, "scene.txt");
}


void FurnitureManager::changePaper(){
    Vector directionVector = mathClass.getRayInWorldSpace();
    Vector cameraPosition = g_Camera.getPosition();
    
    directionVector.normalize();
    
    this->roomDimension.checkIfWallIsHit(cameraPosition,directionVector);
}

///
/// Überprüft ob Wände oder Objekte getroffen sind.
///
void FurnitureManager::checkPicking(){
   
    Vector directionVector = mathClass.getRayInWorldSpace();
    Vector cameraPosition = g_Camera.getPosition();
    
    directionVector.normalize();
    
    this->objectPicking(cameraPosition, directionVector);
}


///
/// Überprüfung von getroffenen Objekten und einige zusammenhängende Funktionen
///
bool FurnitureManager::objectPicking(Vector cameraPosition, Vector ray){
    
    float nearest = std::numeric_limits<float>::max();
    Vector intersection;
    FurnitureNode *tmp = NULL;
    
    typedef std::map<std::string, FurnitureNode*>::iterator it_type;
    int counter = 0;
    for(it_type i = this->objects.begin();
        i != this->objects.end(); i++){
        
        FurnitureNode* element = i->second;
        Matrix scale;
    
        if(element->getModel()->BBoxIntersection(cameraPosition, ray, &intersection, element->getGlobalTransform()*scale.scale(element->getScaling()))){
            
            if((intersection-cameraPosition).length() < nearest){
                nearest = (intersection-cameraPosition).length();
                tmp = element;
            }
        }
        counter++;
    }
    
    /*
     
     Aktionen, die mit Hilfe des neu getroffenen Objekts durchgeführt werden:
     
     Hierarchie entfernen.
     Aktuellen FurnitureNode setzen.
     Hierarchie hinzufügen
     
     
     */
    if(tmp != NULL){
        if(isInEraseHierachyMode()){
            
            if(tmp->getParent() != NULL){
                
                Matrix parentMatrix = tmp->getParent()->getGlobalTransform();
                
                tmp->setPosition(parentMatrix*tmp->getPosition());
                std::cout << "Enternen der Hierarchieinformationen von " << tmp->getName() << std::endl;
                std::cout << " ++++ " << std::endl;
                this->clearHierachyFromNode(tmp);
                
            }
        }else if(!isInHierachyMode()){
            
            std::cout << "Neues aktuelles Möbelobjekt: " << tmp->getName() << std::endl;
            std::cout << " ++++ " << std::endl;
            this->currentFurnitureObject = tmp;
            
        }else{
            if(tmp->getParent() != NULL && tmp != this->currentFurnitureObject){
                
                if(tmp->getParent()->getName() == "Boden"){
                    
                    m_Root.removeChild(tmp);
                    
                    this->currentFurnitureObject->addChild(tmp);
                    Matrix parentMatrix = this->currentFurnitureObject->getGlobalTransform();
                    tmp->setPosition(parentMatrix.invert() * tmp->getPosition());
                    
                    tmp->setParent(this->currentFurnitureObject);
                    std::cout << "Neue Hierarchieinformationen hinzugefügt" << std::endl;
                    std::cout << " ++++ " << std::endl;
                    
                }
                
                
                
            }else{
                m_Root.removeChild(tmp);
                tmp->setParent(this->currentFurnitureObject);
            }
        }
        
        return true;
    }else{
        return false;
    }
}


void FurnitureManager::activateChanceRoomGeometryMode(){
    this->chanceRoomGeometryMode = true;
}
void FurnitureManager::deactivateChanceRoomGeometryMode(){
    this->chanceRoomGeometryMode = false;
}

bool FurnitureManager::isInRoomGeometryMode(){
    return this->chanceRoomGeometryMode;
}



void FurnitureManager::changeRoomWidthDimension(float pFactor){
    this->roomDimension.changeWidth(pFactor);
}
void FurnitureManager::changeRoomDepthDimension(float pFactor){
    this->roomDimension.changeDepth(pFactor);
}
void FurnitureManager::changeRoomHeightDimension(float pFactor){
    
}

///
/// Removing the hierarchy information of one furnitureObject without children
/// ToDo: Removing the hierarchy information of one furnitureObject with children
///
void FurnitureManager::clearHierachyFromNode(FurnitureNode *pNode){
    
    if(pNode->getChildren().size() > 0){
    
        std::cout << "ToDo" << std::endl;
        // ToDo
        /*typedef std::set<FurnitureNode*>::iterator it_type;
        
        for(it_type i = pNode->getChildren().begin();
            i != pNode->getChildren().end(); i++){
            
            FurnitureNode* element = *i;
            
            clearHierachyFromNode(element);
            
        }*/

        
    }else{
        pNode->getParent()->removeChild(pNode);
        pNode->setParent(&m_Root);
        m_Root.addChild(pNode);
    }
    
}

void FurnitureManager::increaseScaleFromCurrentNode(){
    
    if(this->hasCurrentFurnitureElement())
        this->currentFurnitureObject->increaseScaling();
    
        
}

void FurnitureManager::clearCurrentFurnitureNode(){
    if(this->hasCurrentFurnitureElement()){
        std::cout << "Aktuelles Möbelobjekt wurde entfernt" << std::endl;
        std::cout << " ++++ " << std::endl;
        this->currentFurnitureObject = NULL;
    }
}
void FurnitureManager::decreaseScaleFromCurrentNode(){
    if(this->hasCurrentFurnitureElement())
        this->currentFurnitureObject->decreaseScaling();
    
}




///
/// Moving the currentFurnitureObject with mouse movements and the corresponding axis, which is hit
///
void FurnitureManager::moveCurrentNodeWithMouse(int x, int y, int Button, int State){
    if(State == GLUT_DOWN){
        
        if(Button == GLUT_LEFT_BUTTON){
        
            if(movingAxis == 1){
                
                
                unsigned int lastX = moController.getLastXMousePos();
                
                int dX = (lastX-x);
                
                if(dX != 0 && dX < 50 && dX > -50){
                    
                    float factor = dX;
                    float movement = 0.01f*factor;
                    if(checkXRangeFromCurrentNode(movement))
                        this->currentFurnitureObject->changePositionX(movement);
                }
                
                
                
                moController.setLastXMousePos(x);
                moController.setLastYMousePos(y);
            }else if(movingAxis == 2){
                
                unsigned int lastY = moController.getLastYMousePos();
                
                int dY = (lastY-y);
                
                if(dY != 0 && dY < 50 && dY > -50){
                    
                    float factor = dY;
                    float movement = 0.01f*factor;
                    
                    this->currentFurnitureObject->changePositionY(movement);
                }
                
                
                
                moController.setLastXMousePos(x);
                moController.setLastYMousePos(y);
                
            }else if(movingAxis == 3){
                unsigned int lastX = moController.getLastXMousePos();
                
                int dX = (lastX-x);
                
                if(dX != 0 && dX < 50 && dX > -50){
                    
                    float factor = dX;
                    float movement = 0.01f*factor;
                    if(checkZRangeFromCurrentNode(movement))
                        this->currentFurnitureObject->changePositionZ(movement);
                    
                    
                }
                
                moController.setLastXMousePos(x);
                moController.setLastYMousePos(y);
            }else{
            
            }
        }
    }

}


bool FurnitureManager::checkAxesFromCurrentNode(){
    if(hasCurrentFurnitureElement()){
        Vector ray = mathClass.getRayInWorldSpace();
        Vector intersection;
        
        Matrix SM;
        SM.scale(this->currentFurnitureObject->getScaling());
        
        this->movingAxis = this->axisModel.checkIfAxisIsHit(this->currentFurnitureObject->getGlobalPosition(), g_Camera.getPosition(), ray);
        
        if(this->movingAxis > -1){
            this->moveAxes = true;
        }
        
    }
    
    return false;
}

///
/// Load scene
///
bool FurnitureManager::addSceneFile( const char* pSceneFile){
    std::fstream sceneFile(pSceneFile, std::ios::in);
    std::string type, redundantInformation;
    
    if(sceneFile.good()){
        while(sceneFile >> type){
            if(type == "room"){
                this->setupRoomDimension(sceneFile);
            }
            if(type == "object"){
                
                addFurnitureNodeFromStream(sceneFile);
            }
        }
    }
    
    std::cout << this->tvCounter << " TV gefunden." << std::endl;
    
    return true;
}

void FurnitureManager::setupRoomDimension(std::fstream &sceneFStream){
    std::string redundantInformation;
    std::string type, information, path;
    sceneFStream >> redundantInformation >> redundantInformation;
    
    float size1, size2, size3;
    int color1, color2, color3, color4;
    float u, v;
    sceneFStream >> size1 >> size2 >> size3;
    int colors[4];
    
    for(int i=0;i<4;i++)
        colors[i] = 0;
    
    
    sceneFStream >> type;
    
    sceneFStream >> colors[0] >> colors[1] >> colors[2] >> colors[3];
    
    
   
    
    // Setup the RoomDimension
    this->roomDimension = RoomDimension(size1, size2, size3,colors);
    
    sceneFStream >> type;
    
    if(type == "wallpaper"){
        sceneFStream >> redundantInformation >> type;
        
        if(type == "texture"){
            sceneFStream >> path;
            sceneFStream >> type;
        }
        
        if(type == "tiling"){
            sceneFStream >> u >> v;
        }
        
        sceneFStream >> redundantInformation;
        sceneFStream >> type;
        
    }
    
    if(type == "floor"){
        sceneFStream >> redundantInformation >> type;
        
        if(type == "texture"){
            sceneFStream >> path;
            sceneFStream >> type;
        }
        
        if(type == "tiling"){
            sceneFStream >> u >> v;
        }
        
        sceneFStream >> redundantInformation >> redundantInformation;
    }
    
    
    
    
    
    
    
    if(type == "foor"){
        
        sceneFStream >> redundantInformation >> redundantInformation;
    }
    
    
}

void FurnitureManager::addFurnitureNodeFromStream(std::fstream &sceneFStream){
    std::string type, name;
    
    sceneFStream >> name;
    
    float t1, t2, t3;
    float r1, r2, r3, r4;
    float s1, s2, s3;
    std::string modelPath, modelFileName;
    std::string parentName;
    
    for(int i=0; i<=5; i++){
        sceneFStream >> type;
        if(type == "translation"){
            
            sceneFStream >> t1 >> t2 >> t3;
        }else if (type == "rotation"){
            
            sceneFStream >> r1 >> r2 >> r3 >> r4;
            
        }else if (type == "scaling"){
            
            sceneFStream >> s1 >> s2 >> s3;
            
        }else if (type == "model"){
            
            sceneFStream >> modelPath;
            
            
            
            if(this->models.find(modelPath) == this->models.end()){
                Model *tmp = new Model();
                tmp->vLoad(modelPath.c_str(), true, "textures/tapete2.bmp");
                this->models.insert(std::pair<std::string, Model*>(modelPath, tmp));
            }
            
                
        }else if (type == "parent"){
            
            sceneFStream >> parentName;
            
        }
    }
    
    FurnitureNode *tmp = new FurnitureNode();
    
    tmp->setName(name);
    
    if(parentName == "NULL"){
        
        tmp->setParent(&m_Root);
    }else{
        tmp->setParent(this->objects.at(parentName));
    }
    tmp->setScaling(Vector(s1, s2, s3));
    tmp->setLocalTransform(Vector(t1, t2, t3), r4);
    
    if(modelPath == "objects/tv.obj"){
        tvCounter++;
    }
    
    tmp->setModel(this->models.at(modelPath));
    tmp->setModelName(modelPath);
    
    
    
    this->objects.insert(std::pair<std::string, FurnitureNode*>(name, tmp));
    
    if(parentName != "NULL"){
        
        if(this->objects.at(parentName) != NULL && this->objects.at(name)){
            
            
            Matrix parentMatrix = this->objects.at(parentName)->getGlobalTransform();
            //this->objects.at(name)->setPosition(parentMatrix.invert() * this->objects.at(name)->getPosition());
        
            this->objects.at(parentName)->addChild(this->objects.at(name));
        }
    }else{
        this->m_Root.addChild(tmp);
    }
    
}




bool FurnitureManager::addFurnitureNode(){
    return false;
}


void FurnitureManager::changeRotationFromCurrentNode(){
    if(this->hasCurrentFurnitureElement()){
        this->currentFurnitureObject->rotate();
    }
}

void FurnitureManager::changeXPositionFromCurrentNode(float pFactor){
    
    if(this->hasCurrentFurnitureElement()){
        if(checkXRangeFromCurrentNode(pFactor))
            this->currentFurnitureObject->changePositionX(pFactor);
    }
    
}

bool FurnitureManager::checkXRangeFromCurrentNode(float pFactor){
    Matrix TM, SM, RM;
    
    RM.rotationY(this->currentFurnitureObject->getRotation());
    SM.scale(this->currentFurnitureObject->getScaling());
    
    TM = RM*SM;
    
    Vector BBMin = TM.transformVec4x4(this->currentFurnitureObject->getModel()->boundingBox().Max);
    Vector BBMax = TM.transformVec4x4(this->currentFurnitureObject->getModel()->boundingBox().Min);
    
    float BBMinX = BBMin.getX() + 0.05;
    float BBMaxX = BBMax.getX() - 0.05;
    
    
    
    float newXPositionBBMax = this->currentFurnitureObject->getPosition().getX()+pFactor +BBMaxX;
    float newXPositionBBMin = this->currentFurnitureObject->getPosition().getX()+pFactor + BBMinX;
    
    return this->roomDimension.checkXRange(newXPositionBBMax) && this->roomDimension.checkXRange(newXPositionBBMin);
}



///
/// Checking if the movement of the currentFurnitureNode is still in the current roomDimensions
///
void FurnitureManager::changeZPositionFromCurrentNode(float pFactor){

    if(this->currentFurnitureObject != NULL){
        
        if(this->checkZRangeFromCurrentNode(pFactor))
            this->currentFurnitureObject->changePositionZ(pFactor);
    }
    
}


///
/// Checking if the movement of the currentFurnitureNode is still in the current roomDimensions
///
bool FurnitureManager::checkZRangeFromCurrentNode(float pFactor){
    // Räumlichkeiten prüfen...
    Matrix TM, SM, RM;
    
    RM.rotationY(this->currentFurnitureObject->getRotation());
    SM.scale(this->currentFurnitureObject->getScaling());
    
    TM = RM*SM;
    
    Vector BBMin = TM.transformVec4x4(this->currentFurnitureObject->getModel()->boundingBox().Max);
    Vector BBMax = TM.transformVec4x4(this->currentFurnitureObject->getModel()->boundingBox().Min);
    
    float BBMinZ = BBMin.getZ() + 0.05;
    float BBMaxZ = BBMax.getZ() - 0.05;
    
    float newZPositionBBMax = this->currentFurnitureObject->getPosition().getZ()+pFactor +BBMaxZ;
    float newZPositionBBMin = this->currentFurnitureObject->getPosition().getZ()+pFactor + BBMinZ;
    
    return this->roomDimension.checkZRange(newZPositionBBMax) && this->roomDimension.checkZRange(newZPositionBBMin);
}


void FurnitureManager::changeYPositionFromCurrentNode(float pFactor){
    
    if(this->hasCurrentFurnitureElement()){
        
        this->currentFurnitureObject->changePositionY(pFactor);
    }
    
}

void FurnitureManager::drawNode(FurnitureNode* pNode){
    if(pNode != NULL){
        if(pNode->getChildren().size() > 0){
            
            for (std::set<FurnitureNode*>::iterator i = pNode->getChildren().begin();
                 i != pNode->getChildren().end();
                 i++) {
                
                FurnitureNode* element = *i;
                Matrix scale, result,TM;
                scale.scale(element->getScaling());
                
                TM = element->getGlobalTransform();
                result = TM*scale;
                
            
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                
                    glMultMatrixf(result);
                
                    element->getModel()->vDraw(false);
                
                glPopMatrix();
                    
                
                if(element != NULL)
                    this->drawNode(element);
                
            }
            
        }
    }
}

void FurnitureManager::addObject(){
    if(this->isInAddTvMode()){
        Calculations c;
        
        
        FurnitureNode *tmp = new FurnitureNode();
        
        tmp->setParent(&m_Root);
        tmp->setLocalTransform(c.getPositionOnFloorWithMouseCoords(), 0);
        tmp->setScaling(Vector(1, 1, 1));
        tmp->setModel(models.at("objects/tv.obj"));
        tmp->setModelName("objects/tv.obj");
        tvCounter++;
        std::string objectName;
        
        
        std::stringstream sstm;
        sstm << "TV" << ++tvCounter;
        objectName = sstm.str();
        
        tmp->setName(objectName);
        
        m_Root.addChild(tmp);
        
        this->objects.insert(std::pair<std::string, FurnitureNode*>(objectName, tmp));
        
    }
}


void FurnitureManager::deleteCurrentNode(){
    if(this->hasCurrentFurnitureElement()){
        if(this->currentFurnitureObject->getChildren().size() > 0){
            
        }else{
            if(this->currentFurnitureObject->getParent() != NULL){
                this->currentFurnitureObject->getParent()->removeChild(this->currentFurnitureObject);
                this->currentFurnitureObject->setParent(NULL);
                
                this->objects.erase(this->currentFurnitureObject->getName());
                
                this->currentFurnitureObject = NULL;
            }
        }
        
        
    }
}

void FurnitureManager::draw(){
    if(this->hasCurrentFurnitureElement()){
        Matrix RM;
        this->axisModel.draw(this->currentFurnitureObject->getGlobalPosition(),this->currentFurnitureObject->getRotation());
    }
        
    this->roomDimension.draw();
    if(this->isInAddTvMode()){
        Calculations c;
        
        Matrix TM, SM, result;
        TM.translation(c.getPositionOnFloorWithMouseCoords());
        
        SM.scale(1);
        
        result = TM*SM;
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        
            glMultMatrixf(result);
            this->models.at("objects/tv.obj")->vDraw(false);
        
        glPopMatrix();
        
        
    }
    this->drawNode(&m_Root);
    
}


bool FurnitureManager::isInEraseHierachyMode(){
    return this->eraseHierarchyMode;
}


void FurnitureManager::activateEraseHierachyMode(){
    this->eraseHierarchyMode = true;
}

void FurnitureManager::deactivateEraseHierachyMode(){
    this->eraseHierarchyMode = false;
}


void FurnitureManager::activateAddTvMode(){
    this->addTvMode = true;
}
void FurnitureManager::deactivateAddTvMode(){
    this->addTvMode = false;
}


bool FurnitureManager::isInAddTvMode(){
    return this->addTvMode;
}

bool FurnitureManager::isInHierachyMode(){
    return this->hierarchyMode;
}

void FurnitureManager::activateHierachyMode(){
    this->hierarchyMode = true;
}

void FurnitureManager::deactivateHierachyMode(){
    this->hierarchyMode = false;
}

bool FurnitureManager::hasCurrentFurnitureElement(){
    return this->currentFurnitureObject != NULL;
}

bool FurnitureManager::isInMovingAxisMode(){
    return this->moveAxes;
}

void FurnitureManager::deactiveMoveAxisMode(){
    this->moveAxes = false;
    this->movingAxis = -1;
}