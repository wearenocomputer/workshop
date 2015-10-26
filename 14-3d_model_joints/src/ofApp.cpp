#include "ofApp.h"


void ofApp::fillNodeSelectorDropdown() {
    nodeselectordropdown->clearToggles();
    for (unsigned i = 0; i < model.bones.size(); ++i) {
        nodeselectordropdown->addToggle(model.bones[i].name.data);
    }
}

//-----------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e) {
    
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    if(kind == OFX_UI_WIDGET_DROPDOWNLIST) {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        
        if (ddlist->getName()=="Choose node") {
            if (ddlist->getSelectedIndeces().size()==1) {
                currentselectednode = ddlist->getSelectedNames()[0];
                ddlist->clearSelected();
            }
        }
        
        if (ddlist->getName()=="Choose model") {
            if (ddlist->getSelectedIndeces().size()==1) {
                model.setup("dae/"+ddlist->getSelectedNames()[0]);
                regularmodel.loadModel("dae/"+ddlist->getSelectedNames()[0]);
                fillNodeSelectorDropdown();

                ddlist->clearSelected();
            }
        }
    }
    
    if (kind== OFX_UI_WIDGET_SLIDER_H) {
        ofxUISlider *slider = (ofxUISlider*)e.widget;
        if (slider->getName()=="x rotation") {
            model.rotateNodeX(currentselectednode, slider->getValue());
        } else if (slider->getName()=="y rotation") {
             model.rotateNodeY(currentselectednode, slider->getValue());
        } else if(slider->getName()=="z rotation") {
             model.rotateNodeZ(currentselectednode, slider->getValue());
        }
    }
    
    if (kind== OFX_UI_WIDGET_BUTTON) {
        ofxUIButton *button = (ofxUIButton*)e.widget;

        if(!button->getValue()) {
            model.resetToBindPose();
        }
    }
    
    
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    //ofBackground(0,0,0);
    
    //DO CAM SETUP
    cambonedmodel.setDistance(50);
    cambonedmodel.setScale(1, -1, 1);
    cambonedmodel.setNearClip(0.01);
    
    camregularmodel.setDistance(50);
    camregularmodel.setScale(1, -1, 1);
    camregularmodel.setNearClip(0.01);
    camregularmodel.setFarClip(100000);

    regularcamfield.addVertex(ofPoint(ofGetWidth()-320,0));
    regularcamfield.addVertex(ofPoint(ofGetWidth(),0));
    regularcamfield.addVertex(ofPoint(ofGetWidth(),240));
    regularcamfield.addVertex(ofPoint(ofGetWidth()-320,240));
    regularcamfield.close();
    
    
    //DO MODEL SETUP
    model.setup("dae/robotExportSkinnedOneTexture2.dae");
    regularmodel.loadModel("dae/robotExportSkinnedOneTexture2.dae");
    regularmodel.setScaleNomalization(false);
    
    //CREATE GUI || NODESELECTOR
    nodeselector = new ofxUISuperCanvas("NODE SELECTOR", OFX_UI_FONT_MEDIUM);
    nodeselector->addSpacer();
    vector<string> joints;

    for (unsigned i = 0; i < model.bones.size(); ++i) {
        joints.push_back(model.bones[i].name.data);
        
    }
    nodeselectordropdown = nodeselector->addDropDownList("Choose node", joints);
    nodeselectordropdown->setAllowMultiple(false);
    nodeselectordropdown->setAutoClose(true);
    
    vector<string> fileNames; ofDirectory dir; dir.listDir("./dae"); dir.sort();
    for (unsigned i = 0; i < dir.size(); ++i) {
        fileNames.push_back(dir.getName(i));
    }
    
    modelselectordropdown = nodeselector->addDropDownList("Choose model", fileNames);
    modelselectordropdown->setAllowMultiple(false);
    modelselectordropdown->setAutoClose(true);
    
    nodeselector->autoSizeToFitWidgets();
    ofAddListener(nodeselector->newGUIEvent,this,&ofApp::guiEvent);
    
    //CREATE GUI || ROTATION
    rotator = new ofxUISuperCanvas("SET ROTATIONS", OFX_UI_FONT_MEDIUM);
    rotator->setPosition(nodeselector->getRect()->width, 0);
    rotator->addSpacer();
    rotator->addSlider("x rotation", -180.0f, 180.0f, 0.0f);
    rotator->addSlider("y rotation", -180.0f, 180.0f, 0.0f);
    rotator->addSlider("z rotation", -180.0f, 180.0f, 0.0f);
    rotator->addButton("reset to bind pose", false);
    rotator->autoSizeToFitWidgets();
    ofAddListener(rotator->newGUIEvent,this,&ofApp::guiEvent);
}



//--------------------------------------------------------------
void ofApp::update(){
    model.update();
    regularmodel.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableLighting();
    cambonedmodel.begin();
    ofEnableDepthTest();
    
    light.enable();
    model.draw();
    light.disable();
    ofDrawAxis(4);
    ofDisableDepthTest();
    cambonedmodel.end();
    
    ofDisableLighting();
    
    camregularmodel.begin(regularcamfield.getBoundingBox());

    ofEnableDepthTest();
    regularmodel.draw(OF_MESH_FILL);
    ofDisableDepthTest();
    camregularmodel.end();
    
    regularcamfield.draw();
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
    if(rotator->isHit(x, y)) {
        cambonedmodel.disableMouseInput();
    }

    if(regularcamfield.inside(x, y)) {
        cambonedmodel.disableMouseInput();
    }

    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if(!rotator->isHit(x, y)) {
        cambonedmodel.enableMouseInput();
    }
    
    
    if(!regularcamfield.inside(x, y)) {
        cambonedmodel.enableMouseInput();
    }
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    regularcamfield.clear();
    regularcamfield.addVertex(ofPoint(ofGetWidth()-320,0));
    regularcamfield.addVertex(ofPoint(ofGetWidth(),0));
    regularcamfield.addVertex(ofPoint(ofGetWidth(),240));
    regularcamfield.addVertex(ofPoint(ofGetWidth()-320,240));
    regularcamfield.close();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
