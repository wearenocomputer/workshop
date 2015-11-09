#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Color Tracking Mandala");
    ofBackground(33);
    
    // Set params
    
    threshold = 30;
    range = 30;
    subtraction = false;
    dilate = false;
    erode = true;
    blur = true;

    // Initialize grabber and
    // allocate OpenCv images
    
    grabber.initGrabber(320, 240);
    
    rgb.allocate(grabber.getWidth(), grabber.getHeight());
    background.allocate(grabber.getWidth(), grabber.getHeight());
    grayscale.allocate(grabber.getWidth(), grabber.getHeight());
    roi.allocate(grabber.getWidth(), grabber.getHeight());
    red.allocate(grabber.getWidth(), grabber.getHeight());
    green.allocate(grabber.getWidth(), grabber.getHeight());
    blue.allocate(grabber.getWidth(), grabber.getHeight());
    difference.allocate(grabber.getWidth(), grabber.getHeight());
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Grab the camera feed
    // and check for new frames
    
    grabber.update();
    if(grabber.isFrameNew()){
        
        // Get pixel data and
        // pre-process the image
        
        rgb.setFromPixels(grabber.getPixels(), grabber.getWidth(), grabber.getHeight());
        rgb.mirror(false, true);
        
        if(dilate) rgb.dilate();
        if(erode) rgb.erode();
        if(blur) rgb.blur(11);

        // Create a grayscale image
        // for each color channel
        
        rgb.convertToGrayscalePlanarImages(red, green, blue);

        // Convert to grayscale
        
        grayscale = rgb;

        // Perform background subtraction
        // We only track colors inside our ROI
        
        roi.absDiff(background, grayscale);
        roi.threshold(threshold);

        // Loop through all pixels
        // and calculate difference
        
        ofColor min = ofColor(color - range);
        ofColor max = ofColor(color + range);
        
        for(int i=0; i<grabber.getWidth()*grabber.getHeight(); i++){
            
            bool hasR = ofInRange(red.getPixels()[i], min.r, max.r);
            bool hasG = ofInRange(green.getPixels()[i], min.g, max.g);
            bool hasB = ofInRange(blue.getPixels()[i], min.b, max.b);
            
            int result = hasR && hasG && hasB ? 255 : 0;
            
            // If the result is not inside
            // our ROI, color the pixel black
            
            if(subtraction) result = roi.getPixels()[i] == 0 ? 0 : result;
            
            difference.getPixels()[i] = result;
            
        }
        
        difference.flagImageChanged();

        // Find blobs in the diffrence image
        // input, min_size, max_size, how many?, find holes?
        
        contour.findContours(difference, 250, grabber.getWidth()*grabber.getHeight(), 1, false);
        
        // Add blobs to our vector
        // and auto-remove as needed
        
        if(contour.nBlobs > 0) particles.push_back(contour.blobs[0].centroid * 1.25);
        else if(particles.size() > 1) particles.erase(particles.begin());
        
        if(particles.size() > 35) particles.erase(particles.begin());
        
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // Map aspect ratio of the
    // video feed to the screen
    
    int width = ofGetWidth();
    int height = ofGetWidth()/4*3;
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255, 255, 255, 100);
    grabber.draw(width, 0, -width, height);
    ofSetColor(255);
    
    // Draw some particles

    ofTranslate(ofGetWidth()*0.55, ofGetHeight()*0.5);
    for(int i=0; i<360; i+= 10) {
        
        ofPushMatrix();
        ofRotateZ(i);

        for(int p=particles.size()-1; p>-1; p--){
            
            ofVec2f &v = particles[p];
            
            float radius = float(p)/float(particles.size());
            float alpha = radius * 255;
            
            ofSetColor(v.x*0.75, v.y*0.75, 255, alpha);
            ofCircle(v, radius * 15);
            
            // Draw connecting lines
        
            if(p < particles.size()-1) {
                ofSetColor(255, 255, 255, alpha);
                ofLine(v, particles[p+1]);
            }
            
            if(p < particles.size()-7) {
                ofSetColor(255, 255, 255, alpha);
                ofLine(v, particles[p+7]);
            }
            
        }
        
        ofPopMatrix();
    }

    ofPopStyle();
    ofPopMatrix();
    
    debugDraw();
    
}

//--------------------------------------------------------------
void ofApp::debugDraw(){
    
    ofPushMatrix();
    ofTranslate(20, 30);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 0, 0);
    
    rgb.draw(0, 20, 160, 120);
    if(subtraction) roi.draw(180, 20, 160, 120);
    
    ofPushStyle();
    ofSetColor(255, 0, 0);
    red.draw(0, 160, 160, 120);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(0, 255, 0);
    green.draw(0, 300, 160, 120);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(0, 0, 255);
    blue.draw(0, 440, 160, 120);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(color);
    ofRect(180, 160, 160, 120);
    ofPopStyle();
    
    difference.draw(180, 300, 160, 120);

    ofDrawBitmapString("THRESHOLD: " + ofToString(threshold) + "\nuse up/down keys to change\n\nCOLOR RANGE: " + ofToString(range) + "\nuse left/right keys to change\n\nPRE-PROCESS\npress 1 to 3 to toggle filters\n\nBG SUBTRACTION: "+ (subtraction == 0 ? "false" : "true") +"\npress 4 to toggle", 0, 595);
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == ' ') background = grayscale;
    if(key == OF_KEY_DOWN) threshold--;
    if(key == OF_KEY_UP) threshold++;
    if(key == OF_KEY_LEFT) range--;
    if(key == OF_KEY_RIGHT) range++;
    
    if(key == '1') dilate = !dilate;
    if(key == '2') erode = !erode;
    if(key == '3') blur = !blur;
    if(key == '4') subtraction = !subtraction;

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

    // Map mouse positions to match
    // the rgb-image from the camera
    
    x = ofMap(float(x), 0, ofGetWidth(), 0, grabber.getWidth());
    y = ofMap(float(y), 0, ofGetWidth()/4*3, 0, grabber.getHeight());
    
    // Bail out if the mapping
    // falls outsode the image
    
    if(x>grabber.getWidth()) return;
    if(y>grabber.getHeight()) return;

    // Get color values
    // and cache them
    
    int r = red.getPixels()[y*grabber.getWidth() + x];
    int g = green.getPixels()[y*grabber.getWidth() + x];
    int b = blue.getPixels()[y*grabber.getWidth() + x];
    
    color = ofColor(r, g, b);
    
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
