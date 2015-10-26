#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(33, 33, 33);
    niContext.setup();
    //niContext.setupUsingRecording("hammerswing.oni");
    niDepthGenerator.setup( &niContext );
    niImageGenerator.setup( &niContext );
    niUserGenerator.setup( &niContext );
    niContext.setMirror(true);
    niContext.registerViewport();
    
    skeleton.setup();

}

//--------------------------------------------------------------
void ofApp::update(){
    niContext.update();
    niDepthGenerator.update();
    niImageGenerator.update();
    niUserGenerator.update();
    
    
    trackUser();

}

//--------------------------------------------------------------
void ofApp::draw(){
    niImageGenerator.draw(20,20,320,240);
    niDepthGenerator.draw(360,20,320,240);
    cam.begin();
    ofEnableDepthTest();
    skeleton.draw();
    ofDisableDepthTest();
    cam.end();
}

void ofApp::trackUser() {
    
    for( int i = 0; i < niUserGenerator.getNumberOfTrackedUsers() ; i++ ) {
        ofxTrackedUser* tracked = niUserGenerator.getTrackedUser(i+1);
        
        if(niUserGenerator.getXnUserGenerator().GetSkeletonCap().IsTracking(tracked->id) ) {
            
            xn::SkeletonCapability pUserSkel = niUserGenerator.getXnUserGenerator().GetSkeletonCap();
            pUserSkel.SetSmoothing(0.6);
            
            skeleton.updateSkeleton(niUserGenerator, *tracked);
          
        }
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='f'){
        ofToggleFullscreen();
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
