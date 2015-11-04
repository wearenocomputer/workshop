//
//  LMRUser.cpp
//  kinectUserManagament
//
//  Created by Wim Vanhenden on 18/03/14.
//
//

#include "NCKinectUser.h"


void NCKinectUser::setup(int _id) {
    id = _id;
    buffer=90;
    avcounter=0;
}

int NCKinectUser::update(float x_pos) {
    
    if (xpositions.size()>buffer-1) {
        xpositions.erase(xpositions.begin());
        
        std::sort(xpositions.begin(), xpositions.end());
        float av;
        for (int i=0;i<xpositions.size();++i) {
            av +=xpositions[i];
        }
        
        av=av/buffer;
        
        avcounter++;
        
        if (avcounter==91) {
            
            float max =  *max_element(avs.begin(), avs.begin()+90);
            float min = *min_element(avs.begin(), avs.begin()+90);
            //cout<<min<<" : "<<max<<" : "<<max-min<<endl;
            if (max-min<0.3) {
                //cout<<"user "<<id<<" is standing still"<<endl;
                avcounter = 0;
                avs.clear();
                return id;
            }
            avcounter = 0;
            avs.clear();
        } else {
            avs.push_back(av);
        }
        //cout<<(int)av<<" : "<<max<<" : "<<min<<" : "<<abs(max-min)<<endl;
    }
    xpositions.push_back(x_pos);
    
    return -1;
}