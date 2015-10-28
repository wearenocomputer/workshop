#pragma once

#include "ofMain.h"
#include "NCCVdetectMultiScale.h"
#include "NCBlobTracker.h"
#include "NCMovementController.h"
#include "NCEventDispatcher.h"
#include "ofxTimer.h"

class NCFaceDetectorController {
public:
	NCFaceDetectorController();
	~NCFaceDetectorController();

	void setup();
	void update(ofImage &_incimage);
	void draw();


	ofRectangle detectionframe;

	void toggleTracking(bool dotracking);

	NCMovementController movementcontroller;

	void toggleFrontalProfile();



private:
	void detectionRecieved(vector<float> & f);

	ofImage imagetoprocess;
	ofRectangle frame;
	ofTrueTypeFont smallfont;

	NCCVdetectMultiScale multiscaledetector;
	
	NCBlobTracker blobtracker;
	void updatemovementController();

	ofxTimer togglePoller;
	void startAutoToggle();
	void stopAutoToggle();
	void autoToggleCallback(ofEventArgs &args);
	bool bDoToggler;
};

