//
//  NCVideoPlayer.cpp
//  qMusicFaceTracker
//
//  Created by Wim Vanhenden on 02/02/15.
//
//

#include "NCVideoPlayer.h"

void NCVideoPlayer::setup() {
    
    isLive = false;
    isPlayingRecorded = false;
    
    
    output.allocate(NCAppData::NCAPPDATA.outputwidthheight.x,NCAppData::NCAPPDATA.outputwidthheight.y);
    preview.allocate(NCAppData::NCAPPDATA.areawidthheight.x,NCAppData::NCAPPDATA.areawidthheight.y);
    
    qtGrabber = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
    videograbber.setGrabber(qtGrabber);
    videograbber.setVerbose(false);
    
    //SETUP GUI
    gui = new ofxUISuperCanvas("VIDEO INPUT", OFX_UI_FONT_MEDIUM);
    gui->setPosition(0, 0);
    
    //SET MOVIE DROPDOWN
    gui->addSpacer();
    vector<string> fileNames; ofDirectory dir; dir.listDir("./MOVIES"); dir.sort();
    for (unsigned i = 0; i < dir.size(); ++i) {
        fileNames.push_back(dir.getName(i));
    }
    movielistdropdown = gui->addDropDownList("Choose prerecorded", fileNames);
    movielistdropdown->setAllowMultiple(false);
    movielistdropdown->setAutoClose(true);
    
    //SET LIVE INPUT DROPDOWN
    gui->addSpacer();
    vector<string>videoDevices = qtGrabber->listVideoDevices();
    liveinputlistdropdown = gui->addDropDownList("Choose live input", videoDevices);
    liveinputlistdropdown->setAllowMultiple(false);
    liveinputlistdropdown->setAutoClose(true);
    
    //AUTOSIZE GUI
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&NCVideoPlayer::guiEvent);
    
    grid.setup(ofRectangle(0,0,NCAppData::NCAPPDATA.areawidthheight.x,NCAppData::NCAPPDATA.areawidthheight.y), 40);
    
}

void NCVideoPlayer::update() {
    //videoplayer.update();
    if(isLive) {
        videograbber.update();
        if (videograbber.isFrameNew()) {
            output.begin();
            videograbber.draw(0,0,output.getWidth(),output.getHeight());
            output.end();
            
            preview.begin();
            videograbber.draw(0,0,preview.getWidth(),preview.getHeight());
            preview.end();
        }
    }
    
    if(isPlayingRecorded) {
        videoplayer.update();
        if (videoplayer.isFrameNew()) {
            output.begin();
            videoplayer.draw(0,0,output.getWidth(),output.getHeight());
            output.end();
            
            preview.begin();
            videoplayer.draw(0,0,preview.getWidth(),preview.getHeight());
            preview.end();
        }
    }
}

ofImage NCVideoPlayer::getFrame() {
    
    output.readToPixels(videoframe.getPixelsRef());
    
    return videoframe;
}

ofImage NCVideoPlayer::getFrameGrayScale() {
    
    output.readToPixels(videoframe.getPixelsRef());
    videoframe.setImageType(OF_IMAGE_GRAYSCALE);
    return videoframe;
}

void NCVideoPlayer::draw() {
    
    if(isLive) {
        videograbber.draw(0,0);
    }
    
    if(isPlayingRecorded) {
        videoplayer.draw(0,0);
    }

    grid.draw();
}

//--------------------------------------------------------------
void NCVideoPlayer::guiEvent(ofxUIEventArgs &e) {
    
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    if(kind == OFX_UI_WIDGET_DROPDOWNLIST) {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        
        if (ddlist->getName()=="Choose prerecorded") {
            if (ddlist->getSelectedIndeces().size()==1) {
                startPreRecorded(ddlist->getSelectedNames()[0]);
                ddlist->clearSelected();
            }
        }
        
        if (ddlist->getName()=="Choose live input") {
            
            if (ddlist->getSelectedIndeces().size()==1) {
                startLiveInput(ddlist->getSelectedIndeces()[0]);
                ddlist->clearSelected();
            }
        }
        
    }
}

//--------------------------------------------------------------
void NCVideoPlayer::startLiveInput(int device) {
    
    isLive = false;
    isPlayingRecorded = false;
    
    videoplayer.close();
    videograbber.close();
    qtGrabber->close();
    qtGrabber->setDeviceID(device);
    videograbber.initGrabber(NCAppData::NCAPPDATA.areawidthheight.x,NCAppData::NCAPPDATA.areawidthheight.y);
    isLive = true;
}


//--------------------------------------------------------------
void NCVideoPlayer::startPreRecorded(string filename) {
    isLive = false;
    isPlayingRecorded = false;
    videograbber.close();
    qtGrabber->close();
    videoplayer.close();
    videoplayer.loadMovie("MOVIES/"+filename);
    videoplayer.play();
    isPlayingRecorded = true;
}

//--------------------------------------------------------------
void NCVideoPlayer::togglePauseVideo() {
    if (isPlayingRecorded) {
        if (isPaused) {
            videoplayer.play();
            isPaused = false;
        } else {
             videoplayer.stop();
            isPaused = true;
        }
       
    }
}


