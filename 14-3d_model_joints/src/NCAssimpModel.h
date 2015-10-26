//
//  NCAssimpModel.h
//  modelComparer
//
//  Created by Wim Vanhenden on 30/05/13.
//
//

#pragma once

#include "ofMain.h"

#include "assimp.h"
#include "aiScene.h"
#include "NCBone.h"
#include "NCAIMesh.h"

class NCAssimpModel {
public:
    void setup(string pathtomodel);
    void update();
    void draw();
    
    void resetToBindPose();
    
    void rotateNodeWithKinectSkeletonData(ofMatrix4x4 incrotation, string boneName);
    
    vector<NCBone>   bones;
    string jointlabels[100];
    
    void rotateNodeX(string nodename, float degrees);
    void rotateNodeY(string nodename, float degrees);
    void rotateNodeZ(string nodename, float degrees);
    void rotateNode(string nodename, aiMatrix4x4 &rot);
    
    aiMatrix4x4 toAi(ofMatrix4x4 & m);
    ofMatrix4x4 tooF(aiMatrix4x4 & m);
    aiMatrix4x4 returnInverseRotation(aiMatrix4x4 incmat);
    aiMatrix4x4 returnRotation(aiMatrix4x4 incmat);
    aiMatrix4x4 returnTranslation(aiMatrix4x4 incmat);
    aiMatrix4x4 returnScale(aiMatrix4x4 incmat);
    
    
private:
    
    void initBones();
    void saveChild(aiNode* node);
   
    const aiScene* scene;
    
    vector<NCAIMesh> meshes;
    
    const NCBone & findBoneWithName(string incname);
    
};