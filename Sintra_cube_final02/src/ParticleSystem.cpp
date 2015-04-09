//
//  ParticleSystem.cpp
//  ParticleSystem
//
//  Created by tom on 2/25/14.
//
//

#include "ParticleSystem.h"


ParticleSystem::~ParticleSystem()
{
    for(auto it = particles.begin(); it != particles.end(); ++it){
        delete *it;
    }
    particles.clear();
}


void ParticleSystem::update()
{
    for(auto it = particles.begin(); it != particles.end(); ++it){
       (*it)->updateVerlet();
        (*it)->updateHistory();
    }
    
}

void ParticleSystem::draw()
{
    for(auto it = particles.begin(); it != particles.end(); ++it){
        (*it)->draw();
    }
}

void ParticleSystem::drawConnections()
{
    glBegin(GL_LINES);
    for(auto it = particles.begin(); it != particles.end(); ++it){
        Particle *pa = (*it);
        for(auto it2 = particles.begin(); it2 != particles.end(); ++it2){
            Particle *pb = (*it2);
            float d = fabs(pa->position.x - pb->position.x) + fabs(pa->position.y - pb->position.y) + fabs(pa->position.z - pb->position.z);
            if( d > 30 && d < 68){
                glVertex3f(pb->position);
                glVertex3f(pa->position);
                
            }
        }
    }
    glEnd();
}

void ParticleSystem::drawLeft()
{
    int i = 0;
    int numThreshold = int(particles.size()/2.0);
    for(auto it = particles.begin(); it != particles.end(); ++it, ++i){
        if(i < numThreshold)
            (*it)->draw();
        else
            return;
    }
}


void ParticleSystem::drawRight()
{
    int i = 0;
    int numThreshold = int(particles.size()/2.0);
    for(auto it = particles.rbegin(); it != particles.rend(); ++it, ++i){
        if(i < numThreshold)
            (*it)->draw();
        else
            return;
    }
}

void ParticleSystem::drawConnectionsLeft()
{
    int i = 0;
    int j = 0;
    int numThreshold = int(particles.size()/2.0);
    
    for(auto it = particles.begin(); it != particles.end(); ++it, ++i){
        if(i < numThreshold){
            j =0;
            Particle *pa = (*it);
            for(auto it2 = particles.begin(); it2 != particles.end(); ++it2, ++j){
                if(j < numThreshold){
                    Particle *pb = (*it2);
                    float d = pa->position.distance(pb->position);
                    if( d > 30 && d < 68){
                        glBegin(GL_LINES);
                        glVertex3f(pb->position);
                        glVertex3f(pa->position);
                        glEnd();
                    }
                }else{
                    break;
                }
            }
        }else{
            return;
        }
    }
    
}

void ParticleSystem::drawConnectionsRight()
{
    int i = 0;
    int j = 0;
    int numThreshold = int(particles.size()/2.0);
    
    for(auto it = particles.rbegin(); it != particles.rend(); ++it, ++i){
        if(i < numThreshold){
            j = 0;
            Particle *pa = (*it);
            for(auto it2 = particles.rbegin(); it2 != particles.rend(); ++it2, ++j){
                if(j < numThreshold){
                    Particle *pb = (*it2);
                    float d = pa->position.distance(pb->position);
                    if( d > 30 && d < 68){
                        glBegin(GL_LINES);
                        glVertex3f(pb->position);
                        glVertex3f(pa->position);
                        glEnd();
                    }
                }else{
                    break;
                }
            }
        }else{
            return;
        }
    }

}


void ParticleSystem::addParticle( Particle *particle)
{
    particles.push_back(particle);
}

void ParticleSystem::destroyParticle( Particle *particle)
{
    std::vector< Particle *>::iterator it = std::find(particles.begin(), particles.end(), particle);
    delete *it;
    particles.erase( it );
}

std::vector<Particle *>::iterator ParticleSystem::begin()
{
    return particles.begin();
}
std::vector<Particle *>::iterator ParticleSystem::end()
{
    return particles.end();
}

std::vector<Particle *>::reverse_iterator ParticleSystem::rbegin()
{
    return particles.rbegin();
}
std::vector<Particle *>::reverse_iterator ParticleSystem::rend()
{
    return particles.rend();
}
