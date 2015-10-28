//
//  NCGrid.cpp
//  panasonicPTZ50Calibration
//
//  Created by Wim Vanhenden on 13/02/15.
//
//

#include "NCGrid.h"


void NCGrid::setup(ofRectangle _frame, int _gridsize) {
    frame = _frame;
    gridsize = _gridsize;
    ofRegisterMouseEvents(this);
}


void NCGrid::draw() {
    //DRAW GRID

    int xsteps = frame.width/gridsize;
    int ysteps = frame.height/gridsize;
    
    float middlex =(frame.width/2);
    float middley =(frame.height/2);
    
    float beginx = 0;
    float beginy = 0;
    
    float endx = frame.width;
    float endy = frame.height;
    
    
    ofNoFill();
    ofRect(frame);
    ofFill();
    
    ofPushMatrix();
    ofTranslate(frame.x,frame.y);
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255,100);
    
    for (int i=0;i<(ysteps/2)+1;i++) {
        ofLine(beginx, middley-(i*gridsize), endx, middley-(i*gridsize));
        ofLine(beginx, middley+(i*gridsize), endx, middley+(i*gridsize));
    }
    for (int i=0;i<(xsteps/2)+1;i++) {
        ofLine(middlex-(i*gridsize), beginy, middlex-(i*gridsize),endy);
        ofLine(middlex+(i*gridsize), beginy, middlex+(i*gridsize),endy);
    }
    ofSetColor(255,255,255);
    ofDisableAlphaBlending();
    
    //DRAW CENTER LINES
    ofSetColor(0,255,0);
    ofLine(middlex, beginy, middlex,endy);
    ofLine(beginx, middley, endx, middley);
    ofCircle(middlex, middley,10);
    
    //draw half of half
    //first half x
    float dist = (middlex-beginx)/2;
    float xpointfirst = beginx+dist;
    ofCircle(xpointfirst, middley,6);
    //second half x
    float dists = (endx-middlex)/2;
    float xpointsecond = middlex+dists;
    ofCircle(xpointsecond, middley,6);
    
    //first half y
    float ydistfirst = (middley-beginy)/2;
    float yfirst = middley-ydistfirst;
    ofCircle(middlex, yfirst,6);
    
    //second half y
    float ydistsec = (endy-middley)/2;
    float ysecond = middley+ydistsec;
    ofCircle(middlex, ysecond,6);
    
    ofPopMatrix();
    
    //DRAW SELECTED POINT
    ofSetColor(255, 0, 0);
    ofCircle(selectedpoint.x, selectedpoint.y, 5);
    ofSetColor(255,255,255);
}


void NCGrid::mouseMoved(ofMouseEventArgs & args){}
void NCGrid::mouseDragged(ofMouseEventArgs & args){}
void NCGrid::mousePressed(ofMouseEventArgs & args){}
void NCGrid::mouseReleased(ofMouseEventArgs & args){
    ofPoint temp = ofPoint(args.x, args.y);
    if (frame.inside(temp)) {
        selectedpoint = ofPoint(args.x,args.y);
        selectedpointconverted = ofPoint(selectedpoint.x-frame.x, selectedpoint.y-frame.y);
        vector<float> args;
        args.push_back(selectedpointconverted.x);
        args.push_back(selectedpointconverted.y);
        ofNotifyEvent(GRIDCLICKED,args,this);
    }
}