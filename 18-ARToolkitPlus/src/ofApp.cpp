#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(33,33,33);
    
    width=640;
    height=480;
    
    vidGrabber.setVerbose(false);
    //vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(640, 480);
    colorImage.allocate(640, 480);
    grayImage.allocate(640, 480);
    grayThres.allocate(640, 480);
    
    artk.setup(640, 480);
    
    threshold = 85;
    artk.setThreshold(threshold);
    
    googleimage.loadImage("google.png");
    oracleimage.loadImage("oracle.png");
    
    warpermesh.addVertex(ofVec3f(100,100,0));
    warpermesh.addVertex(ofVec3f(100,200,0));
    warpermesh.addVertex(ofVec3f(200,200,0));
    warpermesh.addVertex(ofVec3f(200,10,0));
    
    warpermesh.addTexCoord(ofVec2f(0,300));
    warpermesh.addTexCoord(ofVec2f(300,300));
    warpermesh.addTexCoord(ofVec2f(300,0));
    warpermesh.addTexCoord(ofVec2f(0,0));
    
    warpermesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    
    if(vidGrabber.isFrameNew()) {
        colorImage.setFromPixels(vidGrabber.getPixels(), width, height);
        // convert our camera image to grayscale
        grayImage = colorImage;
        // apply a threshold so we can see what is going on
        grayThres = grayImage;
        grayThres.threshold(threshold);
        // Pass in the new image pixels to artk
        artk.update(grayImage.getPixels());
    }
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    grayImage.draw(0, 0);
    grayThres.draw(640, 0);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 20);
    ofDrawBitmapString(ofToString(artk.getNumDetectedMarkers()) + " marker(s) found", 10, 40);
    ofDrawBitmapString(ofToString(threshold) + " threshold", 650, 20);
    
    for (int i=0;i<artk.getNumDetectedMarkers();++i) {
        
        int detectmarkerid = artk.getMarkerID(i);
        
        if (detectmarkerid==0) {
            ofDrawBitmapString("google detected",10,60);
            vector<ofPoint> corners;
            artk.getDetectedMarkerBorderCorners(i, corners);
            warpermesh.setVertex(0,ofVec3f(corners[0].x,corners[0].y,0));
            warpermesh.setVertex(1,ofVec3f(corners[1].x,corners[1].y,0));
            warpermesh.setVertex(2,ofVec3f(corners[2].x,corners[2].y,0));
            warpermesh.setVertex(3,ofVec3f(corners[3].x,corners[3].y,0));
            googleimage.getTextureReference().bind();
            warpermesh.draw();
            googleimage.getTextureReference().unbind();
        }
        
        if (detectmarkerid==1) {
            ofDrawBitmapString("oracle detected",10,80);
            vector<ofPoint> corners;
            artk.getDetectedMarkerBorderCorners(i, corners);
            warpermesh.setVertex(0,ofVec3f(corners[0].x,corners[0].y,0));
            warpermesh.setVertex(1,ofVec3f(corners[1].x,corners[1].y,0));
            warpermesh.setVertex(2,ofVec3f(corners[2].x,corners[2].y,0));
            warpermesh.setVertex(3,ofVec3f(corners[3].x,corners[3].y,0));
            oracleimage.getTextureReference().bind();
            warpermesh.draw();
            oracleimage.getTextureReference().unbind();
        }
    }
    
    // ARTK 3D stuff
    // This is another way of drawing objects aligned with the marker
    // First apply the projection matrix once
    ofPushMatrix();
    artk.applyProjectionMatrix(640,480);
    
    // Find out how many markers have been detected
    int numDetected = artk.getNumDetectedMarkers();
    // Draw for each marker discovered
    for(int i=0; i<numDetected; i++) {
        
        // Set the matrix to the perspective of this marker
        // The origin is in the middle of the marker
        
        artk.applyModelMatrix(i);
        
        ofSetLineWidth(5);
        ofSetColor(255,0,0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 50);
        glEnd();
        
    }
    
    ofPopMatrix();

    ofSetColor(255,255,255);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == OF_KEY_UP) {
        artk.setThreshold(++threshold);
        
    } else if(key == OF_KEY_DOWN) {
        artk.setThreshold(--threshold);
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
