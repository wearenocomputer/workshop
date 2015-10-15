#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Mesh From Camera example");
    ofBackground(33);
    
    // OF comes with a meshFromCamera example (examples/3d/meshFromCamera)
    // which doesn't need ofImage and adds every pixel to the mesh.
    // This is an alternative method which allows us to set a step
    // size, so we're not using every single pixel in the feed.
    // NOTE: using textures & OFprimitives has better performance...
    
    // Set params
    
    wireframe = true;
    width = 320;
    height = 240;
    step = 10;
    
    // Initialize grabber
    // and allocate ofImages
    
    grabber.initGrabber(width, height);
    rgb.allocate(width, height, OF_IMAGE_COLOR);
    
    // Create mesh
    
    int _width = width/step;
    int _height = height/step;
    
    for(int y=0; y<_height; y++){
        for(int x=0; x<_width; x++){
            
            float size = 15;
            float _x = (x * size) - (_width * size * 0.5);
            float _y = (y * size) - (_height * size * 0.5);
            float _z = 0;
            
            mesh.addVertex(ofVec3f(_x, _y, _z));
            mesh.addColor(ofColor(0));
            
        }
    }
    
    
    // Add indices to create
    // the faces of our mesh
    
    for(int y=0; y<_height-1; y++){
        for(int x=0; x<_width-1; x++){
            
            mesh.addIndex(x+y*_width);         // 0
            mesh.addIndex((x+1)+y*_width);     // 1
            mesh.addIndex(x+(y+1)*_width);     // 10
            
            mesh.addIndex((x+1)+y*_width);     // 1
            mesh.addIndex((x+1)+(y+1)*_width); // 11
            mesh.addIndex(x+(y+1)*_width);     // 10
            
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
        
        // Update vertex position and colors
        // being used to make the drawing

        int index = 0;
        for(int y=0; y<height; y+=step){
            for(int x=0; x<width; x+=step){
        
     
                ofVec3f vertex = mesh.getVertex(index);
                ofColor color = rgb.getColor(x, y);
            
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
void ofApp::draw(){
    
    debugDraw();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    ofScale(1.5, 1.5, 1.5);
    ofEnableDepthTest();

    if(wireframe) mesh.drawWireframe();
    else mesh.drawFaces();
    
    ofDisableDepthTest();
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::debugDraw(){
    
    ofPushMatrix();
    ofTranslate(20, 30);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 0, 0);
    
    grabber.draw(0, 20, 320, 240);
    rgb.draw(0, 280, 320, 240);
    
    ofDrawBitmapString("WIREFRAME: " + ofToString(wireframe) + "\ntoggle 'w' to change", 0, 545);
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if(key == 'w') wireframe = !wireframe;
    
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
