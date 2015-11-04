#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(33, 33, 33);
    //setup cam
    cam.setDistance(10);
    cam.setNearClip(0.1);
    cam.setScale(1,-1,1);
    
    NCKinectControllerofXOpnniMethod::getInstance()->setup();
    NCKinectControllerofXOpnniMethod::getInstance()->bDebugDraw = true;
    NCKinectControllerofXOpnniMethod::getInstance()->breMap2DToScreen2D = true;
    
    silhouettes.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    NCKinectControllerofXOpnniMethod::getInstance()->update();
    silhouettes.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    NCKinectControllerofXOpnniMethod::getInstance()->draw();
    
    cam.begin();
    silhouettes.draw();
    ofDrawGrid();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='f') {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
