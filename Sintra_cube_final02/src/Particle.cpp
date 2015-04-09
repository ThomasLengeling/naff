//
//  Particle.cpp
//  ParticleSystem
//
//  Created by tom on 2/25/14.
//
//

#include "Particle.h"


Particle::Particle( const ci::Vec3f & position, float radius, float mass, float drag){
    this->position = position;
    this->radius   = radius;
    this->mass     = mass;
    this->drag     = drag;
    prevPosition   = position;
    forces         = ci::Vec3f::zero();
    tailLength     = 16;
}


void Particle::updateVerlet()
{
    ci::Vec3f temp = position;
    
    //Velocity
    ci::Vec3f vel = (position - prevPosition) * drag;
    
    position += vel + forces / mass;
    
    prevPosition =  temp;
    
    forces = ci::Vec3f::zero();
    
 
}

void Particle::updateHistory()
{
    positionHistory.push_back(position);
    if(positionHistory.size() > tailLength){
        positionHistory.erase(positionHistory.begin());
    }
}

void Particle::draw()
{
    ci::gl::drawSphere(position, radius, 14);
    
 //   ci::gl::drawSolidCircle(ci::Vec2f(position.x, position.y), position.z/10.0);
    
    drawHistory();
    
    //drawArcs();
}

void Particle::drawArcs()
{
    ci::gl::pushMatrices();
    ci::gl::translate(position.x, position.y, position.z);
    
    ci::gl::drawSphere(ci::Vec3f::zero(), radius, 16);
    
    float r = ci::app::getElapsedSeconds() * (1.0f + 10);
    ci::gl::rotate(ci::Vec3f::one() * r);
    
    ci::gl::color(1.0, 1.0, 1.0);
    arc(0, 0, radius, 2);
    ci::gl::popMatrices();
}

void Particle::arc(float x, float y, float rad,float w) {
   /* glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < 90; i++) {
        glVertex3f(triLUT.getCOS(i)*(rad) + x,     triLUT.getSIN(i)*(rad) + y, 0);
        glVertex3f(triLUT.getCOS(i)*(rad + w) + x, triLUT.getSIN(i)*(rad + w) + y, 0);
    }
    glEnd();
    */
}

void Particle::drawHistory()
{
    glBegin(GL_LINE_STRIP);
    int nPH = positionHistory.size();
    for(int i = 0; i < nPH;  i++){
        float alpha = (float) i / (float) nPH + 0.05;
        ci::gl::color(ci::ColorA(0.1f, 0.55f, 0.6f, alpha));
        ci::gl::vertex(positionHistory[i]);
    }
    glEnd();
}
