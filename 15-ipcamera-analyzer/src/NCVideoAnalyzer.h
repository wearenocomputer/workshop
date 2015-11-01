//
//  NCVideoAnalyzer.h
//  Camera
//
//  Created by Wim Vanhenden on 06/09/13.
//
//
#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxIpVideoGrabber.h"
#include "ofxUI.h"

class NCVideoAnalyzer {
  
  public:

    ofxCvColorImage         incomingframe;
    
    ofxCvGrayscaleImage     incomingframebw;
   
    ofxCvGrayscaleImage     brighimage;
    ofxCvGrayscaleImage     thresimage;
    
    ofxCvGrayscaleImage     oldframe;
    ofxCvGrayscaleImage     newframe;
    
    ofxCvGrayscaleImage     difference;
    
    ofxCvGrayscaleImage     erode;
    ofxCvGrayscaleImage     dilate;
    ofxCvGrayscaleImage     blur;

    ofxCvContourFinder      contourFinder;
    
    ofImage                 croppedtest;
    
    void setup(int _width, int _height,bool _usewebcam, bool _uselivefeed, bool _userecrordevideo);
    void setup(int _width, int _height, ofRectangle crop,bool _usewebcam, bool _uselivefeed, bool _userecrordevideo);
    void update();
    void draw();
    
    float brightness;
    float contrast;
    float threshhold;
    float absdiff;
    float contoursize;
    
    void doThreshold(ofxCvGrayscaleImage _image, float _value);
    void doBrightnessAndContrast(ofxCvGrayscaleImage _image,  float brightnessAmount, float contrastAmount);
    void doDifference();
    void doDilation(ofPixelsRef &incimage, int times);
    void findContours(ofxCvGrayscaleImage &incimage);
    
    bool isready;
	bool docontourfinding;

    ofxIpVideoGrabber   ipcam;
	ofVideoGrabber		grabber;
    ofVideoPlayer 		video;
    
    
private:

    
     ofPixels cropped;
    int width;
    int height;
   
    void internSetup(int incwidth, int incheight,bool _usewebcam, bool _uselivefeed, bool _userecrordevideo);
    ofRectangle         croppedarea;
    ofxUICanvas *gui;
    void doDifferenceRoutine();
	bool usewebcam;
	bool uselivestream;
	bool userecordevideo;
    
};
