//
//  NCButtonNoToggle.h
//  QMUSICCameraController
//
//  Created by Wim Vanhenden on 08/04/15.
//
//

#pragma once
#include "ofMain.h"
#include "NCAbstractEventBaseObject.h"

class NCAbstractEventBaseObject;

class NCGenericEventArg: public ofEventArgs {

public:
	NCAbstractEventBaseObject *pointertosender;
	string message;
};