//
//  NCMesh.cpp
//  assimpFromScratch
//
//  Created by Wim Vanhenden on 30/05/13.
//
//

#include "NCAIMesh.h"

void NCAIMesh::setup(aiMesh * _mesh, const aiScene * _scene) {

    scale = 1.0f;
    
    mesh = _mesh;
    scene = _scene;
    
    animatedPos.clear();
    ofmesh.clear();
    
    //first build the ofmesh
    /*int iMeshFaces = mesh->mNumFaces;
    for (int j=0;j<iMeshFaces;j++) {
        for (int k=0;k<3;k++) {
            const aiFace& face = mesh->mFaces[j];
            aiVector3D pos = mesh->mVertices[face.mIndices[k]];
            aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
            aiVector3D normal = mesh->mNormals[face.mIndices[k]];
            ofmesh.addVertex(ofVec3f(pos.x,pos.y,pos.z));
        }
    }*/
    
    for (int i=0;i<mesh->mNumVertices;i++) {
        ofVec3f vertex;
        vertex.x = mesh->mVertices[i].x;
        vertex.y = mesh->mVertices[i].y;
        vertex.z = mesh->mVertices[i].z;
        ofmesh.addVertex(vertex);
    }
    
    for (unsigned int x = 0; x < mesh->mNumFaces; ++x){
        ofmesh.addTriangle(mesh->mFaces[x].mIndices[0], mesh->mFaces[x].mIndices[1], mesh->mFaces[x].mIndices[2]);
    }
    
}

void NCAIMesh::update(){
    // calculate bone matrices
    std::vector<aiMatrix4x4> boneMatrices( mesh->mNumBones);
    for( size_t a = 0; a < mesh->mNumBones; ++a) {
        const aiBone* bone = mesh->mBones[a];
        // find the corresponding node by again looking recursively through the node hierarchy for the same name
        aiNode* node = scene->mRootNode->FindNode(bone->mName);
        
        // start with the mesh-to-bone matrix
        boneMatrices[a] = bone->mOffsetMatrix;
        
        // and now append all node transformations down the parent chain until we're back at mesh coordinates again
        const aiNode* tempNode = node;
        while( tempNode) {
            boneMatrices[a] = tempNode->mTransformation * boneMatrices[a];
            tempNode =tempNode->mParent;
        }
    }
    
    animatedPos.assign(ofmesh.getVertices().size(),0);
    
    // loop through all vertex weights of all bones
    for( size_t a = 0; a < mesh->mNumBones; ++a) {
        const aiBone* bone = mesh->mBones[a];
        aiMatrix4x4& posTrafo = boneMatrices[a];
    
        for( size_t b = 0; b < bone->mNumWeights; ++b) {
            const aiVertexWeight& weight = bone->mWeights[b];
            size_t vertexId = weight.mVertexId;
            const aiVector3D& srcPos = mesh->mVertices[vertexId];
            //cout << " \n"<< mymesh.animatedPos.size() <<" "<< mymesh.ofmesh.getVertices().size();
            animatedPos[vertexId] += weight.mWeight * (posTrafo * srcPos);
        }
    }

    //update the ofmesh
    for(int i=0;i<ofmesh.getVertices().size();i++ ) {
        ofVec3f vec(animatedPos[i].x*scale, animatedPos[i].y*scale, animatedPos[i].z*scale);
        ofmesh.setVertex(i, vec);
    }

}

void NCAIMesh::draw() {
    ofmesh.draw();
}