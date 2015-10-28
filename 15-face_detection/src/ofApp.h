#pragma once

#include "ofMain.h"
#include "ofxMemoryUsage.h"
#include "NCFaceDetectorController.h"
#include "NCEventDispatcher.h"
#include "NCTCPClient.h"
#include "NCCameraIDSelector.h"
#include "NCAppData.h"
#include "NCVideoPlayer.h"

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
    

    NCVideoPlayer camerafeed;
    
	NCFaceDetectorController facetracker;

	NCTCPClient tcpclient;

	void facetracked(NCGenericEventArg &arg);
	void startTracking(NCGenericEventArg &arg);
	void stopTracking(NCGenericEventArg &arg);
	void getLatestFaceCoord(NCGenericEventArg &arg);

	NCCameraIDSelector idselector;

	ofxMemoryUsage memusage;

};
