#include "NCFaceDetectorController.h"



/********************************
TOGGLE POLLER TIMER
*********************************/
void NCFaceDetectorController::startAutoToggle(){
	togglePoller.setup(30,true);
	togglePoller.startTimer();
	ofAddListener(togglePoller.TIMER_REACHED , this, &NCFaceDetectorController::autoToggleCallback) ;
}


void NCFaceDetectorController::stopAutoToggle(){
	togglePoller.stopTimer();
	ofRemoveListener(togglePoller.TIMER_REACHED , this, &NCFaceDetectorController::autoToggleCallback) ;
}

void NCFaceDetectorController::autoToggleCallback( ofEventArgs &args ) {
	toggleFrontalProfile();
}

NCFaceDetectorController::NCFaceDetectorController() {
}


NCFaceDetectorController::~NCFaceDetectorController() {
	ofRemoveListener(movementcontroller.TRACKED,this,&NCFaceDetectorController::detectionRecieved);
}

void NCFaceDetectorController::detectionRecieved(vector<float> & f) {

	if (bDoToggler) {
		stopAutoToggle();
		bDoToggler = false;
	}

	NCAbstractEventBaseObject * obj = new NCAbstractEventBaseObject();

	//map the detection point between 0 and 1
	float mappedx = ofMap(f[0],0,detectionframe.width,0,1);
	float mappedy = ofMap(f[1],0,detectionframe.height,0,1);

	obj->pointofdetection = ofPoint(mappedx,mappedy);
	NCEventDispatcher::NCEVENTDISPATCHER.dispatchFaceDetected(obj);

	delete obj;

	//movementcontroller.positionMovementRectangle(ofPoint(detectionframe.width/2,detectionframe.height/2));
}


void NCFaceDetectorController::setup() {

	bDoToggler = false;

	blobtracker.setup();
	multiscaledetector.setup();

	movementcontroller.setup(ofRectangle(20,20,NCAppData::NCAPPDATA.areawidthheight.x,NCAppData::NCAPPDATA.areawidthheight.y));
	ofAddListener(movementcontroller.TRACKED,this,&NCFaceDetectorController::detectionRecieved);
	
	frame =ofRectangle(0,0,NCAppData::NCAPPDATA.areawidthheight.x,NCAppData::NCAPPDATA.areawidthheight.y);

	smallfont.loadFont("GUI/gothic_0.ttf", 11);
}

void NCFaceDetectorController::update(ofImage &_incimage) {
	imagetoprocess = _incimage;
	multiscaledetector.update(imagetoprocess);
	blobtracker.update(multiscaledetector.objects);
	updatemovementController();
		//set detectionframe
		if (multiscaledetector.objects.size()>0) {
			detectionframe = ofRectangle(0,0,imagetoprocess.getWidth()*multiscaledetector.imagescalefactor,imagetoprocess.getHeight()*multiscaledetector.imagescalefactor);
		}
}

void NCFaceDetectorController::draw() {

	//DRAW INCOMING FRAME
	imagetoprocess.draw(frame.x, frame.y+frame.height);

	//DRAW MULITSCALE DETECTOR
	multiscaledetector.debugDraw();

	//draw found rectangles
	ofSetColor(0,255,0);
	ofNoFill();
	float rescaleperc = 1.0f/multiscaledetector.imagescalefactor;
	for (int i=0;i<multiscaledetector.objects.size();i++) {
		float xpos = (multiscaledetector.objects[i].x*rescaleperc)+frame.x;
		float ypos = (multiscaledetector.objects[i].y*rescaleperc)+frame.y+frame.height;
		ofRect(xpos,ypos, multiscaledetector.objects[i].width*rescaleperc,multiscaledetector.objects[i].height*rescaleperc);
		ofDrawBitmapStringHighlight(ofToString(blobtracker.recttracker.getCurrentLabels()[i]),xpos,ypos);
	}
	ofFill();
	ofSetColor(255,255,255);

	movementcontroller.draw(ofRectangle(frame.x, frame.y+frame.height,imagetoprocess.getWidth(),imagetoprocess.getHeight()),rescaleperc);
}

void NCFaceDetectorController::updatemovementController() {

	if (multiscaledetector.objects.size()>0) {

		if(bDoToggler) {
			bDoToggler = false;
			stopAutoToggle();
		}

		for (int i=0;i<multiscaledetector.objects.size();i++) {
			int currentid =blobtracker.recttracker.getCurrentLabels()[i];
			NCPTZrectangle rect;
			rect.key = currentid;
			rect.rect = ofxCv::toOf(multiscaledetector.objects[i]);
			movementcontroller.update(rect);
		}
	} else {
		if (!bDoToggler){
			bDoToggler = true;
			startAutoToggle();
		}
		NCPTZrectangle rect;
		rect.key = -1;
		movementcontroller.update(rect);
	}
}

//----------------------------------------------------
void NCFaceDetectorController::toggleTracking(bool dotracking) {

	if (dotracking) {
		movementcontroller.startRecording();
	} else {
		movementcontroller.stopRecording();
	}
}

//-----------------------------------------------------
void NCFaceDetectorController::toggleFrontalProfile() {
	multiscaledetector.toggleFrontalProfile();
}