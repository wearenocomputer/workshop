//
//  NCKinectJoint.cpp
//  assimp_OpenNI
//
//  Created by Wim Vanhenden
//
//

#include "NCOpenNIJoint.h"


void NCOpenNIJoint::calculateKinect(ofxUserGenerator &usergen, ofxTrackedUser &user) {
    xn::SkeletonCapability pUserSkel = usergen.getXnUserGenerator().GetSkeletonCap();
    
    usergen.getXnUserGenerator().SetIntProperty("SkeletonHeuristics", 255);
    
    XnSkeletonJointPosition jointpos;
    pUserSkel.GetSkeletonJointPosition(user.id, name, jointpos);
    
    openniposition.x = jointpos.position.X;
    openniposition.y = jointpos.position.Y;
    openniposition.z = jointpos.position.Z;
    
    XnSkeletonJointOrientation jointOri;
    
    pUserSkel.GetSkeletonJointOrientation(user.id, name, jointOri);
    
    if(jointOri.fConfidence > 0.5 ) {
        
        float * oriM = jointOri.orientation.elements;
        
        ofMatrix4x4 kinectorientation;
        kinectorientation.makeIdentityMatrix();
        kinectorientation.set(oriM[0], oriM[3], oriM[6], 0.0f,
                              oriM[1], oriM[4], oriM[7], 0.0f,
                              oriM[2], oriM[5], oriM[8], 0.0f,
                              0.0f,    0.0f,     0.0f,   1.0f);
        
        
        ofMatrix4x4 flipZaxis;
        flipZaxis.makeRotationMatrix(180, 0, 0, 1);
        
        worldrotation.makeIdentityMatrix();
        worldrotation *= flipZaxis;
        worldrotation *= kinectorientation;
        worldrotation *= flipZaxis.getInverse();
        
        
    
        if (this->parent) {
            NCOpenNIJoint *parentjoint = (NCOpenNIJoint*)this->parent;
            localrotation = worldrotation*parentjoint->worldrotation.getInverse();
        } else {
            localrotation = worldrotation;
        }
    }
}

void NCOpenNIJoint::updateBone() {
    ofQuaternion quat;
    quat.set(localrotation);
    setOrientation(bindrotation*quat);
}
