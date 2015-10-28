//
//  NCTCPClient.cpp
//  QMUSICCameraController
//
//  Created by Wim Vanhenden on 08/04/15.
//
//

#include "NCAppData.h"

NCAppData NCAppData::NCAPPDATA;

NCAppData::~NCAppData(){
	
}

NCAppData::NCAppData() {
	NCAppData::NCAPPDATA = *this;
	areawidthheight = ofVec2f(640,480);
    outputwidthheight = ofVec2f(320,240);
}