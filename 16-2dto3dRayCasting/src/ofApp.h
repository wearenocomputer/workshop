#pragma once

#include "ofMain.h"
#include "ofxRay.h"


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
		
        ofEasyCam cam;

        //ofxRay stuff
        ofRay       ray;
        ofPlane  plane;
    
        void getMousePositionToNearPlane(float mousex, float mousey, GLdouble nearPlaneLocation[3]);
        void getMousePositionToFarPlane(float mousex, float mousey, GLdouble farPlaneLocation[3]);
    
};
