//
//  NCKinectSkeleton.cpp
//  assimp_OpenNI
//
//  Created by Wim Vanhenden on 14/05/13.
//
//

#include "NCKinectSkeletonRigged.h"

void NCKinectSkeletonRigged::setup() {
    
    pelvis = new NCOpenNIJoint();
    pelvis->setPosition(0, 0, 0);
    pelvis->name = XN_SKEL_TORSO;
    pelvis->stringname = "pelvis";
    
    neck = new NCOpenNIJoint();
    neck->setParent(*pelvis);
    neck->setPosition(0, 400, 0);
    neck->name = XN_SKEL_NECK;
    neck->stringname = "neck";
    
    left_upper_leg = new NCOpenNIJoint();
    left_upper_leg->setParent(*pelvis);
    left_upper_leg->setPosition(100, -200, 0);
    left_upper_leg->name = XN_SKEL_RIGHT_HIP;
    left_upper_leg->stringname = "l_hip";
    
    left_lower_leg = new NCOpenNIJoint();
    left_lower_leg->setParent(*left_upper_leg);
    left_lower_leg->setPosition(0, -200, 0);
    left_lower_leg->name = XN_SKEL_RIGHT_KNEE;
    left_lower_leg->stringname = "l_knee";
    
    left_foot = new NCOpenNIJoint();
    left_foot->setParent(*left_lower_leg);
    left_foot->setPosition(0, -50, 40);
    ofQuaternion quat;
    quat.makeRotate(90, 1, 0, 0);
    left_foot->bindrotation = quat;
    
    right_upper_leg = new NCOpenNIJoint();
    right_upper_leg->setParent(*pelvis);
    right_upper_leg->setPosition(-100, -200, 0);
    right_upper_leg->name = XN_SKEL_LEFT_HIP;
    right_upper_leg->stringname = "r_hip";
    
    right_lower_leg = new NCOpenNIJoint();
    right_lower_leg->setParent(*right_upper_leg);
    right_lower_leg->setPosition(0, -200, 0);
    right_lower_leg->name = XN_SKEL_LEFT_KNEE;
    right_lower_leg->stringname = "r_knee";
    
    right_foot = new NCOpenNIJoint();
    right_foot->setParent(*right_lower_leg);
    right_foot->setPosition(0, -50, 40);
    ofQuaternion quatrightfoot;
    quatrightfoot.makeRotate(90, 1, 0, 0);
    right_foot->bindrotation = quat;
    
    left_upper_arm = new NCOpenNIJoint();
    left_upper_arm->setParent(*pelvis);
    left_upper_arm->setPosition(100, 200, 0);
    left_upper_arm->name = XN_SKEL_RIGHT_SHOULDER;
    left_upper_arm->stringname = "l_humerus";
   
    
    left_lower_arm = new NCOpenNIJoint();
    left_lower_arm->setParent(*left_upper_arm);
    left_lower_arm->setPosition(200, 0, 0);
    left_lower_arm->name = XN_SKEL_RIGHT_ELBOW;
    left_lower_arm->stringname = "l_ulna";
   
    right_upper_arm = new NCOpenNIJoint();
    right_upper_arm->setParent(*pelvis);
    right_upper_arm->setPosition(-100, 200, 0);
    right_upper_arm->name = XN_SKEL_LEFT_SHOULDER;
    right_upper_arm->stringname = "r_humerus";
    
    right_lower_arm = new NCOpenNIJoint();
    right_lower_arm->setParent(*right_upper_arm);
    right_lower_arm->setPosition(-200, 0, 0);
    right_lower_arm->name = XN_SKEL_LEFT_ELBOW;
    right_lower_arm->stringname = "r_ulna";

    joints.push_back(pelvis);
    joints.push_back(neck);
    joints.push_back(left_upper_arm);
    joints.push_back(left_lower_arm);
    joints.push_back(right_upper_arm);
    joints.push_back(right_lower_arm);
    joints.push_back(left_upper_leg);
    joints.push_back(left_lower_leg);
    joints.push_back(left_foot);
    joints.push_back(right_upper_leg);
    joints.push_back(right_lower_leg);
    joints.push_back(right_foot);
  
}

void NCKinectSkeletonRigged::updateSkeleton(ofxUserGenerator &usergen,ofxTrackedUser &user) {
    for (int i=0;i<joints.size();++i) {
        joints[i]->calculateKinect(usergen, user);
    }
    update();
}

void NCKinectSkeletonRigged::rotateNodeX(string nodename, float degrees) {
    
    
    if (findBoneWithRegularName(nodename)!=NULL) {
        ofMatrix4x4 rot;
        rot.makeRotationMatrix(degrees, 1, 0, 0);
        findBoneWithRegularName(nodename)->localrotation = rot;
    }

}

void NCKinectSkeletonRigged::rotateNodeY(string nodename, float degrees) {
    if (findBoneWithRegularName(nodename)!=NULL) {
        ofMatrix4x4 rot;
        rot.makeRotationMatrix(degrees, 0, 1, 0);
    
        findBoneWithRegularName(nodename)->localrotation = rot;
  }
    
}

void NCKinectSkeletonRigged::rotateNodeZ(string nodename, float degrees) {
    if (findBoneWithRegularName(nodename)!=NULL) {
        ofMatrix4x4 rot;
        rot.makeRotationMatrix(degrees, 0, 0, 1);
    
        findBoneWithRegularName(nodename)->localrotation = rot;
    }

}

void NCKinectSkeletonRigged::resetNodesRotation() {
    for (int i=0;i<joints.size();++i) {
        joints[i]->localrotation = ofMatrix4x4();
        joints[i]->updateBone();
    }
}

NCOpenNIJoint * NCKinectSkeletonRigged::findBoneWithRegularName(string incname) {
    
    for (int i=0;i<joints.size();i++) {
        if (joints[i]->stringname == incname) {
            return joints[i];
        }
    }
    
    return NULL;
}

void NCKinectSkeletonRigged::update() {
    for (int i=0;i<joints.size();++i) {
        joints[i]->updateBone();
    }
}

void NCKinectSkeletonRigged::draw() {
    for (int i=0;i<joints.size();++i) {
        joints[i]->draw();
    }
}

