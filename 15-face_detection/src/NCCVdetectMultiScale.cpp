#include "NCCVdetectMultiScale.h"

NCCVdetectMultiScale::NCCVdetectMultiScale(){

}

NCCVdetectMultiScale::~NCCVdetectMultiScale() {
	ofUnregisterKeyEvents(this);
	ofRemoveListener(NCEventDispatcher::NCEVENTDISPATCHER.LOADALLSETTING, this, &NCCVdetectMultiScale::loadAllSettings);
	ofRemoveListener(NCEventDispatcher::NCEVENTDISPATCHER.SAVEALLSETTINGS, this, &NCCVdetectMultiScale::saveAllSettings);
	ofRemoveListener(facetrackerGUI->newGUIEvent,this,&NCCVdetectMultiScale::guiEvent);
	delete facetrackerGUI;
}


void NCCVdetectMultiScale::guiEvent(ofxUIEventArgs &e) {
	string name = e.widget->getName();
	int kind = e.widget->getKind();

	if (name == "trackers") {
		ofxUIRadio *radio = (ofxUIRadio *) e.widget;
	
		if (radio->getActiveName() == "profile") {
			useProfile();
		} else if (radio->getActiveName() == "frontal") {
			useFrontal();
		} else {
			useUpperBody();
		}
	}
}

void NCCVdetectMultiScale::loadAllSettings(NCGenericEventArg &e) {
	facetrackerGUI->loadSettings(NCAppData::NCAPPDATA.cameraid+"_multidetect.xml");
}

void NCCVdetectMultiScale::saveAllSettings(NCGenericEventArg &e) {
	facetrackerGUI->saveSettings(NCAppData::NCAPPDATA.cameraid+"_multidetect.xml");
}


void NCCVdetectMultiScale::keyPressed(ofKeyEventArgs &key) {

}
void NCCVdetectMultiScale::keyReleased(ofKeyEventArgs &key) {
	if (key.key == 's') {
		NCGenericEventArg e;
		saveAllSettings(e);
	}

	if (key.key == 'l') {
		NCGenericEventArg e;
		loadAllSettings(e);
	}

}

//--------------------------------------------------------------
void NCCVdetectMultiScale::setupGUI() {


	/************************************
	***** FACE TRACKING GUI         ****
	***********************************/
	facetrackerGUI = new ofxUISuperCanvas("TRACKER PARAMS", OFX_UI_FONT_SMALL);
	facetrackerGUI->setPosition(frame.x,  frame.y+frame.height+80);
	facetrackerGUI->addSpacer();
	facetrackerGUI->addToggle("toggle face detection",&bDoTracking);
	facetrackerGUI->addToggle("toggle histogram",&bDoHistogramEqualization);
	facetrackerGUI->addToggle("toggle resize image",&bDoResizeImage);
	facetrackerGUI->addSlider("image scale factor",0.1,1,&imagescalefactor);
	facetrackerGUI->addSpacer();
	facetrackerGUI->addSlider("FScaleFactor", 1.01, 2.0, &multiScaleFactor);
	facetrackerGUI->addSpacer();
	facetrackerGUI->addIntSlider("FMinNeighbours", 0, 6, &minNeighbors);
	facetrackerGUI->addSpacer();

	vector <string> radiobuttons;
	radiobuttons.push_back("frontal");
	radiobuttons.push_back("profile");
	radiobuttons.push_back("upperbody");
	facetrackerGUI->addRadio("trackers", radiobuttons);
	facetrackerGUI->autoSizeToFitWidgets();
	ofAddListener(facetrackerGUI->newGUIEvent,this,&NCCVdetectMultiScale::guiEvent);
}

void NCCVdetectMultiScale::setup() {
    frame =ofRectangle(0,NCAppData::NCAPPDATA.areawidthheight.y+NCAppData::NCAPPDATA.outputwidthheight.y,NCAppData::NCAPPDATA.areawidthheight.x,NCAppData::NCAPPDATA.areawidthheight.y);
	
	togglecounter = 0;
	bDoHistogramEqualization = false;
	bDoResizeImage = true;
	bDoTracking = false;
	imagescalefactor = 0.8f;
	minSizeScale = 0;
	maxSizeScale = 1;

	multiScaleFactor = 1.1;

	minNeighbors =3;

	smallfont.loadFont("GUI/gothic_0.ttf", 11);

	setupGUI();

	ofRegisterKeyEvents(this);
	ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.LOADALLSETTING, this, &NCCVdetectMultiScale::loadAllSettings);
	ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.SAVEALLSETTINGS, this, &NCCVdetectMultiScale::saveAllSettings);

	useProfile();

	NCGenericEventArg e;
	loadAllSettings(e);
}

void NCCVdetectMultiScale::update(cv::Mat img) {

	//IMAGE MUST BE ONE CHANNEL GRAYSCALE
	if (ofxCv::getChannels(img)!=1) return;

	feedthistoclassifier = ofxCv::toCv(img);

	if (bDoResizeImage) {
		ofxCv::resize(img, feedthistoclassifier, imagescalefactor, imagescalefactor);
	}

	if (bDoHistogramEqualization) {
		ofxCv::equalizeHist(feedthistoclassifier, feedthistoclassifier);
	}


	if (bDoTracking) {
		cv::Size minSize, maxSize;
		float averageSide = (feedthistoclassifier.rows + feedthistoclassifier.cols) / 2;
		if(minSizeScale > 0) {
			int side = minSizeScale * averageSide;
			minSize = cv::Size(side, side);
		}
		if(maxSizeScale < 1) {
			int side = maxSizeScale * averageSide;
			maxSize = cv::Size(side, side);
		}
		classifier.detectMultiScale(feedthistoclassifier, objects, multiScaleFactor, minNeighbors,( ofxCv::CASCADE_SCALE_IMAGE | ofxCv::CASCADE_DO_ROUGH_SEARCH),minSize,maxSize);
	
	}
}

void NCCVdetectMultiScale::debugDraw() {
	ofxCv::drawMat(feedthistoclassifier,frame.x,frame.y);
	if (bDoTracking) {
		//draw found rectangles
		ofSetColor(0,255,0);
		ofNoFill();
		for (int i=0;i<objects.size();i++) {
			ofRect(objects[i].x+frame.x,objects[i].y+frame.y, objects[i].width,objects[i].height);
		}
		ofFill();
		ofSetColor(255,255,255);
	}
}

//------------------------------------------------------------------------------------
void NCCVdetectMultiScale::resetClassifier(string _filename){
	classifier.empty();
	_filename = ofToDataPath(_filename);
	if(ofFile(_filename).exists()) {
		//classifier.empty();
		classifier.load(_filename);
	} else {
		ofLogError("ObjectFinder::setup") << "Couldn't find " << _filename;
	}
}

//------------------------------------------------------------------------------------
void NCCVdetectMultiScale::useFrontal() {
	bIsFrontal = true;
	bIsProfile = false;
	bIsUpperBody = false;
	resetClassifier("HAARCASCADES/FACE/haarcascade_frontalface_alt2.xml");
}

//------------------------------------------------------------------------------------
void NCCVdetectMultiScale::useProfile() {
	bIsFrontal = false;
	bIsProfile = true;
	bIsUpperBody = false;
	resetClassifier("HAARCASCADES/FACE/haarcascade_profileface.xml");
}

//------------------------------------------------------------------------------------
void NCCVdetectMultiScale::useUpperBody() {
	
	bIsUpperBody = true;
	bIsProfile = false;
	bIsUpperBody = false;
	resetClassifier("HAARCASCADES/haarcascade_mcs_upperbody.xml");
}


//-----------------------------------------------------------------------------------
void NCCVdetectMultiScale::toggleFrontalProfile() {
		ofxUIRadio *radio = (ofxUIRadio*)facetrackerGUI->getWidget("trackers");
		if (bIsFrontal) {
			useProfile();
			radio->activateToggle("profile");
		} else {
			useFrontal();
			radio->activateToggle("frontal");
		}
}


//-----------------------------------------------------------------------------------
void NCCVdetectMultiScale::toggleAllProfiles() {

	ofxUIRadio *radio = (ofxUIRadio*)facetrackerGUI->getWidget("trackers");
	
	togglecounter++;

	if (togglecounter == 1) {
		useFrontal();
		radio->activateToggle("frontal");
	} else if(togglecounter == 2) {
		useProfile();
		radio->activateToggle("profile");
	} else if (togglecounter==3) {
		useUpperBody();
		radio->activateToggle("upperbody");
		togglecounter = 0;
	}

	
}
