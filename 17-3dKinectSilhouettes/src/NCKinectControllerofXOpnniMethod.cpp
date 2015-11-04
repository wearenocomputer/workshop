//
//  NCKinectControllerofXOpnniMethod.cpp
//  kinectSilhouette3DTO3DWorld
//
//  Created by Wim Vanhenden on 05/03/14.
//
//

#include "NCKinectControllerofXOpnniMethod.h"


NCKinectControllerofXOpnniMethod* NCKinectControllerofXOpnniMethod::m_pSingleton = NULL;

NCKinectControllerofXOpnniMethod::NCKinectControllerofXOpnniMethod() {
}

NCKinectControllerofXOpnniMethod::~NCKinectControllerofXOpnniMethod() {
}

NCKinectControllerofXOpnniMethod* NCKinectControllerofXOpnniMethod::getInstance() {
    if(m_pSingleton == NULL)
        m_pSingleton = new NCKinectControllerofXOpnniMethod;
    return m_pSingleton;
}

void NCKinectControllerofXOpnniMethod::setup() {
    
    //niContext.setup();
    niContext.setupUsingRecording("oni/crowd_5_p_walking_standing still.oni");
    niDepthGenerator.setup( &niContext );
    niContext.registerViewport();
    niImageGenerator.setup( &niContext );
    niUserGenerator.setup( &niContext );
    
    niContext.setMirror(true);
    
    niDepthGenerator.setDepthThreshold(500,1500);
    ofRegisterKeyEvents(this);
    
    usermap2d.allocate(640, 480);
    bDebugDraw = true;
    bstopGenerating = false;
    breMap2DToScreen2D = false;
    remapfactor = 5.0f;
    
    ofAddListener(niUserGenerator.newUser ,this,&NCKinectControllerofXOpnniMethod::newUserDetected);
    ofAddListener(niUserGenerator.lostUser ,this,&NCKinectControllerofXOpnniMethod::lostUserDetected);
    
}

void NCKinectControllerofXOpnniMethod::update() {
    
    if (!bstopGenerating) {
        
        niContext.update();
        niDepthGenerator.update();
        niImageGenerator.update();
        niUserGenerator.update();
        
        usermap2d.setFromPixels(niUserGenerator.getUserPixels(), 640, 480);
        contourfinder.findContours(usermap2d, 8000, (640*480)/3, 10, false);
        
        contourlines.clear();
        
        for (int i=0; i<contourfinder.nBlobs; ++i) {
            ofPolyline cline;
            for (int j=0; j<contourfinder.blobs[i].pts.size(); ++j) {
                
                if (breMap2DToScreen2D) {
                    float xpos = contourfinder.blobs[i].pts[j].x;
                    float ypos =  contourfinder.blobs[i].pts[j].y;
                    cline.addVertex(remapFrom2DTo3DCoord(xpos, ypos));
                } else {
                    cline.addVertex(contourfinder.blobs[i].pts[j]);
                }
            }
            cline.close();
            contourlines.push_back(cline);
        }
    
    } else {
        contourlines.clear();
    }
    
    whoIsStandingStill();
}

void NCKinectControllerofXOpnniMethod::draw() {
    
    
    if (bDebugDraw) {
        niDepthGenerator.draw(ofGetWidth()-320, ofGetHeight()-240,320,240);
        niImageGenerator.draw(ofGetWidth()-640, ofGetHeight()-240,320,240);
        usermap2d.draw(ofGetWidth()-960, ofGetHeight()-240,320,240);
        contourfinder.draw(ofGetWidth()-960, ofGetHeight()-240,320,240);
        
        /*
        ofSetColor(255, 0, 0);
        for (int i=0;i<lmrusers.size();++i) {
            XnPoint3D userPos;
            niUserGenerator.getXnUserGenerator().GetCoM(lmrusers[i].id, userPos);
            XnPoint3D projective;
            niDepthGenerator.getXnDepthGenerator().ConvertRealWorldToProjective(2, &userPos, &projective);
            ofDrawBitmapString(ofToString(lmrusers[i].id), projective.X,projective.Y);
        }
        ofSetColor(255, 255, 255);*/
    }
    
}

void NCKinectControllerofXOpnniMethod::keyPressed(ofKeyEventArgs &args) {
    if (args.key== 'o') {
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select an oni file");
        //Check if the user opened a file
        if (openFileResult.bSuccess){
            ofFile file (openFileResult.getPath());
            niContext.shutdown();
            niContext.setupUsingRecording(file.path());
            niContext.registerViewport();
            niDepthGenerator.setup( &niContext );
            niImageGenerator.setup( &niContext );
            niUserGenerator.setup( &niContext );
            niContext.setMirror(true);
            
            ncusers.clear();
        }
    }
    
    if (args.key== 'r') {
        niContext.shutdown();
        niContext.setup();
        niContext.registerViewport();
        niDepthGenerator.setup( &niContext );
        niImageGenerator.setup( &niContext );
        niUserGenerator.setup( &niContext );
        niContext.setMirror(true);
        ncusers.clear();
    }
    
    if (args.key == 'd') {
        bDebugDraw = !bDebugDraw;
    }
    
    if (args.key == 'k') {
        bstopGenerating = !bstopGenerating;
    }
}

void NCKinectControllerofXOpnniMethod::keyReleased(ofKeyEventArgs &args) {
    
}

void NCKinectControllerofXOpnniMethod::whoIsStandingStill() {
    
  
    for (int i=0; i<ncusers.size(); ++i) {
        
        XnPoint3D userPos;
        niUserGenerator.getXnUserGenerator().GetCoM(ncusers[i].id, userPos);
        
        //check if user is not out of kinect space
        if(userPos.X!=0) {
            
            //convert to 2D screen coordinates
            XnPoint3D projective;
            niDepthGenerator.getXnDepthGenerator().ConvertRealWorldToProjective(2, &userPos, &projective);
            
            if (ncusers[i].update(projective.X)>=0) {
                //cout<<"WIM SAYS "<<ncusers[i].id<<" is standing still"<<endl;
                //remap 2D coord into our 3D world
                ofPoint point = remapFrom2DTo3DCoord(projective.X, projective.Y);
                ofNotifyEvent(userStandingStillDetected,point,this);
            }
        }
    }
}

ofPoint NCKinectControllerofXOpnniMethod::remapFrom2DTo3DCoord(float xpos, float ypos) {
    float newspox = ofMap(xpos, 0, 640, -(remapfactor+1), remapfactor+1);
    float newypos = ofMap(ypos, 0, 480, -remapfactor, remapfactor);
    return ofPoint(newspox,newypos);
}

void NCKinectControllerofXOpnniMethod::newUserDetected(int &f) {
    std::vector<NCKinectUser>::iterator it;
    it = std::find(ncusers.begin(),ncusers.end(),f);
    if (it == ncusers.end()) {
        NCKinectUser newuser;
        newuser.setup(f);
        ncusers.push_back(newuser);
    }
}

void NCKinectControllerofXOpnniMethod::lostUserDetected(int &f) {
    std::vector<NCKinectUser>::iterator it;
    it = std::find(ncusers.begin(),ncusers.end(),f);
    if (it != ncusers.end()) {
        ncusers.erase(it);
    }
}