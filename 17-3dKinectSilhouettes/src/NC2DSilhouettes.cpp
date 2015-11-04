//
//  NC2DSilhouettes.cpp
//  BionicKinect
//
//  Created by Wim Vanhenden on 07/03/14.
//
//

#include "NC2DSilhouettes.h"

bool isPointInsideHole(ofPoint p, ofPolyline line) {
    if ( line.inside(p, line)) {
        return true;
    }
    return false;
}

void NC2DSilhouettes::setup() {
    
}

void NC2DSilhouettes::convertLinesToMeshes() {
    trimesh.clear();
    for (int i=0;i< NCKinectControllerofXOpnniMethod::getInstance()->contourlines.size();++i) {
        
        ofPolyline &line = NCKinectControllerofXOpnniMethod::getInstance()->contourlines[i];
        line = line.getResampledByCount(100).getSmoothed(2);
        
        ofRectangle rect = line.getBoundingBox();
        
        ofxPoly2Tri trian;
        vector<ofPoint>points;
        
        //create random points in shape
        while (points.size()<20) {
            ofPoint po = ofPoint(ofRandom(rect.x,rect.x+rect.getWidth()),ofRandom(rect.y,rect.y+rect.getHeight()));
            if (isPointInsideHole(po, line)) {
                points.push_back(po);
            }
        }
        trian.bUseVboMesh = false;
        trian.setSteinerPoints(points);
        trian.triangulate(line);
        trimesh.push_back(trian);
    }
    
}

void NC2DSilhouettes::update() {
    convertLinesToMeshes();
}

void NC2DSilhouettes::draw() {
    for (int i=0;i<trimesh.size();++i) {
        if (trimesh[i].bUseVboMesh) {
            trimesh[i].vboMesh.draw();
        } else {
            trimesh[i].triangulatedMesh.draw();
        }
        
    }
}