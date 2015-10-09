#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Background Subtraction Grid");
    ofBackground(33, 33, 33);
    
    // Set params
    
    width = 320;
    height = 240;
    threshold = 30;

    // Initialize grabber
    // and allocate ofImages
    
    grabber.initGrabber(width, height);
    
    rgb.allocate(width, height, OF_IMAGE_COLOR);
    grayscale.allocate(width, height, OF_IMAGE_GRAYSCALE);
    background.allocate(width, height, OF_IMAGE_GRAYSCALE);
    difference.allocate(width, height, OF_IMAGE_COLOR);
    
    // Create mesh

    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            
            float size = 5;
            float _x = (x * size) - (width * size * 0.5);
            float _y = (y * size) - (height * size * 0.5);
            
            mesh.addVertex(ofVec3f(_x, _y, 0));
            mesh.addColor(ofColor(0));
            
        }
    }
    
    // Add indices to create
    // the faces of our mesh
    
    for(int y = 0; y<height-1; y++){
        for(int x=0; x<width-1; x++){
            
            mesh.addIndex(x+y*width);         // 0
            mesh.addIndex((x+1)+y*width);     // 1
            mesh.addIndex(x+(y+1)*width);     // 10
            
            mesh.addIndex((x+1)+y*width);     // 1
            mesh.addIndex((x+1)+(y+1)*width); // 11
            mesh.addIndex(x+(y+1)*width);     // 10
            
        }
    }
    
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
    
        
        // Update vertex position and colors
        // being used to make the drawing
        
        int index = 0;
        for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){
                
                ofVec3f vertex = mesh.getVertex(index);
                ofColor color = difference.getColor(x, y);
                
                // Make transparent where
                // the image is black (= no movement)
                
                if(color == 0) color.a = 0;
                
                // Get average color values and extrude along the z-axis
                // Mix this with previous z-positions to get an easing effect
                
                float z = (color.r + color.g + color.b) * 0.5;
                vertex.z = (vertex.z * 0.6) + (z * 0.4);
                
                // Update
                
                mesh.setVertex(index, vertex);
                mesh.setColor(index, color);
                
                index++;
                
            }
        }
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
            
            ofColor color = rgb.getColor(x, y);
            ofColor gray = grayscale.getColor(x, y);
            ofColor bg = background.getColor(x, y);
            
            // Calculate the difference for each
            // pixel by subtracting the 2 images
            
            float r = abs(bg.r - gray.r) > threshold ? color.r : 0;
            float g = abs(bg.g - gray.g) > threshold ? color.g : 0;
            float b = abs(bg.b - gray.b) > threshold ? color.b : 0;
            
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
            
            float result = (bg.b * 0.965) + (gray.b * 0.035);
            
            background.setColor(x, y, ofColor(result));
            
        }
    }
    
    background.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    debugDraw();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    
    //mesh.drawFaces();
    
    // Draw rectangles on each vertex position
    // with a radius based on the z-position
    
    for(int i=0; i<mesh.getNumVertices(); i++){
        
        ofVec3f vertex = mesh.getVertex(i);
        ofColor color = mesh.getColor(i);

        i%9 == 0 ? ofFill() : ofNoFill();
        
        ofSetColor(color);
        ofSetCircleResolution( i%2 == 0 ? 4 : 3 );
        
        ofPushMatrix();
        ofTranslate(vertex);
        ofRotateZ(ofGetFrameNum() * i * 0.25);
        ofCircle(0, 0, vertex.z * 0.1);
        ofPopMatrix();
        
    }

    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::debugDraw(){
    
    ofPushMatrix();
    ofTranslate(20, 30);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 0, 0);
    ofSetColor(255);
    
    rgb.draw(0, 20, 320, 240);
    difference.draw(0, 280, 320, 240);
    
    ofDrawBitmapString("THRESHOLD: " + ofToString(threshold) + "\nuse up/down keys to change", 0, 550);
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
