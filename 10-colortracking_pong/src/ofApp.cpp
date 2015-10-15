#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Color Tracking Pong");
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
    
    rgb.allocate(grabber.width, grabber.height);
    background.allocate(grabber.width, grabber.height);
    grayscale.allocate(grabber.width, grabber.height);
    roi.allocate(grabber.width, grabber.height);
    red.allocate(grabber.width, grabber.height);
    green.allocate(grabber.width, grabber.height);
    blue.allocate(grabber.width, grabber.height);
    difference.allocate(grabber.width, grabber.height);
    
    // Setup the ball
    // Using ball.z as radius
    
    ball.set(ofGetWidth()*0.5, 20, 20);
    velocity.set(0, 80);
    
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
        
        for(int i=0; i<grabber.width*grabber.height; i++){
            
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
        
        contour.findContours(difference, 250, grabber.width*grabber.height, 2, false);
        
    }
    
    updateBallPosition();

}

//--------------------------------------------------------------
void ofApp::updateBallPosition(){

    if(contour.nBlobs == 2){
        
        int width = ofGetWidth();
        int height = ofGetWidth()/4*3;
    
        ofVec2f v1, v2;
     
        v1.x = ofMap(contour.blobs[0].centroid.x, 0, grabber.width, 0, width);
        v1.y = ofMap(contour.blobs[0].centroid.y, 0, grabber.height, 0, height);
        v2.x = ofMap(contour.blobs[1].centroid.x, 0, grabber.width, 0, width);
        v2.y = ofMap(contour.blobs[1].centroid.y, 0, grabber.height, 0, height);

        // Reverse velocity.y if the ball is between
        // both blobs & the y-position is greater then
        // the midpoint between both blobs
        // Note: quick & dirty hack :)
        
        if(ball.y > v2.y - (v2.y - v1.y)/2) {
            
            if(v1.x < v2.x && ball.x < v2.x && ball.x > v1.x) velocity.y *= -1;
            if(v2.x < v1.x && ball.x < v1.x && ball.x > v2.x) velocity.y *= -1;
            
        }

    }

    // Add velocity and acceleration
    
    ball += velocity * (ball.y / ofGetHeight());
    
    // Set boundaries
    
    if(ball.y > ofGetHeight()-ball.z) ball.y = ball.z;
    if(ball.y < ball.z && velocity.y < 0) velocity.y *= -1;
    
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
    
    // Draw pong bar

    if(contour.nBlobs == 2){
        
        ofVec2f v1, v2;
        
        v1.x = ofMap(contour.blobs[0].centroid.x, 0, grabber.width, 0, width);
        v1.y = ofMap(contour.blobs[0].centroid.y, 0, grabber.height, 0, height);
        v2.x = ofMap(contour.blobs[1].centroid.x, 0, grabber.width, 0, width);
        v2.y = ofMap(contour.blobs[1].centroid.y, 0, grabber.height, 0, height);
        
        ofSetLineWidth(10);
        ofCircle(v1, 10);
        ofCircle(v2, 10);
        ofLine(v1, v2);
    
    }

    drawBall();
    ofPopStyle();
    ofPopMatrix();
    
    debugDraw();
    
}

//--------------------------------------------------------------
void ofApp::drawBall(){
    
    ofPushMatrix();
    ofSetCircleResolution(72);
    ofSetColor(255, 255, 0);
    ofCircle(ball.x, ball.y, ball.z);
    ofPopMatrix();
    
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
    
    x = ofMap(float(x), 0, ofGetWidth(), 0, grabber.width);
    y = ofMap(float(y), 0, ofGetWidth()/4*3, 0, grabber.height);
    
    // Bail out if the mapping
    // falls outsode the image
    
    if(x>grabber.width) return;
    if(y>grabber.height) return;

    // Get color values
    // and cache them
    
    int r = red.getPixels()[y*grabber.width + x];
    int g = green.getPixels()[y*grabber.width + x];
    int b = blue.getPixels()[y*grabber.width + x];
    
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
