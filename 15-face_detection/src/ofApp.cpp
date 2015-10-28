#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::facetracked(NCGenericEventArg &arg){
	tcpclient.sendMessage("FACETRACKED/"+NCAppData::NCAPPDATA.cameraid+"/"+ofToString(arg.pointertosender->pointofdetection.x)+"/"+ofToString(arg.pointertosender->pointofdetection.y));
	cout<<"WIM SAYS SEND FACE TRACKED COORD"<<arg.pointertosender->pointofdetection.x<<" : " <<arg.pointertosender->pointofdetection.y<< " : "<<ofToString(ofGetFrameNum())<<endl;
}

//--------------------------------------------------------------
void ofApp::startTracking(NCGenericEventArg &arg){
	if (facetracker.movementcontroller.bIsRecording) {
		ofPoint point = facetracker.movementcontroller.getCurrentFacePoint();
		//map the detection point between 0 and 1
		float mappedx = ofMap(point.x,0,facetracker.detectionframe.width,0,1);
		float mappedy = ofMap(point.y,0,facetracker.detectionframe.height,0,1);
		tcpclient.sendMessage("FACETRACKED/"+NCAppData::NCAPPDATA.cameraid+"/"+ofToString(mappedx)+"/"+ofToString(mappedy));
	} else {
		ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.FACETRACKED, this, &ofApp::facetracked);
		facetracker.toggleTracking(true);
	}
}

//--------------------------------------------------------------
void ofApp::stopTracking(NCGenericEventArg &arg){
	if(facetracker.movementcontroller.bIsRecording) {
		ofRemoveListener(NCEventDispatcher::NCEVENTDISPATCHER.FACETRACKED, this, &ofApp::facetracked);
		facetracker.toggleTracking(false);
	}
}

//--------------------------------------------------------------
void ofApp::getLatestFaceCoord(NCGenericEventArg &arg) {
	
	ofPoint point = facetracker.movementcontroller.getCurrentFacePoint();
	//map the detection point between 0 and 1
	float mappedx = ofMap(point.x,0,facetracker.detectionframe.width,0,1);
	float mappedy = ofMap(point.y,0,facetracker.detectionframe.height,0,1);

	//cout<<"WIM SAYS SEND LATEST FACE COORD"<<mappedx<<" : " <<mappedy<<" : "<<ofToString(ofGetFrameNum())<<endl;

	tcpclient.sendMessage("LATESTFACECOORD/"+NCAppData::NCAPPDATA.cameraid+"/"+ofToString(mappedx)+"/"+ofToString(mappedy));
}

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(33, 33, 33);
    
	idselector.setup();
	facetracker.setup();
    camerafeed.setup();
	tcpclient.setup(1717,"127.0.0.1");
	memusage.setup();

	NCGenericEventArg arg;

	startTracking(arg);

	ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.STARTRACKING, this, &ofApp::startTracking);
	ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.STOPTRACKING, this, &ofApp::stopTracking);
	ofAddListener(NCEventDispatcher::NCEVENTDISPATCHER.GETLATESTFACECOORDIANTES, this, &ofApp::getLatestFaceCoord);
}

//--------------------------------------------------------------
void ofApp::update(){

	memusage.update();

	tcpclient.update();
    
    camerafeed.update();

    ofImage image = camerafeed.getFrameGrayScale();
	facetracker.update(image);

}

//--------------------------------------------------------------
void ofApp::draw(){
    camerafeed.draw();
	facetracker.draw();
	memusage.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='f') {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
