//
//  NCPTZController.cpp
//  qMusicFaceTracker
//
//  Created by Wim Vanhenden on 09/02/15.
//
//

#include "NCMovementController.h"


/********************************
POLLER TIMER
*********************************/
void NCMovementController::startPollingForNoFace(){
	nofacepoller.setup(750,false);
	nofacepoller.startTimer();
	ofAddListener(nofacepoller.TIMER_REACHED , this, &NCMovementController::facePollerCompleteHandler) ;
}

void NCMovementController::stopPollingForNoFace(){
	nofacepoller.stopTimer();
	ofRemoveListener(nofacepoller.TIMER_REACHED , this, &NCMovementController::facePollerCompleteHandler) ;
}

void NCMovementController::facePollerCompleteHandler( ofEventArgs &args ) {
	ofLogVerbose()<<"NO FACE DETECTED CLEAR DATA!"<<endl;
	stopPollingForNoFace();
	clearData();
}

NCMovementController::~NCMovementController() {
	ofUnregisterKeyEvents(this);
	ofRemoveListener(ptztrackerGUI->newGUIEvent,this,&NCMovementController::guiEvent);
	ofRemoveListener(NCEventDispatcher::NCEVENTDISPATCHER.LOADALLSETTING, this, &NCMovementController::loadAllSettings);
	ofRemoveListener(NCEventDispatcher::NCEVENTDISPATCHER.SAVEALLSETTINGS, this, &NCMovementController::saveAllSettings);
	delete ptztrackerGUI;
}

NCMovementController::NCMovementController() {

}

void NCMovementController::loadAllSettings(NCGenericEventArg &e) {
	ptztrackerGUI->loadSettings(NCAppData::NCAPPDATA.cameraid+"_movement.xml");
}

void NCMovementController::saveAllSettings(NCGenericEventArg &e) {
	ptztrackerGUI->saveSettings(NCAppData::NCAPPDATA.cameraid+"_movement.xml");
}

void NCMovementController::keyPressed(ofKeyEventArgs &key) {

}

void NCMovementController::keyReleased(ofKeyEventArgs &key) {
	if (key.key == 's') {
		NCGenericEventArg e;
		saveAllSettings(e);
	}

	if (key.key == 'l') {
		NCGenericEventArg e;
		loadAllSettings(e);
	}

}

void NCMovementController::positionMovementRectangle(ofPoint &p) {
	currentmovementrectangle.x = p.x-(currentmovementrectangle.width*0.5);
	currentmovementrectangle.y = p.y-(currentmovementrectangle.height*0.5);
}

vector<NCPTZrectangle> NCMovementController::returnMostOccurances(vector<NCPTZrectangle> &rectangles) {

	//SORT THE RECTANGLES BASED ON KEY
	std::sort(rectangles.begin(), rectangles.end(), less_than_key());
	//CREATE A MAP CONTAING THE OCCURCANES PER KEY
	map<int, int> occurances;
	for(int i = 0; i < rectangles.size(); i++) {
		int count = 1;
		int limit = rectangles.size() - 1;
		while(i < limit  && rectangles[i].key == rectangles[i+1].key) {
			count++;
			i++;
		}
		occurances.insert(std::pair<int, int>(count,rectangles[i].key));
	}

	//GET THE KEY WITH THE MOST OCCURANCES (the map is sorted from lowest to highest) but you have to take rbegin iterator
	int keywiththemostocurances = occurances.rbegin()->second;
	vector<NCPTZrectangle> mostfound;

	float perc = (float)occurances.rbegin()->first/rectangles.size();

	//if the key with the most occurances is -1 return nothing
	//if the percentage of found keys is less than n of total don't even bother
	if (keywiththemostocurances == -1) {
		return mostfound;
	} else if (perc<percentagetopass) {
		return mostfound;
	}

	//FILL VECTOR WITH RECTANGLES OCCURED THE MOST
	for(int i = 0; i < rectangles.size(); i++) {
		if (rectangles[i].key == keywiththemostocurances) {
			mostfound.push_back(rectangles[i]);
		}
	}
	return mostfound;
}

void NCMovementController::setup(ofRectangle _rect) {

	frame = _rect;
	secondstorecord = 2;
	percentagetopass = 0.8;
	movementfactor = 65.0f;

	bIsRecording = false;

	/************************************
	***** PTZ TRACKING GUI         ****
	***********************************/
	ptztrackerGUI = new ofxUISuperCanvas("MOVEMENT PARAMS", OFX_UI_FONT_SMALL);
	ptztrackerGUI->setPosition(frame.x, frame.y+frame.height+350);
	ptztrackerGUI->addSpacer();
	ptztrackerGUI->addSlider("seconds to record", 0.2, 5, &secondstorecord);
	ptztrackerGUI->addSpacer();
	ptztrackerGUI->addSlider("percentage to pass", 0, 1, &percentagetopass);
	ptztrackerGUI->addSpacer();
	ptztrackerGUI->addSlider("movement area", 5, 150, &movementfactor);
	ptztrackerGUI->addSpacer();
	ptztrackerGUI->addButton("start tracking", false);
	ptztrackerGUI->addButton("stop tracking", false);
	ptztrackerGUI->autoSizeToFitWidgets();

	ofAddListener(ptztrackerGUI->newGUIEvent,this,&NCMovementController::guiEvent);
	ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.LOADALLSETTING, this, &NCMovementController::loadAllSettings);
	ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.SAVEALLSETTINGS, this, &NCMovementController::saveAllSettings);
	ofRegisterKeyEvents(this);

	NCGenericEventArg e;
	loadAllSettings(e);

	clearData();

}

void NCMovementController::update(NCPTZrectangle npzrect) {

	if (bIsRecording) {
		trackedinstances.push_back(npzrect);
		float passedtime = ofGetElapsedTimef()-lastTime;

		if (passedtime>secondstorecord) {

			if (returnMostOccurances(trackedinstances).size()>0) {
				passedrectanglesfirststage = returnMostOccurances(trackedinstances);

				referencerectangle = passedrectanglesfirststage[passedrectanglesfirststage.size()-1].rect;

				//compare if the center of the reference rectangle is inside the currentmovemement rectangle
				//we do not compare if the complete reference rectangle is inside the current movement rectangle
				//this gives better, less jumpy results
				if (!currentmovementrectangle.inside(referencerectangle.getCenter())) {
					//WE HAVE DETECTION
					currentmovementrectangle = calculateMovementRectangle();
					vector<float> xy;
					xy.push_back(referencerectangle.x+(referencerectangle.width/2));
					xy.push_back(referencerectangle.y+(referencerectangle.height/2));
					//ofLogVerbose()<<"FACE DETECTED OUTSIDE BOUNDS"<<endl;
					ofNotifyEvent(TRACKED,xy,this);
					stopPollingForNoFace();
					startPollingForNoFace();
				} else {
					stopPollingForNoFace();
					startPollingForNoFace();
					//ofLogVerbose()<<"FACE DETECTED WITHIN BOUNDS"<<endl;
				}
			} else {
				//ofLogVerbose()<<"NO FACE DETECTED"<<endl;
				//clearData();
				int val=0;
				ofNotifyEvent(NOT_TRACKED,val,this);
				
			}
			trackedinstances.clear();
			lastTime = ofGetElapsedTimef();
		}
	}
}

void NCMovementController::draw(ofRectangle incframe, float scaleuppercentage) {

	/**************
	DEBUG DRAW
	**************/

	ofNoFill();
	//draw all rectangles from result
	ofSetColor(0, 255, 255);
	for(int i=0;i<passedrectanglesfirststage.size();i++) {
		ofRect(((passedrectanglesfirststage[i].rect.x*scaleuppercentage)+incframe.x), ((passedrectanglesfirststage[i].rect.y*scaleuppercentage)+incframe.y), passedrectanglesfirststage[i].rect.width*scaleuppercentage, passedrectanglesfirststage[i].rect.height*scaleuppercentage);
	}


	if (passedrectanglesfirststage.size()>0) {

		NCPTZrectangle temprect = passedrectanglesfirststage[passedrectanglesfirststage.size()-1];
		ofSetColor(255, 0, 0);
		ofRect(((temprect.rect.x*scaleuppercentage)+incframe.x), ((temprect.rect.y*scaleuppercentage)+incframe.y), temprect.rect.width*scaleuppercentage, temprect.rect.height*scaleuppercentage);
	}


	ofSetColor(0, 0, 255);
	ofRect(((currentmovementrectangle.x*scaleuppercentage)+incframe.x), ((currentmovementrectangle.y*scaleuppercentage)+incframe.y), currentmovementrectangle.width*scaleuppercentage, currentmovementrectangle.height*scaleuppercentage);

	ofSetColor(255, 255, 255);
	ofFill();

	if (bIsRecording) {
		ofSetColor(255,0,0);
		ofCircle(frame.x, frame.y,8);
		ofSetColor(255,255,255);
	}
}


void NCMovementController::startRecording() {
	if (!bIsRecording) {
		bIsRecording = true;
		clearData();
		lastTime = ofGetElapsedTimef();
	}
}

void NCMovementController::stopRecording() {
	if (bIsRecording) {
		bIsRecording = false;
		clearData();
	}
}

ofRectangle NCMovementController::calculateMovementRectangle() {

	ofRectangle mov;
	mov.x = (referencerectangle.x+referencerectangle.width*0.5) - (movementfactor*0.5);
	mov.y = (referencerectangle.y+referencerectangle.height*0.5)- (movementfactor*0.5);
	mov.width = movementfactor;
	mov.height = movementfactor;

	return mov;

}


//--------------------------------------------------------------
void NCMovementController::guiEvent(ofxUIEventArgs &e) {

	string name = e.widget->getName();
	int kind = e.widget->getKind();

	if (name == "start tracking") {
		ofxUIButton *button = (ofxUIButton *) e.widget;
		if(button->getValue()) {
			startRecording();
		}
	}

	if (name == "stop tracking") {
		ofxUIButton *button = (ofxUIButton *) e.widget;
		if(button->getValue()) {
			stopRecording();
		}
	}

	if (name == "movement area") {
		currentmovementrectangle.x = (currentmovementrectangle.x+currentmovementrectangle.width*0.5) - (movementfactor*0.5);
		currentmovementrectangle.y = (currentmovementrectangle.y+currentmovementrectangle.height*0.5)- (movementfactor*0.5);
		currentmovementrectangle.width = movementfactor;
		currentmovementrectangle.height = movementfactor;
	}

}

void NCMovementController::clearData() {
	currentmovementrectangle = ofRectangle(0,0,0,0);
	trackedinstances.clear();
	passedrectanglesfirststage.clear();
	referencerectangle = ofRectangle(0,0,0,0);
}

//-------------------------------------------------------------
ofRectangle NCMovementController::getCurrentFaceRectangle() {
	return currentmovementrectangle;
}

//-------------------------------------------------------------
ofPoint NCMovementController::getCurrentFacePoint() {
	currentmovementrectangle = calculateMovementRectangle();
	vector<float> xy;
	xy.push_back(referencerectangle.x+(referencerectangle.width/2));
	xy.push_back(referencerectangle.y+(referencerectangle.height/2));
	return ofPoint(xy[0],xy[1]);
}