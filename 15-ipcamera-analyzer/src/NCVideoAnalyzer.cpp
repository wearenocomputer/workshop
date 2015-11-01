//
//  NCVideoAnalyzer.cpp
//  Camera
//
//  Created by Wim Vanhenden on 06/09/13.
//
//

#include "NCVideoAnalyzer.h"

void NCVideoAnalyzer::setup(int _width, int _height,bool _usewebcam=true, bool _uselivefeed=false, bool _userecrordevideo=false) {
	croppedarea = ofRectangle(0,0,_width,_height);
	internSetup(_width, _height,_usewebcam,_uselivefeed,_userecrordevideo);

}

void NCVideoAnalyzer::setup(int _width, int _height, ofRectangle _crop,bool _usewebcam=true, bool _uselivefeed=false, bool _userecrordevideo=false) {
	croppedarea = _crop;
	internSetup(_width, _height,_usewebcam,_uselivefeed,_userecrordevideo);
}

void NCVideoAnalyzer::internSetup(int incwidth, int incheight,bool _usewebcam, bool _uselivefeed, bool _userecrordevideo) {

	docontourfinding = false;

	usewebcam = _usewebcam;
	uselivestream = _uselivefeed;
	userecordevideo = _userecrordevideo;

	absdiff = 45;
	contoursize = 500;
	brightness = 0;
	contrast =0;
	threshhold =0;

	gui = new ofxUICanvas(0, 0, ofGetWidth(), ofGetHeight());
	gui->setColorBack(ofColor(255,255,255,0));

	gui->addSlider("brightness", -127, 127, &brightness);
	gui->addSlider("contrast", -127, 127, &contrast);
	gui->addSlider("threshold", 0, 255, &threshhold);
	gui->addSlider("absdiff", 0, 255, &absdiff);
	gui->addSlider("contoursize", 0, 1000, &contoursize);

	width = incwidth;
	height = incheight;


	if (uselivestream) {
		//ipcam.setUsername("admin");
		//ipcam.setPassword("admin");
		//ipcam.setURI("http://192.168.0.102:10002/cgi-bin/guest/Video.cgi?media=mjpeg&resolution=vga");
		//ipcam.setURI("http://192.168.0.102:10002/cgi-bin/guest/Video.cgi?media=mjpeg");
		ipcam.setURI("http://195.58.188.49:8081/mjpg/video.mjpg");
		ipcam.connect();
	}

	if (userecordevideo) {
		video.loadMovie(ofToDataPath("synapse2_test_capture_2.mov"));
		video.play();
	}

	if (usewebcam) {
        grabber.setDeviceID(0);
		grabber.initGrabber(width,height);

	}
}

void NCVideoAnalyzer::update() {
	bool bNewFrame = false;

	if (uselivestream) {
		ipcam.update();
		bNewFrame = ipcam.isFrameNew();
	}

	if (userecordevideo) {
		video.update();
		bNewFrame = video.isFrameNew();
	}

	if (usewebcam) {
		grabber.update();
		bNewFrame = grabber.isFrameNew();
	}

	if (bNewFrame){

		if (uselivestream) {
			cropped.setFromPixels(ipcam.getPixels(), width, height, OF_IMAGE_COLOR);
			
		}

		if (userecordevideo) {
			cropped.setFromPixels(video.getPixels(), width, height, OF_IMAGE_COLOR);
		
		}

		if (usewebcam) {
			cropped.setFromPixels(grabber.getPixels(), width, height, OF_IMAGE_COLOR);
		}

		cropped.crop(croppedarea.getX(), croppedarea.getY(), croppedarea.getWidth(), croppedarea.getHeight());
		incomingframe.setFromPixels(cropped);

		incomingframebw = incomingframe;
		doBrightnessAndContrast(incomingframebw, brightness, contrast);
		doThreshold(brighimage, threshhold);
		doDifference();
		doDilation(difference.getPixelsRef(), 5);
		if (docontourfinding) {
			findContours(dilate);
		}
	}
}

void NCVideoAnalyzer::doThreshold(ofxCvGrayscaleImage _image, float _value) {
	thresimage.setFromPixels(_image.getPixelsRef());
	thresimage.threshold(_value,true);
}

void NCVideoAnalyzer::doBrightnessAndContrast(ofxCvGrayscaleImage _image,  float brightnessAmount, float contrastAmount){

	brighimage.setFromPixels(_image.getPixelsRef());
	float brightnessVal = brightnessAmount;
	float contrastVal = contrastAmount;

	unsigned char data[ 256 ];
	CvMat * matrix;
	double delta, a, b;

	matrix = cvCreateMatHeader( 1, 256, CV_8UC1 );
	cvSetData( matrix, data, 0 );

	if ( contrastVal>0 ) {
		delta = (127.0f*contrastVal) / 128.0f;
		a = 255.0f / ( 255.0f-(delta*2.0f) );
		b = a * (brightnessVal-delta);
	}
	else {
		delta = (-128.0f*contrastVal) / 128.0f;
		a = ( 256.0f-(delta*2.0f) ) / 255.0f;
		b = ( a*brightnessVal )+delta;
	}

	for( int i=0; i<256; i++ ) {
		int value = cvRound( (a*i)+b );
		data[i]	= (unsigned char) min( max(0,value), 255 );
	}

	cvLUT( brighimage.getCvImage(), brighimage.getCvImage(), matrix );
	cvReleaseMat( &matrix );
}

void NCVideoAnalyzer::doDifference() {
	newframe = brighimage;
	if (oldframe.getHeight()!=0) {
		isready = true;
		doDifferenceRoutine();
	}
	oldframe = newframe;
}

void NCVideoAnalyzer::doDifferenceRoutine() {

	if (isready) {

		difference.absDiff(oldframe, newframe);

		unsigned char *temppixels = difference.getPixels();

		for (int j=0;j<croppedarea.width*croppedarea.height;j++) {

			if (temppixels[j] > absdiff) {
				temppixels[j]=255;
			} else {
				temppixels[j]=0;
			}

		}
	}
}

void NCVideoAnalyzer::doDilation(ofPixelsRef &incimage, int times) {
	if (isready) {
		dilate.setFromPixels(incimage);

		for (int i=0;i<times;++i) {
			dilate.dilate_3x3();
		}
		dilate.updateTexture();
	}
}

void NCVideoAnalyzer::findContours(ofxCvGrayscaleImage &incimage) {
	if (isready) {
		contourFinder.findContours(incimage, contoursize, (croppedarea.width*croppedarea.height)/3, 10, false);
	}
}

void NCVideoAnalyzer::draw() {
	float yoffset = 200;
	incomingframebw.draw(0, yoffset);
	brighimage.draw(croppedarea.width, yoffset);
	thresimage.draw(croppedarea.width*2, yoffset);
    
    //dilate.draw(0, yoffset+croppedarea.height);
	//contourFinder.draw(croppedarea.width,yoffset+croppedarea.height);
    
    
	oldframe.draw(0,yoffset+croppedarea.height);
	newframe.draw(croppedarea.width,yoffset+croppedarea.height);
	difference.draw(croppedarea.width*2, yoffset+croppedarea.height);
	
    dilate.draw(0, yoffset+croppedarea.height*2);
	ofSetColor(0,0,0);
	ofRect(croppedarea.width, yoffset+croppedarea.height*2, croppedarea.width, croppedarea.height);
	contourFinder.draw(croppedarea.width, yoffset+croppedarea.height*2);
	ofSetColor(255,255,0);

	if (uselivestream) {
			ofDrawBitmapString("camera fps: "+ofToString(ipcam.getFrameRate()),0,yoffset-10);
	}

	ofDrawBitmapString("live image cropped grayscale", 0,yoffset+10);
	ofDrawBitmapString("brightness contrast image", croppedarea.width,yoffset+10);
	ofDrawBitmapString("inverse threshold image", croppedarea.width*2,yoffset+10);

	ofDrawBitmapString("previous frame", 0,yoffset+croppedarea.height+10);
	ofDrawBitmapString("current frame", croppedarea.width,yoffset+croppedarea.height+10);
	ofDrawBitmapString("absolute difference frame", croppedarea.width*2,yoffset+croppedarea.height+10);
	ofDrawBitmapString("dilation", 0,yoffset+croppedarea.height*2+10);

	ofSetColor(255, 255, 255);
}
