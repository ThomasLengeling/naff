#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"

#include "cinder/MayaCamUI.h"

#include "cinder/params/Params.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"

#include "Resources.h"

#include "ParticleSystem.h"
#include "AudioAnaylzer.h"

#include "Room.h"
#include "SpringCam.h"
#include "CubeMap.h"

#include "MidiIn.h"
#include "MidiMessage.h"
#include "MidiConstants.h"

#include "OscListener.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define NUMPARTICLE 512

#define APP_WIDTH  1920
#define APP_HEIGHT 1080

#define ROOM_FBO_RES	1

#define PORT       32000

/*
 spline moving attraction position
 camera change zoom and out.
 
 simulate the initial setup
 change colors.
 
 shader spheres joining and exploding.
 
 
 when creating a sphere with custom index, normals and vertexs, by
 definition its a convex hull of a sphere, then changing the vertex will transform everytime the sphere
 but eveytime its a sphere by definition of the index of the vertices.
 */

class ParticleSystemApp : public AppNative {
public:
    
	void setup();
	void prepareSettings( Settings *settings );
    void mouseDown( MouseEvent event );
    void mouseMove( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseUp( MouseEvent event);
    void keyDown(KeyEvent event);
    
	void update();
	void draw();
    void resize();
    
    //SECOND WINDOW
    void         drawMain();
    void         shutDown();
    
    //AUDIO
    AudioAnalyzer * mAudio;
    
    //PARTICLES
    ParticleSystem *mParticleSystem;
    
    //POSITIONS
    ci::Vec3f attrPositionCenter;
    ci::Vec3f attrPositionLeft;
    ci::Vec3f attrPositionRight;
    
    float     attrPositionFactor;
    
    ci::Vec3f  translateRight;
    
    
    float attrFactor;
    float repulsionFactor;
    float repulsionRadius;
    
    CameraPersp mCam;
    ci::Quatf camOrientation;
    
    //PARTICLES REACTIVE
    void particleBeatForce();
    void particleBeatForceGrow();
    
    void particleBeatForceLeft();
    void particleBeatForceRight();
    
    bool activateBeatGrow;
    
    ci::Vec3f sphericalToCartesians(float radius, float theta, float phi);
    
    void drawParticles();
    void drawShadedParticles();
    
    void drawShadedParticlesRight();
    void drawShadedParticlesLeft();
    
    void particleBeatForceGrowRight();
    void particleBeatForceGrowLeft();
    
    void drawIntoFboScene();
    
    void createNewParticles();
    
    double    forceIncrement;
    
    ci::ColorA colorConectionLeft;
    ci::ColorA colorConectionRight;
    
    float       lineWidthFactor;
    
    float    rotation;
    float    rotationInc;
    float    rotationMax;
    void     rotateScene();
    
    //PARAMS
    ci::params::InterfaceGlRef mParams;
    
    float    avgFps;
    float    randAngle;
    
    //GLSL
    gl::GlslProg    mDifuseShader;
    
    //BLOOM
    gl::Fbo::Format format;
	
	gl::Fbo mFboScene;
	gl::Fbo mFboBlur1;
	gl::Fbo mFboBlur2;
	
	gl::GlslProg mShaderBlur;
    
    void setupBlur();
	void updateBlur();
    
    //COLORS
    ci::ColorA colorDifuse;
    ci::ColorA colorAmbient;
    ci::ColorA colorSpecular;
    ci::ColorA colorEmission;
    
    ci::ColorA modelAmbient;
    
    GLfloat* light_position;
    GLfloat* light_positionLeft;
    GLfloat* light_positionRight;
    
    GLfloat* shininess_mat;
    
    float attenuationBloom;
    
    float alphaBackground;
    float grayBackground;
    
    //TIMERS
    void timeToggle();
    void timeActivator();
    
    bool activateEvent;
    int  T0GGLE_MODE;
    int  eventTimePrev;
    
    //CAMERA
    float distanceCamera;
    float distanceCameraOriginal;
    float distanceCameraTmp;
    
    float zoom;
    float particleRadius;
    ci::Vec3f mCenter;
    ci::Vec3f mUp;
    
    float beatParticleThreshold;
    
    bool  activateMoons;
    
    //CUBE
    void                drawCube();
    void                contrasteChanger();
    ci::gl::Texture     mask;
    bool                activateMask;
    
    // CAMERA
	SpringCam			mSpringCam;
	
	// SHADERS
	gl::GlslProg		mRoomShader;
	gl::GlslProg		mSphereShader;
	
	// TEXTURES
	CubeMap				mCubeMap;
	
	// ROOM
	Room				mRoom;
	gl::Fbo				mRoomFbo;
    
	// MOUSE
	Vec2f				mMousePos, mMouseDownPos, mMouseOffset;
	bool				mMousePressed;

    void                drawIntoRoomFbo();
	void                drawSphere();
    
    //OSC
	void				oscSetup();
	void				oscUpdate();
	ci::osc::Listener   listener;
    float				mFreq;
	float				mFreqLog;
	float				mAmp;
	bool				mOnSet;
    
    float               scaleVal;
    
    //MIDI
	void				setupMIDI();
	void                midiListener(midi::Message msg);
	midi::Input         mInput;

};