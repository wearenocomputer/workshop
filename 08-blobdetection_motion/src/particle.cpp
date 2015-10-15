#include "Particle.h"

//--------------------------------------------------------------
Particle::Particle(ofVec3f _position, float _mass, ofVec3f _velocity){
  
    position = _position;
    velocity = _velocity;
    unweightedMass = _mass;
    mass = _mass;

}

//--------------------------------------------------------------
void Particle::reset(){
    
    acceleration.set(0, 0, 0);

}

//--------------------------------------------------------------
void Particle::apply(float gravitation, float friction){
    
    acceleration *= gravitation * mass;
    velocity += acceleration;
    velocity *= friction;

}

//--------------------------------------------------------------
void Particle::update(float timestep){

    position += velocity * timestep;

}