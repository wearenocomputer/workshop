//
//  NCCameraElementsViewController.cpp
//  QMUSICCameraController
//
//  Created by Wim Vanhenden on 08/04/15.
//
//

#include "NCCameraIDSelector.h"

void NCCameraIDSelector::keyReleased(ofKeyEventArgs &key) {
	if (key.key == 's') {
		gui->saveSettings("idselector.xml");
	}

	if (key.key == 'l') {
		gui->loadSettings("idselector.xml");
	}

}

void NCCameraIDSelector::keyPressed(ofKeyEventArgs &key) {

}

NCCameraIDSelector::~NCCameraIDSelector(){
	ofUnregisterKeyEvents(this);
	ofAddListener(gui->newGUIEvent,this,&NCCameraIDSelector::guiEvent);
	delete gui;
}

NCCameraIDSelector::NCCameraIDSelector() {
	ofRegisterKeyEvents(this);
}

void NCCameraIDSelector::setup() {
	gui = new ofxUISuperCanvas("SET CAMERA ID", OFX_UI_FONT_MEDIUM);
	gui->addTextInput("camid","0");
	gui->setPosition(NCAppData::NCAPPDATA.areawidthheight.x,0);
	gui->autoSizeToFitWidgets();
	

	ofxUITextInput *textfieldpan = (ofxUITextInput*)  gui->getWidget("camid");
	textfieldpan->setAutoClear(false);

	gui->loadSettings("idselector.xml");
	NCAppData::NCAPPDATA.cameraid = textfieldpan->getTextString();
	
	
	ofAddListener(gui->newGUIEvent,this,&NCCameraIDSelector::guiEvent);
}


//-----------------------------------------------------------------
void NCCameraIDSelector::guiEvent(ofxUIEventArgs &e) {
	string name = e.widget->getName();
	int kind = e.widget->getKind();

	if (kind == OFX_UI_WIDGET_TEXTINPUT) {
		ofxUITextInput *textfieldpan = (ofxUITextInput*)  gui->getWidget("camid");

		string value = textfieldpan->getTextString();
		if (value == NCAppData::NCAPPDATA.cameraid) {
			NCEventDispatcher::NCEVENTDISPATCHER.dispatchSaveAllSettings();
		} else {
			NCAppData::NCAPPDATA.cameraid = textfieldpan->getTextString();
			NCEventDispatcher::NCEVENTDISPATCHER.dispatchLoadAllSettings();
		}
		gui->saveSettings("idselector.xml");
	}
}