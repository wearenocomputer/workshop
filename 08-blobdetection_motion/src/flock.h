#pragma once

#include "ofMain.h"
#include "Particle.h"

class Flock : public ofNode {
public:

    Flock();
    
    float mass;
    float gravitation;
    float friction;
    float timestep;
    
    vector<Particle> particles;
    vector<Particle> attractors;

    void addParticle(Particle particle);
    void addAttractor(Particle particle);

    void update();
    void draw();

};