#include "NCBlobTracker.h"

void NCBlobTracker::loadAllSettings(NCGenericEventArg &e) {
	blobtrackerGUI->loadSettings(NCAppData::NCAPPDATA.cameraid+"_blobtracker.xml");
}

void NCBlobTracker::saveAllSettings(NCGenericEventArg &e) {
	blobtrackerGUI->saveSettings(NCAppData::NCAPPDATA.cameraid+"_blobtracker.xml");
}

NCBlobTracker::NCBlobTracker() {

};

NCBlobTracker::~NCBlobTracker() {
	ofRemoveListener(blobtrackerGUI->newGUIEvent,this,&NCBlobTracker::guiEvent);
	ofRemoveListener(NCEventDispatcher::NCEVENTDISPATCHER.LOADALLSETTING, this, &NCBlobTracker::loadAllSettings);
	ofRemoveListener(NCEventDispatcher::NCEVENTDISPATCHER.SAVEALLSETTINGS, this, &NCBlobTracker::saveAllSettings);
	ofUnregisterKeyEvents(this);

	delete blobtrackerGUI;
};


void NCBlobTracker::keyPressed(ofKeyEventArgs &key) {

}
void NCBlobTracker::keyReleased(ofKeyEventArgs &key) {
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
void NCBlobTracker::guiEvent(ofxUIEventArgs &e) {

	string name = e.widget->getName();
	int kind = e.widget->getKind();

	if (kind == OFX_UI_WIDGET_SLIDER_H) {

		if (name == "Distance") {
			recttracker.setMaximumDistance(blobtrackerdistance);
		}
	}

	if (kind == OFX_UI_WIDGET_INTSLIDER_H) {

		if (name == "Persistance") {
			recttracker.setPersistence(blobtrackerpersistance);
		}

	}
}

void NCBlobTracker::setup() {

	frame =ofRectangle(20,20,NCAppData::NCAPPDATA.areawidthheight.x,NCAppData::NCAPPDATA.areawidthheight.y);

	blobtrackerdistance = 32;
	blobtrackerpersistance = 30;

	/************************************
	***** BLOB TRACKING GUI         ****
	***********************************/
	blobtrackerGUI = new ofxUISuperCanvas("BLOB TRACKER PARAMS", OFX_UI_FONT_SMALL);
	blobtrackerGUI->setPosition(frame.x, frame.y+frame.height+250);
	blobtrackerGUI->addSpacer();
	blobtrackerGUI->addSlider("Distance", 1, 200, &blobtrackerdistance);
	blobtrackerGUI->addSpacer();
	blobtrackerGUI->addIntSlider("Persistance", 1, 240, &blobtrackerpersistance);
	blobtrackerGUI->autoSizeToFitWidgets();

	ofAddListener(blobtrackerGUI->newGUIEvent,this,&NCBlobTracker::guiEvent);
	ofRegisterKeyEvents(this);
	ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.LOADALLSETTING, this, &NCBlobTracker::loadAllSettings);
	ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.SAVEALLSETTINGS, this, &NCBlobTracker::saveAllSettings);

    NCGenericEventArg e;
	loadAllSettings(e);

}

void NCBlobTracker::update(vector<cv::Rect> &rects) {
	recttracker.track(rects);
}