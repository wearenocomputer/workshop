//
//  NCAssimpModel.cpp
//  modelComparer
//
//  Created by Wim Vanhenden on 30/05/13.
//
//

#include "NCAssimpModel.h"

aiMatrix4x4 NCAssimpModel::toAi(ofMatrix4x4 & m){
    return aiMatrix4x4(m(0,0),m(1,0),m(2,0),m(3,0),
                       m(0,1),m(1,1),m(2,1),m(3,1),
                       m(0,2),m(1,2),m(2,2),m(3,2),
                       m(0,3),m(1,3),m(2,3),m(3,3));
}

ofMatrix4x4 NCAssimpModel::tooF(aiMatrix4x4 & m){
    return ofMatrix4x4(m[0][0],m[1][0],m[2][0],m[3][0],
                       m[0][1],m[1][1],m[2][1],m[3][1],
                       m[0][2],m[1][2],m[2][2],m[3][2],
                       m[0][3],m[1][3],m[2][3],m[3][3]);
}

aiMatrix4x4 NCAssimpModel::returnInverseRotation(aiMatrix4x4 incmat) {
    aiVector3D pos;
    aiQuaternion quat;
    aiVector3D scale;
    incmat.Decompose(scale, quat, pos);
    aiMatrix4x4 toreturn = (aiMatrix4x4)quat.GetMatrix();
    
    toreturn.Inverse();
    
    return toreturn;
}


aiMatrix4x4 NCAssimpModel::returnRotation(aiMatrix4x4 incmat) {
    aiVector3D pos;
    aiQuaternion quat;
    aiVector3D scale;
    incmat.DecomposeNoScaling(quat, pos);
    aiMatrix4x4 toreturn = aiMatrix4x4(quat.GetMatrix());
    
    return toreturn;
    
}

aiMatrix4x4 NCAssimpModel::returnTranslation(aiMatrix4x4 incmat) {
    
    aiVector3D pos;
    aiQuaternion quat;
    aiVector3D scale;
    
    incmat.Decompose(scale, quat, pos);
    
    aiMatrix4x4 trans;
    aiMatrix4x4::Translation(pos, trans);
    
    return trans;
    
}

aiMatrix4x4 NCAssimpModel::returnScale(aiMatrix4x4 incmat) {
    aiVector3D pos;
    aiQuaternion quat;
    aiVector3D scale;
    incmat.Decompose(scale, quat, pos);
    
    aiMatrix4x4 scalee;
    aiMatrix4x4::Scaling(scale,scalee);
    
    return scalee;    
}


void NCAssimpModel::setup(string pathtomodel){

    string filepath = ofToDataPath(pathtomodel);
    bool optimize =false;

    // only ever give us triangles.
    aiSetImportPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
    aiSetImportPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, true);
    
    // aiProcess_FlipUVs is for VAR code. Not needed otherwise. Not sure why.
    unsigned int flags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
    
    if(optimize) flags |=  aiProcess_ImproveCacheLocality | aiProcess_OptimizeGraph |
        aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices |
        aiProcess_RemoveRedundantMaterials;
    
    if (scene) {
        for(int i = 0; i < 100; i++) {
            jointlabels[i] = "";
        }
        bones.clear();
        delete scene;
    }
    
    scene = aiImportFile(filepath.c_str(), flags);
    
    if(scene){
        meshes.clear();
        initBones();
        
        for (int i=0;i<scene->mNumMeshes;i++) {
            NCAIMesh mesh;
            mesh.setup(scene->mMeshes[i], scene);
            meshes.push_back(mesh);
        }
    }
   
}

void NCAssimpModel::update() {
    for (int i=0;i<meshes.size();i++) {
        meshes[i].update();
    }
}

void NCAssimpModel::draw() {
    for (int i=0;i<meshes.size();i++) {
        meshes[i].draw();
    }
}

void NCAssimpModel::initBones() {
    aiNode* root = scene->mRootNode;
    saveChild(root);
    for (int i=0;i<bones.size();++i) {
        jointlabels[i] = bones[i].node->mName.data;
    }
}

void NCAssimpModel::saveChild(aiNode* node) {
    NCBone bone;
    bone.ownstarttransform = node->mTransformation;
    bone.name = node->mName;
    bone.node = node;
    bones.push_back(bone);

    if (node->mNumChildren > 0) {
		for (int i=0 ;i < (int)node->mNumChildren;i++) {
			aiNode* childNode = node->mChildren[i];
            saveChild(childNode);
        }
    }
}


const NCBone & NCAssimpModel::findBoneWithName(string incname) {
    aiString bonename;
    bonename.Set(incname);
    
    for (int i=0;i<bones.size();i++) {
        if (bones[i].name == bonename) {
            return bones[i];
        }
    }
}

void NCAssimpModel::resetToBindPose() {
    for (int i=0; i<bones.size(); i++) {
        bones[i].node->mTransformation = bones[i].ownstarttransform;
    }
}

void NCAssimpModel::rotateNodeX(string nodename, float degrees) {
    aiMatrix4x4 rot;
    aiMatrix4x4::Rotation(ofDegToRad(degrees), aiVector3D(1,0,0), rot);
    rotateNode(nodename, rot);
}


void NCAssimpModel::rotateNodeY(string nodename, float degrees) {
    aiMatrix4x4 rot;
    aiMatrix4x4::Rotation(ofDegToRad(degrees), aiVector3D(0,1,0), rot);
    rotateNode(nodename, rot);
}

void NCAssimpModel::rotateNodeZ(string nodename, float degrees) {
    aiMatrix4x4 rot;
    aiMatrix4x4::Rotation(ofDegToRad(degrees), aiVector3D(0,0,1), rot);
    rotateNode(nodename, rot);
}

void NCAssimpModel::rotateNode(string nodename, aiMatrix4x4 &rot) {
    if (nodename!="") {
    findBoneWithName(nodename).node->mTransformation = findBoneWithName(nodename).ownstarttransform;
    findBoneWithName(nodename).node->mTransformation *= rot;
    }
}

void NCAssimpModel::rotateNodeWithKinectSkeletonData(ofMatrix4x4 incrotation, string boneName) {
    findBoneWithName(boneName).node->mTransformation = returnTranslation(findBoneWithName(boneName).ownstarttransform);
    findBoneWithName(boneName).node->mTransformation *= returnRotation(findBoneWithName(boneName).ownstarttransform);
    findBoneWithName(boneName).node->mTransformation *= toAi(incrotation);
}
