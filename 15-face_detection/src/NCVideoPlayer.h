//
//  NCVideoPlayer.h
//  qMusicFaceTracker
//
//  Created by Wim Vanhenden on 02/02/15.
//
//

#pragma once

#include "ofMain.h"
#include "NCGrid.h"
#include "ofxUI.h"
#include "NCAppData.h"

class NCVideoPlayer {
    
public:
    void setup();
    void update();
    void draw();
    
    void togglePauseVideo();
    
    ofImage         getFrame();
    ofImage         getFrameGrayScale();
    
private:
    
    ofFbo   output;
    ofFbo   preview;
    
    NCGrid grid;
    
    ofImage             videoframe;
    
    ofVideoPlayer       videoplayer;
    ofVideoGrabber      videograbber;
    ofPtr<ofQTKitGrabber>	qtGrabber;
    
    //GUI
    ofxUISuperCanvas        * gui;
    ofxUIDropDownList       * movielistdropdown;
    ofxUIDropDownList       * liveinputlistdropdown;
    
    void guiEvent(ofxUIEventArgs &e);
    
    void startLiveInput(int device);
    
    void startPreRecorded(string filename);
    
    bool isLive;
    bool isPlayingRecorded;
    bool isPaused;
    
};