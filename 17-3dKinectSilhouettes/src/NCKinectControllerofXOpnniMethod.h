#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "NCUserGenerator.h"
#include "ofxOpenCv.h"
#include "NCKinectUser.h"

class NCKinectControllerofXOpnniMethod {
    private:
        static NCKinectControllerofXOpnniMethod* m_pSingleton;
    
        ofxOpenNIContext                niContext;
        ofxDepthGenerator               niDepthGenerator;
        ofxImageGenerator               niImageGenerator;
        NCUserGenerator                 niUserGenerator;
        ofxCvGrayscaleImage             usermap2d;
        ofxCvContourFinder              contourfinder;

        bool                            bstopGenerating;
    
        vector<NCKinectUser>            ncusers;
    
        void newUserDetected(int &f);
        void lostUserDetected(int &f);
    
        void whoIsStandingStill();

    
        ofPoint remapFrom2DTo3DCoord(float xpos, float ypos);
    

    protected:
        NCKinectControllerofXOpnniMethod();
    
    public:
    
        virtual ~NCKinectControllerofXOpnniMethod();
    
        static NCKinectControllerofXOpnniMethod* getInstance();
    
        void setup();
        void update();
        void draw();
    
        void keyPressed(ofKeyEventArgs &key);
        void keyReleased(ofKeyEventArgs &key);
    
        bool breMap2DToScreen2D;
    
        float remapfactor;
    
        bool                            bDebugDraw;
    
        //2D STUFF
        vector<ofPolyline>               contourlines;
    
        ofEvent<ofPoint>                 userStandingStillDetected;
};