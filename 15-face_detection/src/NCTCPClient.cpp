//
//  NCTCPClient.cpp
//  QMUSICCameraController
//
//  Created by Wim Vanhenden on 08/04/15.
//
//

#include "NCTCPClient.h"

NCTCPClient::~NCTCPClient(){
	stopClient();
	ofRemoveListener(gui->newGUIEvent,this,&NCTCPClient::guiEvent);
	ofRemoveListener(NCEventDispatcher::NCEVENTDISPATCHER.LOADALLSETTING, this, &NCTCPClient::loadAllSettings);
	ofRemoveListener(NCEventDispatcher::NCEVENTDISPATCHER.SAVEALLSETTINGS, this, &NCTCPClient::saveAllSettings);
	ofUnregisterKeyEvents(this);

	delete gui;
}

NCTCPClient::NCTCPClient() {

}


void NCTCPClient::startClient() {
	if (!bWeConnected) {
		bWeConnected = tcpclient.setup(host, port, false);
		tcpclient.setMessageDelimiter("\n");
		if (bWeConnected) {
			//cout<<"start client with id: "<<currentid<<endl;
			sendMessage("CLIENTSAYSHI/"+currentid);
		}
	}
}

void NCTCPClient::stopClient() {
	if (bWeConnected) {
		//cout<<"stop client with id: "<<currentid<<endl;
		sendMessage("CLIENTSAYSGOODBYE/"+currentid);
		tcpclient.close();
		bWeConnected = false;
	}
	currentid = NCAppData::NCAPPDATA.cameraid;
}

void NCTCPClient::keyPressed(ofKeyEventArgs &key) {

}
void NCTCPClient::keyReleased(ofKeyEventArgs &key) {
	if (key.key == 's') {
		NCGenericEventArg e;
		saveAllSettings(e);
	}

	if (key.key == 'l') {
		NCGenericEventArg e;
		loadAllSettings(e);
	}

}

void NCTCPClient::loadAllSettings(NCGenericEventArg &e) {
	stopClient();
	gui->loadSettings(NCAppData::NCAPPDATA.cameraid+"_tcpclient.xml");
}

void NCTCPClient::saveAllSettings(NCGenericEventArg &e) {
	gui->saveSettings(NCAppData::NCAPPDATA.cameraid+"_tcpclient.xml");
}

void NCTCPClient::guiEvent(ofxUIEventArgs &e) {
	string name = e.widget->getName();

	int kind = e.widget->getKind();

	if (kind == OFX_UI_WIDGET_TEXTINPUT) {

		if (name=="port") {
			ofxUITextInput *textfieldport = (ofxUITextInput*)  gui->getWidget("port");
			port = ofToInt(textfieldport->getTextString());
		}

		if (name=="host") {
			ofxUITextInput *textfieldhost = (ofxUITextInput*)  gui->getWidget("host");
			host = textfieldhost->getTextString();
		}
	}

	if (name=="videostream"){

		ofxUIRadio *radio = (ofxUIRadio *) e.widget;
		if (radio->getActiveName()=="connect") {
			startClient();
		} else {
			stopClient();
		}
	}	
}

void NCTCPClient::setup(int _port, string _host) {

	port = _port;
	host= _host;

	gui = new ofxUISuperCanvas("TCP CLIENT", OFX_UI_FONT_SMALL);
	gui->addSpacer();
	vector<string> choices;
	choices.push_back("connect");
	choices.push_back("disconnect");

	gui->addRadio("videostream",choices);

	gui->addTextInput("host",host);
	gui->addTextInput("port",ofToString(port));
	gui->autoSizeToFitWidgets();

	ofAddListener(gui->newGUIEvent,this,&NCTCPClient::guiEvent);
	ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.LOADALLSETTING, this, &NCTCPClient::loadAllSettings);
	ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.SAVEALLSETTINGS, this, &NCTCPClient::saveAllSettings);
	ofRegisterKeyEvents(this);

	currentid = NCAppData::NCAPPDATA.cameraid;

	NCGenericEventArg e;
	loadAllSettings(e);

	stopClient();
}

void NCTCPClient::setGUIStuff() {

	if (bWeConnected) {
		ofxUIRadio *radio = (ofxUIRadio *)	gui->getWidget("videostream");
		radio->activateToggle("connect");
	} else {
		ofxUIRadio *radio = (ofxUIRadio *)	gui->getWidget("videostream");
		radio->activateToggle("disconnect");
	}
}


void NCTCPClient::update() {
	setGUIStuff();
	if (bWeConnected) {
		string str = tcpclient.receive();
		if( str.length() > 0 ){
			parseMessage(str);
		}
	}
}

void NCTCPClient::sendMessage(string _msg) {
	string message = _msg+"\n";
	ofLogVerbose()<<"TCP sending message: "<<_msg<<endl;
	if (tcpclient.send(message)) {
		ofLogVerbose()<<"TCP client message delivered"<<endl;
	} else {
		ofLogVerbose()<<"TCP client message NOT delivered"<<endl;
	}

}

//-----------------------------------------------------------------
void NCTCPClient::parseMessage(string _message){
	ofLogVerbose()<<"MESSAGE INCOMING FROM TCPSERVER: "<<_message<<endl;
	//FACETRACKED
	string PREFIXSTARTTRACKING = "STARTTRACKING";
	string PREFIXSTOPTRACKING = "STOPTRACKING";
	string PREFIXGETLATESTFACECOORD = "GETLATESTFACECOORDINATES";
	string PREFIXSERVERDOWN = "SERVERWENTAWAY";

	if (ofStringTimesInString(_message, PREFIXSTARTTRACKING)==1) {	
		vector<string>parts = ofSplitString(_message,"/",true);
		int camid = ofToInt(parts[1]);

		if (camid == ofToInt(NCAppData::NCAPPDATA.cameraid)) {
			NCEventDispatcher::NCEVENTDISPATCHER.dispatchStartTracking();
		}
	} else if (ofStringTimesInString(_message, PREFIXSTOPTRACKING)==1) {
		vector<string>parts = ofSplitString(_message,"/",true);
		int camid = ofToInt(parts[1]);
		if (camid == ofToInt(NCAppData::NCAPPDATA.cameraid)) {
			NCEventDispatcher::NCEVENTDISPATCHER.dispatchStopTracking();
		}
	} else if (ofStringTimesInString(_message, PREFIXGETLATESTFACECOORD)==1) {
	
		vector<string>parts = ofSplitString(_message,"/",true);
		int camid = ofToInt(parts[1]);
		if (camid == ofToInt(NCAppData::NCAPPDATA.cameraid)) {
			NCEventDispatcher::NCEVENTDISPATCHER.dispatchGetLatestFaceCoordinates();
		}
	} else if (ofStringTimesInString(_message, PREFIXSERVERDOWN)==1) {
		stopClient();
	}
}
