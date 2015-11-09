#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Slitscan example");
    ofBackground(33);
    
    // Init grabber
    
    grabber.initGrabber(320, 240);
    
    // Init FBO
    // FBO sizes need to be a power of 2
    // Otherwise openFrameworks will scale
    // our FBO, which results in incorrect
    // mapping of texture coordinates
    
    fboSize = 512;
    index = 0;
    
    fbo.allocate(fboSize, fboSize);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Grab the camera feed
    // and check for new frames
    
    grabber.update();
    if(grabber.isFrameNew()){
        
        // And draw a slice of the
        // new frame to the FBO
        
        float slice = grabber.getWidth() / float(fboSize);
        float offset = grabber.getWidth() * 0.5;
        
        fbo.begin();
        grabber.getTexture().drawSubsection(index, 0, slice, fboSize, offset, 0, 1, grabber.getHeight());
        fbo.end();
        
        // Increase index or
        // x-offset position
        
        index < fboSize ? index++ : index=0;
        
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    debugDraw();
    
    ofPushMatrix();
    ofTranslate(360, 50);
    
    fbo.draw(0, 0, 1024, 240);
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::debugDraw(){
    
    ofPushMatrix();
    ofTranslate(20, 30);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 0, 0);
    
    grabber.draw(0, 20, 320, 240);
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
