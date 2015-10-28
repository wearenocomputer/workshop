//
//  NCPTZController.h
//  qMusicFaceTracker
//
//  Created by Wim Vanhenden on 09/02/15.
//
//



/*******************************       ALGORITHMIC APPROACH        *****************************************/


/************************************************************************************************************ 
stores incoming face data for x seconds.
When this time is passed we check what the most common id is in the data range
When this number exceeds a threshold of n compared to the total data instances then all is good
*************************************************************************************************************/

/************************************************************************************************************
We check for the min and max value of center of incoming data (rectangles).
If the absolute distance bewteen these two stays within a certain threshold then all is good
*************************************************************************************************************/

/************************************************************************************************************
Check if our average is not contained within our movement area. If so all is good
*************************************************************************************************************/


#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "NCEventDispatcher.h"
#include "NCAppData.h"
#include "ofxTimer.h"

struct NCPTZrectangle {
	int key;
	ofRectangle rect;
};

struct less_than_key
{
	inline bool operator() (const NCPTZrectangle& struct1, const NCPTZrectangle& struct2)
	{
		return (struct1.key < struct2.key);
	}
};


class NCMovementController {
public:

	~NCMovementController();
	NCMovementController();

	void setup(ofRectangle _rect);
	void update(NCPTZrectangle npzrect);
	void draw(ofRectangle incframe, float scaleuppercentage);

	void startRecording();
	void stopRecording();

	void positionMovementRectangle(ofPoint &p);

	ofRectangle getCurrentFaceRectangle();

	ofPoint		getCurrentFacePoint();

	ofEvent< vector<float> > TRACKED;
	ofEvent<int> NOT_TRACKED;

	void keyPressed(ofKeyEventArgs &key);
	void keyReleased(ofKeyEventArgs &key);

	bool bIsRecording;

private:

	void guiEvent(ofxUIEventArgs &e);
	void loadAllSettings(NCGenericEventArg &e);
	void saveAllSettings(NCGenericEventArg &e);

	void clearData();

	float secondstorecord;
	float percentagetopass;
	float movementfactor;

	ofxUISuperCanvas        * ptztrackerGUI;

	float lastTime;

	ofRectangle frame;

	ofRectangle currentmovementrectangle;    
	ofRectangle referencerectangle;

	vector<NCPTZrectangle> passedrectanglesfirststage;
	vector<NCPTZrectangle> trackedinstances;
	vector<NCPTZrectangle> returnMostOccurances(vector<NCPTZrectangle> &rectangles);

	ofRectangle calculateMovementRectangle();



	ofxTimer nofacepoller;
	void startPollingForNoFace();
	void stopPollingForNoFace();
	void facePollerCompleteHandler(ofEventArgs &args);

};