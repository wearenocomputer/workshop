#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Blob Detection Motion");
    ofBackground(33, 33, 33);
    
    // Set params
    
    threshold = 30;
    dilate = false;
    erode = false;
    blur = false;
    breathe = false;

    // Initialize grabber and
    // allocate OpenCv images
    
    grabber.initGrabber(320, 240);
    
    rgb.allocate(grabber.width, grabber.height);
    grayscale.allocate(grabber.width, grabber.height);
    background.allocate(grabber.width, grabber.height);
    difference.allocate(grabber.width, grabber.height);
    
    // Add some particles and
    // attractors to our flock
    
    for(int i=0; i<1000; i++){
        
        float mass = ofRandom(1, 7);
        float x = ofRandom(-5, 5);
        float y = ofRandom(-5, 5);
        float z = 0;
        
        flock.addParticle(Particle(ofVec3f(x,y,z), mass));
        
    }
    
    flock.addAttractor(Particle(ofVec3f(0,0,0), 75));
    flock.addAttractor(Particle(ofVec3f(0,0,0), 75));
    flock.addAttractor(Particle(ofVec3f(0,0,0), 75));
    
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
        
        contour.findContours(difference, 1000, grabber.width*grabber.height, 1, false);
        
        
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
    
    updateFlock();

}

//--------------------------------------------------------------
void ofApp::updateFlock(){
    
    // Attractors will always steer
    // towards the center (0, 0, 0)
    
    ofVec3f target = ofVec3f(0, 0, 0);
    
    // If there were blobs found
    // follow the first one instead

    if(contour.nBlobs>0){
        
        float x = contour.blobs[0].centroid.x / grabber.width * ofGetWidth();
        float y = contour.blobs[0].centroid.y / grabber.height * ofGetHeight();
    
        target = ofVec3f(x - ofGetWidth()*0.5, y - ofGetHeight()*0.5, 0);
        
    }
    
    // Calculate target
    
    target = ofVec3f((target - flock.attractors[0].position) * 0.075);
    
    // Update attractors
    
    flock.attractors[0].position += target;
    flock.attractors[1].position += target;
    flock.attractors[2].position += target;
    
    // Add some motion
    // to the attractors
    
    if(breathe){
        for(int i=0; i<flock.attractors.size(); i++){
            
            Particle &p = flock.attractors[i];
            p.mass = p.unweightedMass * flock.mass;
            p.mass += cos(ofGetFrameNum() * (0.05 * i)) * (15 * i);
            
        }
    }
    
    flock.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    debugDraw();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.6, ofGetHeight()*0.5);

    flock.draw();
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::debugDraw(){
    
    ofPushMatrix();
    ofTranslate(20, 30);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 0, 0);
    
    grayscale.draw(0, 20, 320, 240);
    background.draw(0, 280, 320, 240);
    difference.draw(0, 540, 320, 240);
    contour.draw(0, 540, 320, 240);
    
    ofDrawBitmapString("THRESHOLD: " + ofToString(threshold) + "\nuse up/down keys to change\n\nPRE-PROCESS\npress 1 to 3 to toggle filters\n\nToggle 4 to make it breathe", 0, 810);
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
    if(key == '4') breathe =!breathe;

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
