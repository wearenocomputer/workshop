#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Color Tracking example");
    ofBackground(33, 33, 33);
    
    // Set params
    
    threshold = 30;
    range = 20;
    dilate = false;
    erode = false;
    blur = false;
    channel = "red";

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

        // Convert to grayscale and get difference
        // We only track colors inside our ROI
        
        grayscale = rgb;
        
        roi.absDiff(background, grayscale);
        roi.threshold(threshold);

        // Set base channel to
        // calculate difference
        
        unsigned char *pixels;
        
        if(channel == "red") pixels = red.getPixels();
        if(channel == "green") pixels = green.getPixels();
        if(channel == "blue") pixels = blue.getPixels();
        
        // Get min & max
        // offset values
        
        int min = color.r - range;
        int max = color.r + range;
        
        // Loop through all pixels
        // and calculate difference
        
        for(int i=0; i<grabber.width*grabber.height; i++){
            
            int result = ofInRange(pixels[i], min, max) ? 255 : 0;
            
            // Check if the result is inside our ROI
            // and push the result to our pixel array
            
            result = roi.getPixels()[i] == 0 ? 0 : result;
            
            difference.getPixels()[i] = result;
            
        }
        
        difference.flagImageChanged();

        // Find blobs in the diffrence image
        // input, min_size, max_size, how many?, find holes?
        
        contour.findContours(difference, 250, grabber.width*grabber.height, 3, false);
        
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
    ofSetLineWidth(2);
    //ofSetColor(255, 255, 255, 200);
    grabber.draw(width, 0, -width, height);
    //ofSetColor(255);
    contour.draw(0, 0, width, height);
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
    roi.draw(180, 20, 160, 120);
    
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

    ofDrawBitmapString("THRESHOLD: " + ofToString(threshold) + "\nuse up/down keys to change\n\nRANGE: " + ofToString(range) + "\nuse left/right keys to change\n\nPRE-PROCESS\npress 1 to 3 to toggle filters\n\nBASE CHANNEL: " + channel + "\npress 4 to 6 to toggle channel", 180, 455);
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
    
    if(key == '4') channel = "red";
    if(key == '5') channel = "green";
    if(key == '6') channel = "blue";

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
