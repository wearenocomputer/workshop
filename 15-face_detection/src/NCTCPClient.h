//
//  NCTCPClient.h
//  QMUSICCameraController
//
//  Created by Wim Vanhenden on 08/04/15.
//
//

#pragma once
#include "ofxNetwork.h"
#include "ofUtils.h"
#include "NCAppData.h"
#include "NCEventDispatcher.h"
#include "ofxUI.h"

class NCTCPClient {

public:

	~NCTCPClient();
	NCTCPClient();

	void setup(int _port, string _host);
	void update();

	void sendMessage(string _msg);

	void keyPressed(ofKeyEventArgs &key);
	void keyReleased(ofKeyEventArgs &key);

private:
	int connectTime;
	int deltaTime;
	bool bWeConnected;
	ofxTCPClient tcpclient;

	ofxUISuperCanvas        *gui;

	int port;
	string host;

	string currentid;

	void parseMessage(string _message);
	void setGUIStuff();
	void guiEvent(ofxUIEventArgs &e);

	void loadAllSettings(NCGenericEventArg &e);
	void saveAllSettings(NCGenericEventArg &e);

	void startClient();
	void stopClient();

};