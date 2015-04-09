//
//  ParticleSystem.h
//  ParticleSystem
//
//  Created by tom on 2/25/14.
//
//

#pragma once

#include "Particle.h"
#include <vector>

class ParticleSystem{
public:
    ~ParticleSystem();
    
    void update();
    void draw();
    void drawConnections();
    
    void drawLeft();
    void drawRight();
    
    void drawConnectionsLeft();
    void drawConnectionsRight();
    
    void addParticle(Particle *particle);
    void destroyParticle(Particle *particle);
    
    void clear(){particles.clear();}
    
    void particleMoveTo(unsigned int i, ci::Vec3f & dst) { particles[i]->moveTo(dst);}
    
    int size(){return particles.size();}
    
    inline std::vector<Particle *> getVectorPoints();
    
    
    //ITRERATORS
    std::vector<Particle *>::iterator           begin();
    std::vector<Particle *>::iterator           end();
    std::vector<Particle *>::reverse_iterator   rbegin();
    std::vector<Particle *>::reverse_iterator   rend();
    
private:
    
    std::vector<Particle *> particles;
};


std::vector<Particle *>  ParticleSystem::getVectorPoints()
{
    return particles;
}