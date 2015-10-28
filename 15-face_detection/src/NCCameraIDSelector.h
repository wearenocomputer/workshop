//
//  NCCameraElementsViewController.h
//  QMUSICCameraController
//
//  Created by Wim Vanhenden on 08/04/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxUI.h"
#include "NCAppData.h"
#include "NCEventDispatcher.h"

class NCCameraIDSelector {

public:

	void keyPressed(ofKeyEventArgs &key);
	void keyReleased(ofKeyEventArgs &key);

	~NCCameraIDSelector();
	NCCameraIDSelector();

	ofxUISuperCanvas        * gui;

	void setup();

	void guiEvent(ofxUIEventArgs &e);

};