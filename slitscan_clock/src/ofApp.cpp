#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Slitscan Clock");
    ofBackground(33, 33, 33);
    
    // Init grabber
    
    grabber.initGrabber(320, 240);
    
    // Init FBO
    // FBO sizes need to be a power of 2
    // Otherwise openFrameworks will scale
    // our FBO, which results in incorrect
    // mapping of texture coordinates
    
    fboSize = 512;

    createMeshes();
    
}

//--------------------------------------------------------------
void ofApp::createMeshes(){
    
    meshes.clear();
    fbos.clear();
    indexes.clear();
    
    // Then create some stuff
    // Each mesh has its own fbo
    // and an offset position
    
    for(int i=0; i<3; i++) {
        
        ofVboMesh mesh;
        ofFbo fbo;
        
        createMesh(mesh, ofRandom(3, 120), i);
        createFbo(fbo);
        
        meshes.push_back(mesh);
        fbos.push_back(fbo);
        indexes.push_back(0);

    }
    
}

//--------------------------------------------------------------
void ofApp::createMesh(ofVboMesh &mesh, int segments, int scale){
    
    float innerRadius = 60 + (100 * scale);
    float outerRadius = 150 + (100 * scale);
    
    int step = 360/segments;
    int max = 360; // degrees
    
    // Create vertices
    
    for(int i=0; i<max; i+=step){
        
        float x1 = cos(i * DEG_TO_RAD) * innerRadius;
        float y1 = sin(i * DEG_TO_RAD) * innerRadius;
        
        float x2 = cos(i * DEG_TO_RAD) * outerRadius;
        float y2 = sin(i * DEG_TO_RAD) * outerRadius;
        
        // Create some fancy shapes by playing round with offsets
        //float x2 = cos(i * DEG_TO_RAD) * (outerRadius + cos(i * DEG_TO_RAD * 6) * 10 * (scale + 1));
        //float y2 = sin(i * DEG_TO_RAD) * (outerRadius + cos(i * DEG_TO_RAD * 6) * 10 * (scale + 1));
        
        float tx = ofMap(i, 0, max, 0, fboSize);
        
        mesh.addVertex(ofVec2f(x1, y1));
        mesh.addColor(ofColor(255, 255, 255, 255));
        mesh.addTexCoord(ofVec2f(tx, 0));
        
        mesh.addVertex(ofVec2f(x2, y2));
        mesh.addColor(ofColor(255, 255, 255, 255));
        mesh.addTexCoord(ofVec2f(tx, fboSize));
        
    }
    
    // Create faces
    
    int vertices = mesh.getNumVertices();
    
    for(int i=3; i<vertices; i+=2){
        
        mesh.addTriangle(i, i-1, i-2);
        mesh.addTriangle(i-1, i-2, i-3);
        
    }
    
    // Add closing faces
    
    mesh.addTriangle(1, 0, vertices-1);
    mesh.addTriangle(0, vertices-1, vertices-2);
    
}

//--------------------------------------------------------------
void ofApp::createFbo(ofFbo &fbo){
    
    fbo.allocate(fboSize, fboSize);
    fbo.begin();
        
    // Remove fbo artefacts in
    // memory by clearing it
        
    ofClear(255,255,255, 5);
    fbo.end();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Grab the camera feed
    // and check for new frames
    
    grabber.update();
    if(grabber.isFrameNew()){
        
        // And draw a slice of the
        // new frame to the FBO
        
        float slice = grabber.getWidth() / float(fboSize);
        float offset = grabber.getWidth() * 0.5;

        // Draw the first mesh
        // in our vector
        
        fbos[0].begin();
        grabber.getTextureReference().drawSubsection(indexes[0], 0, slice, fboSize, offset, 0, 1, grabber.getHeight());
        fbos[0].end();
        
        // Increase index or
        // x-offset position
        
        indexes[0] < fboSize ? indexes[0]++ : indexes[0]=0;
     
        // Then check when we need to draw
        // the other meshes (@slower pace)
        
        for(int i=1; i<fbos.size(); i++){
         
            ofFbo &fbo = fbos[i];
            int &index = indexes[i];
        
            if(indexes[0]%(2*i) == 0) {
                
                fbo.begin();
                grabber.getTextureReference().drawSubsection(index, 0, slice, fboSize, offset, 0, 1, grabber.getHeight());
                fbo.end();
                
                // Increase index or
                // x-offset position
                
                index < fboSize ? index++ : index=0;
                
            }
        }
 
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    debugDraw();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    //ofRotateZ(ofGetFrameNum() * 0.15);
    //ofRotateX(ofGetFrameNum() * -0.15);
    
    for(int i=0; i<meshes.size(); i++){
        
        ofVboMesh &mesh = meshes[i];
        ofFbo &fbo = fbos[i];
        
        // Bind textures
        // and draw the mesh
        //mesh.drawWireframe();
        
        fbo.getTextureReference().bind();
        mesh.draw();
        fbo.getTextureReference().bind();

    }
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::debugDraw(){
    
    ofPushMatrix();
    ofTranslate(20, 30);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 0, 0);
    
    grabber.draw(0, 20, 320, 240);
    
    for(int i=0; i<fbos.size(); i++) {
        
        ofFbo &fbo = fbos[i];
        fbo.draw(0, 280+(140*i), 320, 120);
        
    }
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    createMeshes();
    
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
