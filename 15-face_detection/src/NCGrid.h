//
//  NCGrid.h
//  panasonicPTZ50Calibration
//
//  Created by Wim Vanhenden on 13/02/15.
//
//

#pragma once

#include "ofMain.h"

class NCGrid {
    
public:
    void setup(ofRectangle _frame, int _gridsize=20);
    void draw();
    
    //We need to declare all this mouse events methods to be able to listen to mouse events.
    //All this must be declared even if we are just going to use only one of this methods.
    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    
    
    ofEvent<vector <float> > GRIDCLICKED;
    ofRectangle frame;
private:
   
    ofPoint selectedpoint;
    ofPoint selectedpointconverted;
    int gridsize;
};
