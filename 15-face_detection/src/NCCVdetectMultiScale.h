#pragma once
#include "ofMain.h"
#include "ofxCv/Utilities.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "NCEventDispatcher.h"
#include "NCAppData.h"

class NCCVdetectMultiScale {

public:


	NCCVdetectMultiScale();
	~NCCVdetectMultiScale();

	void setup();
	template <class T> 
	void update(T& img) {
		update(ofxCv::toCv(img));
	}
	
	void update(cv::Mat img);

	void debugDraw();

	bool bDoHistogramEqualization;
	bool bDoResizeImage;
	float imagescalefactor;
	bool bDoTracking;
	vector<cv::Rect> objects;

	void keyPressed(ofKeyEventArgs &key);
	void keyReleased(ofKeyEventArgs &key);

	void toggleFrontalProfile();
	void toggleAllProfiles();

private:

	void useProfile();
	void useFrontal();
	void useUpperBody();


	void resetClassifier(string _filename);

	void guiEvent(ofxUIEventArgs &e);

	ofRectangle frame;
	ofTrueTypeFont smallfont;

	ofxUISuperCanvas        * facetrackerGUI;

	void loadAllSettings(NCGenericEventArg &e);
	void saveAllSettings(NCGenericEventArg &e);

	void setupGUI();

	cv::CascadeClassifier classifier;
	cv::Mat feedthistoclassifier;
	float minSizeScale, maxSizeScale;
	float multiScaleFactor;
	int minNeighbors;

	bool bIsFrontal;
	bool bIsProfile;
	bool bIsUpperBody;

	int togglecounter;
};