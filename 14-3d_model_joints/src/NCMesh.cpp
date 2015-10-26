//
//  NCMesh.cpp
//  assimpFromScratch
//
//  Created by Wim Vanhenden on 30/05/13.
//
//

#include "NCMesh.h"

void NCMesh::setup(const aiScene* scene) {
    
    ofmesh.clear();
    
    
    mesh = scene->mMeshes[2];
    
    for (int i=0;i<scene->mNumMeshes;i++) {
        aiMesh* mesh = scene->mMeshes[i];
        
        int iMeshFaces = mesh->mNumFaces;
        
        for (int j=0;j<iMeshFaces;j++) {
            
            for (int k=0;k<3;k++) {
                const aiFace& face = mesh->mFaces[j];
                aiVector3D pos = mesh->mVertices[face.mIndices[k]];
                aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
                aiVector3D normal = mesh->mNormals[face.mIndices[k]];
                ofmesh.addVertex(ofVec3f(pos.x,pos.y,pos.z));
            }
        }
    }
}

void NCMesh::setup(aiMesh * meshL) {
    
    scale = 1.0f;
    
    mesh = meshL;
    
    ofmesh.clear();

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

void NCMesh::update(){
    
    for(int i=0;i<ofmesh.getVertices().size();i++ ) {
        ofVec3f vec(animatedPos[i].x*scale, animatedPos[i].y*scale, animatedPos[i].z*scale);
        ofmesh.setVertex(i, vec);
    }
}

void NCMesh::draw() {
    ofmesh.drawWireframe();
}