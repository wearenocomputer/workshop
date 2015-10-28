#pragma once
#include "ofBaseTypes.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "NCEventDispatcher.h"
#include "NCAppData.h"

class NCBlobTracker {

public:

	NCBlobTracker();
	~NCBlobTracker();

	void setup();
	void update(vector<cv::Rect> &rects);

	ofxCv::RectTracker recttracker;

	void keyPressed(ofKeyEventArgs &key);
	void keyReleased(ofKeyEventArgs &key);

private:

	ofRectangle frame;
	
	ofxUISuperCanvas        * blobtrackerGUI;
	float blobtrackerdistance;
	int blobtrackerpersistance;
	void guiEvent(ofxUIEventArgs &e);
	void loadAllSettings(NCGenericEventArg &e);
	void saveAllSettings(NCGenericEventArg &e);
};