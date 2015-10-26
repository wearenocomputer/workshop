#pragma once
#include "ofMain.h"
#include "aiConfig.h"
#include "aiPostProcess.h"
#include "aiScene.h"

class NCAIMesh {

public:
    
   
    ofVboMesh   ofmesh;
    void setup(aiMesh * _mesh, const aiScene *_scene);
    
    void update();
    void draw();
     
    float scale;
    
private:
    aiMesh *mesh;
    const aiScene *scene;
    vector <aiVector3D>animatedPos;
};