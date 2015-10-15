#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Blob Detection example");
    ofBackground(33);
    
    // Set params
    
    threshold = 30;
    dilate = false;
    erode = false;
    blur = false;

    // Initialize grabber and
    // allocate OpenCv images
    
    grabber.initGrabber(320, 240);
    
    rgb.allocate(grabber.width, grabber.height);
    grayscale.allocate(grabber.width, grabber.height);
    background.allocate(grabber.width, grabber.height);
    difference.allocate(grabber.width, grabber.height);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Grab the camera feed
    // and check for new frames
    
    grabber.update();
    if(grabber.isFrameNew()){
        
        // Get pixel data and
        // pre-process the image
        
        rgb.setFromPixels(grabber.getPixels(), grabber.width, grabber.height);
        rgb.mirror(false, true);
        
        if(dilate) rgb.dilate();
        if(erode) rgb.erode();
        if(blur) rgb.blur(33);
        
        // Convert to grayscale
        
        grayscale = rgb;
        
        // Get difference
        
        difference.absDiff(background, grayscale);
        difference.threshold(threshold);
        
        // Find blobs - input, min_size, max_size, how many?, find holes?
        
        contour.findContours(difference, 2000, grabber.width*grabber.height, 3, false);
        
        
        // Background learning
        // Mix the grayscale image with background
        // to get a smoother transfer (trails)
        /*
        unsigned char *gray = grayscale.getPixels();
        unsigned char *bg = background.getPixels();
        for(int i=0; i<grabber.width*grabber.height; i++){
            
            bg[i] *= 0.9;
            bg[i] += gray[i]*0.1;
            
        }
        
        background = bg;
        */
        
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    debugDraw();
    
    ofPushMatrix();
    ofTranslate(700, 310);
    ofSetLineWidth(2);
    
    rgb.draw(0, 0, 320, 240);
    contour.draw(0, 0, 320, 240);
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::debugDraw(){
    
    ofPushMatrix();
    ofTranslate(20, 30);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 0, 0);
    
    grabber.draw(0, 20, 320, 240);
    rgb.draw(340, 20, 320, 240);
    grayscale.draw(680, 20, 320, 240);

    background.draw(0, 280, 320, 240);
    difference.draw(340, 280, 320, 240);
    
    ofDrawBitmapString("THRESHOLD: " + ofToString(threshold) + "\nuse up/down keys to change\n\nPRE-PROCESS\npress 1 to 3 to toggle filters", 1020, 295);
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == ' ') background = grayscale;
    if(key == OF_KEY_DOWN) threshold--;
    if(key == OF_KEY_UP) threshold++;
    
    if(key == '1') dilate = !dilate;
    if(key == '2') erode = !erode;
    if(key == '3') blur = !blur;

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
