//
//  NCMesh.h
//  assimpFromScratch
//
//  Created by Wim Vanhenden on 30/05/13.
//
//

#pragma once
#include "ofMain.h"
#include "aiConfig.h"
#include "aiPostProcess.h"
#include "aiScene.h"

class NCMesh {

public:
    
    aiMesh *mesh;
    ofVboMesh ofmesh;
    
    vector <aiVector3D>animatedNorm;
    vector <aiVector3D>animatedPos;
    
    void setup(aiMesh * mesh);
    
    void setup(const aiScene* scene);
    
    void update();
    void draw();
     
    float scale;
    
};