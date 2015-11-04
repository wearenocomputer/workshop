#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(33, 33, 33);
    //setup cam
    cam.setDistance(10);
    cam.setNearClip(0.1);
    
    //setup plane
    plane.setCenter(ofVec3f(0.0f, 0.0f, 0.0f));
    plane.setNormal(ofVec3f(0.0f, 0.0f, 1.0f).normalize());
    plane.setUp(ofVec3f(0.0f, 1.0f, 0.0f).normalize());
    plane.setScale(ofVec2f(1.0f, 1.0f));
    plane.setInfinite(true);
    plane.color = ofColor(255,0,0);
    
    //setup ray
    ray = ofRay(ofVec3f(0,0,0), ofVec3f(0,0,0),ofColor::green,false);
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
   
    
    ofEnableDepthTest();
    cam.begin();
    

    plane.draw();
    ray.draw();
    
    GLdouble near[3];
    GLdouble far[3];
    
    getMousePositionToNearPlane(mouseX, mouseY, near);
    getMousePositionToFarPlane(mouseX, mouseY, far);
    
    ofVec3f nearpoint = ofVec3f(near[0],near[1],near[2]);
    ofVec3f farpoint = ofVec3f(far[0],far[1],far[2]);
    
    ray.s = nearpoint;
    ray.t = farpoint;
    
    ofVec3f intersectionpoint;
    plane.intersect(ray,intersectionpoint);
    
    ofDrawGrid();
    ofDrawSphere(intersectionpoint, 1);
    
    cam.end();
    
    ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='f') {
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


void ofApp::getMousePositionToNearPlane(float mousex, float mousey, GLdouble nearPlaneLocation[3]) {
    GLdouble modelview[16], projection[16];
    GLint viewport[4];
    
    //get the viewport
    glGetIntegerv( GL_VIEWPORT, viewport );
    //get the projection matrix
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    //get the modelview matrix
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    
    mousey = (float)viewport[3] - mousey;
    
    gluUnProject(mousex, mousey, 0.0, modelview, projection,
                 viewport, &nearPlaneLocation[0], &nearPlaneLocation[1],
                 &nearPlaneLocation[2]);

}


void ofApp::getMousePositionToFarPlane(float mousex, float mousey, GLdouble farPlaneLocation[3]) {
    GLdouble modelview[16], projection[16];
    GLint viewport[4];
    
    //get the viewport
    glGetIntegerv( GL_VIEWPORT, viewport );
    //get the projection matrix
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    //get the modelview matrix
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    
    mousey = (float)viewport[3] - mousey;
    
    gluUnProject(mousex, mousey, 1.0, modelview, projection,
                 viewport, &farPlaneLocation[0], &farPlaneLocation[1],
                 &farPlaneLocation[2]);
}
