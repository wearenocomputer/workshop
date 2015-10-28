//
//  NCTCPClient.h
//  QMUSICCameraController
//
//  Created by Wim Vanhenden on 08/04/15.
//
//

#pragma once

#include "ofUtils.h"
class NCAppData {

public:

	static NCAppData NCAPPDATA;  

	~NCAppData();
	NCAppData();

	string cameraid;

	ofVec2f areawidthheight;
    ofVec2f outputwidthheight;

};