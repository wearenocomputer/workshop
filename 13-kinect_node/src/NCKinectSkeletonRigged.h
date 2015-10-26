//
//  NCKinectSkeleton.h
//  assimp_OpenNI
//
//  Created by Wim Vanhenden on 14/05/13.
//
//

#pragma once
#include "ofMain.h"
#include "NCOpenNIJoint.h"

class NCKinectSkeletonRigged {
    
public:
    
    vector<NCOpenNIJoint*> joints;
    
    void setup();
    
    NCOpenNIJoint *pelvis;
    NCOpenNIJoint *neck;
    NCOpenNIJoint *left_upper_leg;
    NCOpenNIJoint *left_lower_leg;
    NCOpenNIJoint *left_foot;
    NCOpenNIJoint *right_upper_leg;
    NCOpenNIJoint *right_lower_leg;
    NCOpenNIJoint *right_foot;
    NCOpenNIJoint *left_upper_arm;
    NCOpenNIJoint *left_lower_arm;
    NCOpenNIJoint *right_upper_arm;
    NCOpenNIJoint *right_lower_arm;
    
    void updateSkeleton(ofxUserGenerator &usergen,ofxTrackedUser &user);
    
    void rotateNodeX(string nodename, float degrees);
    void rotateNodeY(string nodename, float degrees);
    void rotateNodeZ(string nodename, float degrees);
    
    void resetNodesRotation();
    
   
    void draw();
    
    NCOpenNIJoint * findBoneWithRegularName(string incname);    
    
private:
     void update();
    
};
