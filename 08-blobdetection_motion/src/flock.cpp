#include "Flock.h"

Flock::Flock(){
    
    mass = 1;
    friction = .96;
    gravitation = .05;
    timestep = 1;

}

//--------------------------------------------------------------
void Flock::addParticle(Particle particle){
    
    particles.push_back(particle); 
    
}

//--------------------------------------------------------------
void Flock::addAttractor(Particle particle){
    
    attractors.push_back(particle); 
    
}

//--------------------------------------------------------------
void Flock::update(){
    
    
    // Update particles
    
    for(int i=0; i<particles.size(); ++i) {

        Particle &p = particles[i];

        p.reset();
        p.mass = p.unweightedMass * mass;
        
        // Check with attractors
        for(int j=0; j<attractors.size(); ++j) {
            
            // get direction
            ofVec3f dir = p.position - attractors[j].position;
            // get distance
            float dist = dir.lengthSquared();
            // get force
            float f = pow(attractors[j].mass, -1);
            // if too close, inverse attraction
            if( dist < attractors[j].mass * attractors[j].mass) dir *= -1;
            // add force
            p.acceleration -= dir * f;
            
        }

        // Check with other particles
        for(int j=0; j<particles.size(); ++j) {
            
            // don't check with itself
            if(i==j) continue;
            
            // get direction
            ofVec3f dir = p.position - particles[j].position;
            // get distance
            float dist = dir.lengthSquared();
            // get force
            float f = particles[j].mass * pow(dist, -1);
            // add force
            p.acceleration += dir * f;
        
        }
        
        // Apply force and
        // update positions
        
        p.apply(gravitation, friction);
        p.update(timestep);
        
    }  
    
    
    //for(int i=0; i<attractors.size(); ++i) attractors[i].update(timestep);
    
    
    
}

//--------------------------------------------------------------
void Flock::draw() {
    
    ofPushStyle();
    
    for(int i=0; i<particles.size(); ++i) {
        
        Particle &p = particles[i];

        float random = ofRandom(125, 255);
        
        ofNoFill();
        ofSetColor(1, random, random * 0.75);
        ofCircle( particles[i].position, particles[i].mass);
        ofFill();
        ofCircle( particles[i].position, particles[i].mass * 0.3);
        
    }
    
    ofPopStyle();


    for(int i=0; i<attractors.size(); ++i) {
        
        ofFill();
        ofCircle(attractors[i].position,attractors[i].mass * .1);
        
    }
    
}