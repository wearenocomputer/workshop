//
//  NCCameraElementsViewController.h
//  QMUSICCameraController
//
//  Created by Wim Vanhenden on 08/04/15.
//
//

#pragma once
#include "ofMain.h"
#include "NCGenericEventArg.h"

class NCEventDispatcher {

public:

	static NCEventDispatcher NCEVENTDISPATCHER;  

	~NCEventDispatcher();
	NCEventDispatcher();

	ofEvent <NCGenericEventArg> FACETRACKED;
	void dispatchFaceDetected(NCAbstractEventBaseObject *object);

	ofEvent <NCGenericEventArg> STARTRACKING;
	void dispatchStartTracking();

	ofEvent <NCGenericEventArg> STOPTRACKING;
	void dispatchStopTracking();

	ofEvent <NCGenericEventArg> GETLATESTFACECOORDIANTES;
	void dispatchGetLatestFaceCoordinates();

	ofEvent <NCGenericEventArg> LOADALLSETTING;
	void dispatchLoadAllSettings();

	ofEvent <NCGenericEventArg> SAVEALLSETTINGS;
	void dispatchSaveAllSettings();

};