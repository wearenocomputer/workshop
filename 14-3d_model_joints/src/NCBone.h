//
//  NCBone.h
//  assimp_OpenNI
//
//  Created by Wim Vanhenden
//
//

#pragma once
#include "aiScene.h"
#include "ofMain.h"
class NCBone {
    
    public:
    
        aiString     name;
    
        aiNode       *node;
    
        aiMatrix4x4  ownstarttransform;
    
};
