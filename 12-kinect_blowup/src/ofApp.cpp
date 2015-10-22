#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("Kinect Blowup");
    ofBackground(33);
    
    // OF comes with a kinect example (examples/addons/kinectExample)
    // This builds upon that...
    
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    kinect.setCameraTiltAngle(15);
    
    // GUI
    
    gui.setup();
    gui.add(guiStep.setup("Step", 2, 2, 12));
    gui.add(guiNear.setup("Near", 1000, 0, 1000));
    gui.add(guiFar.setup("Far", -250, -1000, 0));
    gui.add(guiExtrude.setup("Extrude", 0, 0, 200));
    gui.add(guiHair.setup("Hair", 0, 0, 50));
   
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Grab the kinect feed
    // and check for new frames

    kinect.update();
    if(kinect.isFrameNew()){
        
        mesh.clear();

        // Get input width & height and divide
        // by the step value to create our mesh
        
        int width = kinect.width / guiStep;
        int height = kinect.height / guiStep;

        for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){

                // Get position & color
                // from kinect feed
                
                ofColor color = kinect.getColorAt(x * guiStep, y * guiStep);
                ofVec3f vertex = kinect.getWorldCoordinateAt(x * guiStep, y * guiStep);
                ofVec3f normal = ofVec3f(0, 0, 0);

                // We'll set/use alpha value to indicate wether
                // this vertex will be used in the draw method

                if(vertex.z == 0) color.a = 0;
                
                // Set depth offset
                
                vertex.z *= -1;
                vertex.z += 1000;
                
                // If the vertex depth is not
                // in range, don't draw it
                
                if(vertex.z < guiFar || vertex.z > guiNear) color.a = 0;
                
                // Add to mesh
                
                mesh.addVertex(vertex);
                mesh.addNormal(normal);
                mesh.addColor(color);

            }
        }

        // Add indices to create
        // the faces of our mesh
        
        for(int y=0; y<height-1; y++){
            for(int x=0; x<width-1; x++){
                
                mesh.addIndex(x+y*width);         // 0
                mesh.addIndex((x+1)+y*width);     // 1
                mesh.addIndex(x+(y+1)*width);     // 10
                
                mesh.addIndex((x+1)+y*width);     // 1
                mesh.addIndex((x+1)+(y+1)*width); // 11
                mesh.addIndex(x+(y+1)*width);     // 10
                
            }
        }
        
        // Calculate face normals and
        // add them to triangle vertices
        
        vector<ofIndexType> &ind = mesh.getIndices();
        vector<ofVec3f> &verts = mesh.getVertices();
        vector<ofVec3f> &norms = mesh.getNormals();
        
        for(int i=0; i<ind.size(); i+=3){
    
            int i1 = ind[i];
            int i2 = ind[i+1];
            int i3 = ind[i+2];
            
            // Calculate angle between 2 vectors and use it as a 'weight'
            // This results in 'weighted' normals, which are more accurate
            
            ofVec3f v1 = verts[i1] - verts[i2];
            ofVec3f v2 = verts[i3] - verts[i2];
            
            float weight = acos(v1.dot(v2) * pow(v1.length() * v2.length(), -1));
            
            ofVec3f normal = v2.cross(v1) * weight;
            
            // Add face normal to vertices
            
            norms[i1] += normal;
            norms[i2] += normal;
            norms[i3] += normal;
        }
        
        // Normalize vertex normals
        // and extrude faces
        
        for(int i=0; i<norms.size(); i++){
            
            norms[i].normalize();
            verts[i] += norms[i] * guiExtrude;
        
        }
 
    }


}

//--------------------------------------------------------------
void ofApp::draw(){
    
    debugDraw();
    
    ofPushMatrix();
    ofEnableDepthTest();
    ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    ofScale(-1, 1, 1);
    ofPushStyle();
    
    
    //mesh.draw();
    //mesh.drawWireframe();
    
    
    vector<ofMeshFace> faces = mesh.getUniqueFaces();
    for(int i=0; i<faces.size(); i++) {
        
        ofVec3f v1 = faces[i].getVertex(0);
        ofVec3f v2 = faces[i].getVertex(1);
        ofVec3f v3 = faces[i].getVertex(2);
        
        ofVec3f n1 = faces[i].getNormal(0);
        ofVec3f n2 = faces[i].getNormal(1);
        ofVec3f n3 = faces[i].getNormal(2);
        
        ofFloatColor c1 = faces[i].getColor(0);
        ofFloatColor c2 = faces[i].getColor(1);
        ofFloatColor c3 = faces[i].getColor(2);
        
        if(c1.a > 0 && c2.a > 0 && c3.a > 0) {
            
            ofSetColor(c1);
            ofTriangle(v1, v2, v3);
            
            // Debug draw normals
            /*
            ofSetColor(ofFloatColor(n1.x, n1.y, n1.z));
            ofLine(v1, v1 + n1*50);
            ofSetColor(ofFloatColor(n2.x, n2.y, n2.z));
            ofLine(v2, v2 + n2*50);
            ofSetColor(ofFloatColor(n3.x, n3.y, n3.z));
            ofLine(v3, v3 + n3*50);
            */
            
            // Draw hair
            
            ofVec3f start = v1;
            ofVec3f end = ofVec3f(v1 + n1);
            
            for(int j=0; j<guiHair; j++){
                
                ofLine(start, end);
                
                start = end;
                end += ofVec3f(0, ofRandom(j), 0) + n1*ofRandom(j);
                
            }
 
        }
        
    }
    
    
    ofPopStyle();
    ofDisableDepthTest();
    ofPopMatrix();
    
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::debugDraw(){
    
    ofPushMatrix();
    ofTranslate(20, 30);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 0, 0);
    
    kinect.draw(0, 20, 320, 240);
    kinect.drawDepth(0, 280, 320, 240);

    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::exit() {
    
    kinect.setCameraTiltAngle(0);
    kinect.close();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
