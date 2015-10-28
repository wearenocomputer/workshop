//
//  NCCameraElementsViewController.cpp
//  QMUSICCameraController
//
//  Created by Wim Vanhenden on 08/04/15.
//
//

#include "NCEventDispatcher.h"

NCEventDispatcher NCEventDispatcher::NCEVENTDISPATCHER;

NCEventDispatcher::~NCEventDispatcher(){
	
}

NCEventDispatcher::NCEventDispatcher() {
	NCEventDispatcher::NCEVENTDISPATCHER = *this;
}

void NCEventDispatcher::dispatchFaceDetected(NCAbstractEventBaseObject *object) {
	NCGenericEventArg e;  
	e.pointertosender = object;
	ofNotifyEvent(FACETRACKED, e, this); 
}


void NCEventDispatcher::dispatchStartTracking() {
	NCGenericEventArg e;
	ofNotifyEvent(STARTRACKING, e, this); 
}

void NCEventDispatcher::dispatchStopTracking() {
	NCGenericEventArg e;
	ofNotifyEvent(STOPTRACKING, e, this); 
}

void NCEventDispatcher::dispatchGetLatestFaceCoordinates() {
	NCGenericEventArg e;
	ofNotifyEvent(GETLATESTFACECOORDIANTES,e,this);
}


void NCEventDispatcher::dispatchLoadAllSettings() {
	NCGenericEventArg e;
	ofNotifyEvent(LOADALLSETTING, e, this); 
}

void NCEventDispatcher::dispatchSaveAllSettings() {
	NCGenericEventArg e;
	ofNotifyEvent(SAVEALLSETTINGS, e, this); 
}
