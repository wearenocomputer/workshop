//
//  NC2DSilhouettes.h
//  BionicKinect
//
//  Created by Wim Vanhenden on 07/03/14.
//
//

#pragma once
#include "ofMain.h"
#include "NCKinectControllerofXOpnniMethod.h"
#include "ofxPoly2Tri.h"

class NC2DSilhouettes {
    
public:
    
    void setup();
    
    void update();
    
    void draw();
    

private:
   
    void convertLinesToMeshes();
    vector<ofxPoly2Tri>       trimesh;
    
};