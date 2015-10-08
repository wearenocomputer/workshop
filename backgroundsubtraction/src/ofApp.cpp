#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Background Subtraction example");
    ofBackground(33, 33, 33);
    
    // Set params
    
    width = 320;
    height = 240;
    threshold = 60;

    // Initialize grabber
    // and allocate ofImages
    
    grabber.initGrabber(width, height);
    
    rgb.allocate(width, height, OF_IMAGE_COLOR);
    grayscale.allocate(width, height, OF_IMAGE_GRAYSCALE);
    background.allocate(width, height, OF_IMAGE_GRAYSCALE);
    difference.allocate(width, height, OF_IMAGE_GRAYSCALE);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Grab the camera feed
    // and check for new frames
    
    grabber.update();
    if(grabber.isFrameNew()){
        
        getRGBPixels();
        convertToGrayscale();
        getDifference();
        backgroundLearning();
        
    }

}

//--------------------------------------------------------------
void ofApp::getRGBPixels(){
    
    // Get raw pixel data and
    // draw it into an ofImage
    
    unsigned char *pixels = grabber.getPixels();
    for(int i=0; i<width*height*3; i+=3){
        
        float r = pixels[i];
        float g = pixels[i+1];
        float b = pixels[i+2];
        
        rgb.setColor(i, ofColor(r, g, b));
        
    }
    
    // Mirror horizontally and
    // update the base image
    
    rgb.mirror(false, true);
    rgb.update();
    
}

//--------------------------------------------------------------
void ofApp::convertToGrayscale(){
   
    for(int x=0; x<width; x++){
        for(int y=0; y<height; y++){
            
            ofColor color = rgb.getColor(x, y);
            
            float r = color.r;
            float g = color.g;
            float b = color.b;
            
            // Convert to grayscale values
            // http://www.tannerhelland.com/3643/grayscale-image-algorithm-vb6/
            
            float gray = (0.3 * r) + (0.59 * g) + (0.11 * b);
            
            // Or use built-in OF methods
            // float brightness = rgb.getColor(x, y).getBrightness();
            // float lightness = rgb.getColor(x, y).getLightness();
            
            grayscale.setColor(x, y, ofColor(gray));
            
        }
    }
    
    grayscale.update();
    
}

//--------------------------------------------------------------
void ofApp::getDifference(){
    
    for(int x=0; x<width; x++){
        for(int y=0; y<height; y++){
            
            ofColor gray = grayscale.getColor(x, y);
            ofColor bg = background.getColor(x, y);
            
            // Calculate the difference for each
            // pixel by subtracting the 2 images
            
            float r = abs(bg.r - gray.r) > threshold ? 255 : 0;
            float g = abs(bg.g - gray.g) > threshold ? 255 : 0;
            float b = abs(bg.b - gray.b) > threshold ? 255 : 0;
            
            difference.setColor(x, y, ofColor(r, g, b));
            
        }
    }
    
    difference.update();
    
}

//--------------------------------------------------------------
void ofApp::backgroundLearning(){
    
    for(int x=0; x<width; x++){
        for(int y=0; y<height; y++){
            
            ofColor gray = grayscale.getColor(x, y);
            ofColor bg = background.getColor(x, y);
            
            // Mix the grayscale image with background
            // to get a smoother transfer (trails)
            
            float result = (bg.b * 0.99) + (gray.b * 0.01);
            
            background.setColor(x, y, ofColor(result));
            
        }
    }
    
    background.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    debugDraw();
    
    ofPushMatrix();
    ofTranslate(360, 310);
    
    difference.draw(0, 0, 320, 240);
    
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
    
    ofDrawBitmapString("THRESHOLD: " + ofToString(threshold) + "\nuse up/down keys to change", 680, 295);
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == ' ') background = grayscale;
    if(key == OF_KEY_DOWN) threshold--;
    if(key == OF_KEY_UP) threshold++;

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
