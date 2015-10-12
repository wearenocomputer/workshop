#pragma once

#include "ofMain.h"

class Particle : public ofNode{
public:
    
    float mass;
    float unweightedMass;
    
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f acceleration;
    
    Particle(ofVec3f _position, float _mass = 1, ofVec3f _velocity = ofVec3f(0, 0, 0));

    void reset();
    void apply(float gravitation, float friction);
    void update(float timestep);
    
};