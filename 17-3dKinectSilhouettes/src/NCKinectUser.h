//
//  LMRUser.h
//  kinectUserManagament
//
//  Created by Wim Vanhenden on 18/03/14.
//
//

#pragma once

#include "ofMain.h"

class NCKinectUser {
    
public:
    
    void setup(int _id);
    
    int update(float _xpos);
    
    int id;
    
    bool operator==(const int &incid) const {
		return id == incid;
	}
    
	bool operator < (const NCKinectUser& rhs) const {
		return id < rhs.id;
	}
    
private:
    
    int buffer;
    vector<float> xpositions;
    int avcounter;
    
    vector<float> avs;
    
};