#pragma once

#include "ofMain.h"
#include "NCAssimpModel.h"
#include "ofxAssimpModelLoader.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        NCAssimpModel model;
        ofxAssimpModelLoader regularmodel;
    
        ofEasyCam cambonedmodel;
        ofEasyCam camregularmodel;
    
        ofxUISuperCanvas        * nodeselector;
        ofxUIDropDownList       * nodeselectordropdown;
        ofxUIDropDownList       * modelselectordropdown;
    
    
        ofxUISuperCanvas        * rotator;

        void guiEvent(ofxUIEventArgs &e);
    
        string currentselectednode;
    
        ofPolyline regularcamfield;
    
        void fillNodeSelectorDropdown();
    
        ofLight light;
    
           
};
