//
//  NCAIMesh.h
//  modelComparer
//
//  Created by Wim Vanhenden
//  with the amazing help of the Openframeworks forum || Kyle McDonald and Arturo
//  http://forum.openframeworks.cc/t/posing-with-ofxassimpmodelloader/7714
//  http://forum.openframeworks.cc/t/blender-collada-ofxassimploader-bones-not-working/12584


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