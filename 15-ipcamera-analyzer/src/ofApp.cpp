#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //
    
    ofBackground(33, 33, 33);
   
    analyzer.setup(640, 480, false, true, false);
    //analyzer.setup(640, 480,ofRectangle(100, 100, 320, 240),true,false,false);
    analyzer.docontourfinding = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    analyzer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    analyzer.draw();
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
