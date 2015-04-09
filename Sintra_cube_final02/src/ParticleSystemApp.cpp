#include "ParticleSystemApp.h"
#include "timeToggle.h"
#include "Midi.h"
#include "Osc.h"

//-------------------SETUP-----------------------------------------------------
void ParticleSystemApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(200.f);
    settings->setBorderless();
 //   settings->disableFrameRate();
    settings->setWindowPos(0, 0);
    
}

void ParticleSystemApp::setup()
{
    gl::disableVerticalSync();
    glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
    
	glEnable(GL_POINT_SIZE);
    
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POINT_SMOOTH_HINT,  GL_NICEST );
    
    glEnable(GL_LINE_WIDTH);
    
   // glEnable( GL_LIGHTING );
	//glEnable( GL_DEPTH_TEST );
    glShadeModel(GL_SMOOTH);
    
    
    //MIDI
    setupMIDI();
    
    
    // CAMERA
	mSpringCam			= SpringCam( -466.0f, getWindowAspectRatio() );
	
	// LOAD SHADERS
	try {
		mRoomShader		= gl::GlslProg( loadResource(ROOM_VERT), loadResource( ROOM_FRAG ) );
		mSphereShader	= gl::GlslProg( loadResource(SPHERE_VERT), loadResource( SPHERE_FRAG) );
	} catch( gl::GlslProgCompileExc e ) {
		std::cout << e.what() << std::endl;
		quit();
	}
	
	// LOAD TEXTURES
	mCubeMap			= CubeMap( GLsizei(512), GLsizei(512),
								  Surface8u( loadImage( loadResource( RES_CUBE1_ID ) ) ),
								  Surface8u( loadImage( loadResource( RES_CUBE2_ID ) ) ),
								  Surface8u( loadImage( loadResource( RES_CUBE3_ID ) ) ),
								  Surface8u( loadImage( loadResource( RES_CUBE4_ID ) ) ),
								  Surface8u( loadImage( loadResource( RES_CUBE5_ID ) ) ),
								  Surface8u( loadImage( loadResource( RES_CUBE6_ID ) ) )
								  );
    
	// ROOM
	gl::Fbo::Format roomFormat;
	roomFormat.setColorInternalFormat( GL_RGB );
	int fboXRes			= APP_WIDTH/ROOM_FBO_RES;
	int fboYRes			= APP_HEIGHT/ROOM_FBO_RES;
	mRoomFbo			= gl::Fbo( fboXRes, fboYRes, roomFormat );
	bool isPowerOn		= false;
	bool isGravityOn	= true;
	mRoom				= Room( Vec3f( 574.0f, 326.0f, 1500.0f ), isPowerOn, isGravityOn );
	mRoom.init();
	
	// MOUSE
	mMousePos			= Vec2f::zero();
	mMouseDownPos		= Vec2f::zero();
	mMouseOffset		= Vec2f::zero();
	mMousePressed		= false;
    
    attrFactor          = 0.35f;
    forceIncrement      = 0.00014;
    
    mParticleSystem = new ParticleSystem();
    
    //cuando se vuelve loco
   // attrFactor      = 0.34f;
   // forceIncrement  =0.00094;
    
    randAngle             = 45.f;
    attrPositionCenter    = ci::Vec3f::zero();
    
    attrPositionFactor     =  (float)APP_WIDTH/6.0;
    attrPositionLeft       = ci::Vec3f(-attrPositionFactor, 0.0, 0.0);
    attrPositionRight      = ci::Vec3f(attrPositionFactor, 0.0, 0.0);
   
    repulsionRadius = 140.f;
    repulsionFactor = -12.f;
    
    //PARTICLES
    createNewParticles();
    
    //CAMERA
    mCenter			= Vec3f::zero();
    mUp				= Vec3f::yAxis();
    mCam.setPerspective(45.0f, getWindowAspectRatio(), 0.1, 10000);
    mCam.setEyePoint(ci::Vec3f(0.f, 0.f, 300.f));
    mCam.setCenterOfInterestPoint(ci::Vec3f::zero());
    
    //MASK
    mask = loadImage(loadResource(MASK));
    
    //AUDIO
    mAudio = new AudioAnalyzer();
   // beatDetector = new BeatDetector();
    
    //LIGHTS
    light_position = new GLfloat[4];
    light_position[3] = -1;
    
    light_positionLeft = new GLfloat[4];
    light_positionLeft[0] = 6;//attrPositionLeft.x;
    light_positionLeft[1] = -1;
    light_positionLeft[2] = 1;
    light_positionLeft[3] = 0;
    
    light_positionRight = new GLfloat[4];
    light_positionRight[0] = 1;//attrPositionRight.x;
    light_positionRight[1] = -1;
    light_positionRight[2] = 0;
    light_positionRight[3] = 0;
    
    shininess_mat = new GLfloat[1];
    shininess_mat[0] =50;
    
    colorDifuse   = ci::ColorA(0.9, 0.9, 0.9, 1.0);
    colorAmbient  = ci::ColorA(0.7, 0.5, 0.4, 1.0f);
    colorSpecular = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
    colorEmission = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
    
    mParams = params::InterfaceGl::create(getWindow(), "Params", toPixels(ci::Vec2i(350, 750)));
    mParams->addParam("FPS", &avgFps);
    mParams->addParam("Force Inc", &forceIncrement, "min=0.0000001 max =0.001 step = 0.00001");
    mParams->addParam("attraction Position", &attrPositionCenter,  "min=0 max =350 step=5");
    mParams->addParam("attrFactor", &attrFactor, "min=0.001 max=2 step =0.001");
    mParams->addSeparator();
    mParams->addParam("camOrientation", &camOrientation, "");
    mParams->addParam("CenterCam", &mCenter, "min=0.0 max=1000.0 step=10.0");
    mParams->addSeparator();
    
    mParams->addParam("colorDifuse", &colorDifuse, "");
    mParams->addParam("colorAmbient", &colorAmbient, "");
    mParams->addParam("colorSpecular", &colorSpecular, "");
    mParams->addParam("colorEmission", &colorEmission, "");
    mParams->addSeparator();
    mParams->addParam("modelAmbient", &modelAmbient, "");
    mParams->addParam("attenuationBloom", &attenuationBloom, "min=0 max= 1.5 step=0.1");
    mParams->addParam("alphaBackground", &alphaBackground, "min=0 max= 1.0 step=0.03");
    mParams->addParam("grayBackground", &grayBackground, "min=0 max= 1.0 step=0.03");
    
    mParams->addParam("particleRadius", &particleRadius, "min=50 max=150 step=3");
    mParams->addParam("distancaceCamera", &distanceCamera, "min=0 max=1200 step=3");
    mParams->addSeparator();
    mParams->addParam("LeftPos", &attrPositionLeft, "min=-400 max=400 step=1");
    mParams->addParam("RightPos", &attrPositionRight, "min=-400 max=400 step=1");
    
    mParams->addParam("scale Val", &scaleVal, "min=-0.0 max=30 step=0.01");
    
    setupBlur();
    
    scaleVal = 0.83;
    
    eventTimePrev = 0;
    
    rotationMax = 0;
    rotationInc = 1;
    
    attenuationBloom = 0.8f;
    alphaBackground  = 0.24;
    grayBackground   = 0.06;
    
    particleRadius      = 150;
    distanceCamera      = 600;
    distanceCameraTmp   = 350;
    
    activateBeatGrow      = true;
    
    beatParticleThreshold = 0.001f;
    
    colorConectionLeft = ci::ColorA(0.5, 0.5, 0.5, 0.4);
    colorConectionRight = ci::ColorA(0.5, 0.5, 0.5, 0.4);
    
    activateMask = false;
    activateMoons = false;
}

void ParticleSystemApp::resize()
{
    glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
    
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POINT_SMOOTH_HINT,  GL_NICEST );
    
    mCam.setPerspective(45.0f, getWindowAspectRatio(), 0.1, 10000);
}

//------------EVENTS--------------------------------------------------------------
void ParticleSystemApp::mouseDown( MouseEvent event )
{
    mMouseDownPos = event.getPos();
	mMousePressed = true;
	mMouseOffset = Vec2f::zero();
}

void ParticleSystemApp::mouseDrag( MouseEvent event)
{
    mouseMove( event );
	mMouseOffset = ( mMousePos - mMouseDownPos );
}

void ParticleSystemApp::mouseMove( MouseEvent event)
{
    mMousePos = event.getPos();
}

void ParticleSystemApp::mouseUp( MouseEvent event)
{
	mMousePressed = false;
	mMouseOffset = Vec2f::zero();
}

void ParticleSystemApp::keyDown(KeyEvent event)
{
    switch (event.getChar()) {
        case 'a':
            createNewParticles();
        break;
        case '1':
            light_position[3] =0;
            break;
        case '2':
            light_position[3] = -1;
            break;
        case '3':
            shininess_mat[0] = 10;
            break;
        case '4':
            activateMoons = !activateMoons;
            console()<<activateMoons<<std::endl;
            break;
        case '0':
            mAudio->stopTrack();
            mAudio->stopDrums();
            break;
        case '9':
            mAudio->playTrack();
            mAudio->playDrums();
            mAudio->setVolumeTrack(1.0);
            mAudio->setVolumeDrums(0.0);
            break;
        case '8':
            activateBeatGrow  =  !activateBeatGrow;
            console()<<activateBeatGrow<<endl;
            break;
        case 'z':
            activateMask = !activateMask;
            break;
    }
    
    if(event.getChar() == ' '){
        mRoom.togglePower();
    }
}

//------------UPDATE-----------------------------------------------------------------
void ParticleSystemApp::update()
{
    avgFps = getAverageFps();
    
    mAudio->beatDetectionLeft();
    mAudio->beatDetectionRight();
    mAudio->beatDetectionDrums();
    mAudio->drawFftLeftScene();
    mAudio->drawFftRightScene();
    
    if(activateMoons){
        if(activateBeatGrow){
           // particleBeatForce();
            particleBeatForceLeft();
            particleBeatForceRight();
        }else{
            //particleBeatForceGrow();
            particleBeatForceGrowLeft();
            particleBeatForceGrowRight();
        }
    }else{
        if(activateBeatGrow)
            particleBeatForce();
        else
            particleBeatForceGrow();
    }
    
    
    mParticleSystem->update();
    
    timeActivator();
    
	mRoom.update();
    
	// CAMERA
	if( mMousePressed ){
		mSpringCam.dragCam( ( mMouseOffset ) * 0.01f, ( mMouseOffset ).length() * 0.01 );
	}
	mSpringCam.update( 0.4f );
    
	// ROOM
	drawIntoRoomFbo();
    
    gl::VboMesh::VertexIter iter = mRoom.mVbo.mapVertexBuffer();
    
    for( int i = 0; i< 36; i++ ){
        iter.setPosition(mRoom.vertexs[i]);
        ++iter;
    }

}

//----------------AUDIO RELATIVE----------------------- TRUE
void ParticleSystemApp::particleBeatForce()
{
    int fftCount = 0;
    float beatValueLeft  = mAudio->getBeatValueLeft();
    float beatValueRight = mAudio->getBeatValueRight();
    float beatForLeft    = mAudio->getBeatForceLeft();
    float *levels        = mAudio->getLeftLevels();
    
    for(auto it = mParticleSystem->begin(); it != mParticleSystem->end(); ++it, ++fftCount){
        ci::Vec3f attrForce = attrPositionCenter - (*it)->position;
        attrForce *= attrFactor + beatValueLeft;
        if(attrPositionCenter.distance((*it)->position) < particleRadius){
            attrForce = (*it)->position - attrPositionCenter;
            attrForce *= beatValueLeft*10 + 0.2f;//0.2f
        }
        
         if(beatValueLeft > beatParticleThreshold){
             (*it)->forces += attrForce;
             ci::Vec3f beatForceVec = (*it)->position - attrPositionCenter;
             beatForceVec.normalize();
           //  if(beatValueLeft> 0.01)
                 beatForceVec.rotate(randVec3f(), randAngle);
             beatForceVec *= (beatValueLeft +beatValueRight + 0.01) * ci::randFloat(beatForLeft*2, beatForLeft*4.0); //0.01, 3
             (*it)->forces += beatForceVec + randVec3f()*((levels[ci::randInt(fftCount)])/3.5 );
        
             for(auto it2 = mParticleSystem->begin(); it2 != mParticleSystem->end(); ++it2){
                 (*it)->forces += ((*it)->position - (*it2)->position ) * forceIncrement * 0.5;
             }
        }
         fftCount = (fftCount +1)%BANDS;
    }
    randAngle = lmap<float>(beatValueLeft, 0.0, 0.2, 0, 360);
}

void ParticleSystemApp::particleBeatForceGrow()
{
    int fftCount = 0;
    float beatValueLeft  = mAudio->getBeatValueLeft();
    float beatValueRight = mAudio->getBeatValueRight();
    float beatForLeft    = mAudio->getBeatForceLeft();
    float *levels        = mAudio->getLeftLevels();
    
    for(auto it = mParticleSystem->begin(); it != mParticleSystem->end(); ++it, ++fftCount){
        ci::Vec3f attrForce = attrPositionCenter - (*it)->position;
        attrForce *= attrFactor;
        if(attrPositionCenter.distance((*it)->position) < particleRadius){
            attrForce = (*it)->position - attrPositionCenter;
            attrForce *= beatValueLeft + 0.2;//0.2f
        }
        
        (*it)->forces += attrForce;
        ci::Vec3f beatForceVec = (*it)->position - attrPositionCenter;
        beatForceVec.normalize();
        beatForceVec.rotate(randVec3f(), randAngle);
        beatForceVec *= (beatValueLeft +beatValueRight) * ci::randFloat(beatForLeft, beatForLeft*2.2); //0.01, 3
        (*it)->forces += beatForceVec + randVec3f()*((levels[fftCount])/3.5 );
        
        for(auto it2 = mParticleSystem->begin(); it2 != mParticleSystem->end(); ++it2){
            (*it)->forces += ((*it)->position - (*it2)->position ) * forceIncrement * 0.5;
        }
        fftCount = (fftCount +1)%BANDS;
    }
    randAngle = lmap<float>(beatValueLeft, 0.0, 0.2, 0, 360);
}


///----------BEAT FORCE GROW------------------------------------------------------
void ParticleSystemApp::particleBeatForceGrowLeft()
{
    int ip  = 0;
    int fftCount = 0;
    float beatValueLeft  = mAudio->getBeatValueLeft();
    float beatValueRight = mAudio->getBeatValueRight();
    float beatForceRight = mAudio->getBeatForceRight();
    float *levels        = mAudio->getLeftLevels();
    
    int numThreshold = int(NUMPARTICLE/2.0);
    
    //console()<<beatValueLeft<<" grow"<<endl;
    
    for(auto it = mParticleSystem->begin(); it != mParticleSystem->end(); ++it, ++ip, ++fftCount){
        if(ip < numThreshold){
            ci::Vec3f attrForce = attrPositionLeft - (*it)->position;
            attrForce *= attrFactor;
            if(attrPositionLeft.distance((*it)->position) < particleRadius){
                attrForce = (*it)->position - attrPositionLeft;
                attrForce *= beatValueLeft + 0.5;//0.2f
            }
            
            (*it)->forces += attrForce;
            ci::Vec3f beatForceVec = (*it)->position - attrPositionLeft;
            beatForceVec.normalize();
            beatForceVec.rotate(randVec3f(), randAngle);
            beatForceVec *= (beatValueLeft +beatValueRight) *(beatForceRight); //0.01, 3
            (*it)->forces += beatForceVec + randVec3f()*((levels[fftCount])/3.5 );
            
            int it2p =0;
            for(auto it2 = mParticleSystem->begin(); it2 != mParticleSystem->end(); ++it2, ++it2p){
                if(it2p < numThreshold)
                    (*it)->forces += ((*it)->position - (*it2)->position ) * forceIncrement * 0.5;
                else
                    break;
            }
        }else{
            return;
        }
         fftCount = (fftCount +1)%BANDS;
    }
    randAngle = lmap<float>(beatValueLeft, 0.0, 0.2, 0, 360);
}

void ParticleSystemApp::particleBeatForceGrowRight()
{
    int ip  = 0;
    int fftCount = 0;
    float beatValueLeft  = mAudio->getBeatValueLeft();
    float beatValueRight = mAudio->getBeatValueRight();
    float beatForLeft    = mAudio->getBeatForceLeft();
    float *levels        = mAudio->getRightLevels();
    
    
    int numThreshold = int(NUMPARTICLE/2.0);
    
    for(auto it = mParticleSystem->rbegin(); it != mParticleSystem->rend(); ++it, ++ip, ++fftCount){
        if(ip < numThreshold){
            ci::Vec3f attrForce = attrPositionRight - (*it)->position;
            attrForce *= attrFactor;
            if(attrPositionRight.distance((*it)->position) < particleRadius){
                attrForce = (*it)->position - attrPositionRight;
                attrForce *= beatValueLeft + 0.2;//0.2f
            }
            
            (*it)->forces += attrForce;
            ci::Vec3f beatForceVec = (*it)->position - attrPositionRight;
            beatForceVec.normalize();
            beatForceVec.rotate(randVec3f(), randAngle);
            beatForceVec *= (beatValueLeft +beatValueRight) * ci::randFloat(beatForLeft, beatForLeft*2.2); //0.01, 3
            (*it)->forces += beatForceVec + randVec3f()*((levels[fftCount])/3.5 );
            
            int it2p =0;
            for(auto it2 = mParticleSystem->rbegin(); it2 != mParticleSystem->rend(); ++it2, ++it2p){
                if(it2p < numThreshold)
                    (*it)->forces += ((*it)->position - (*it2)->position ) * forceIncrement * 0.5;
                else
                    break;
            }
        }else{
            return;
        }
         fftCount = (fftCount +1)%BANDS;
    }
    randAngle = lmap<float>(beatValueLeft, 0.0, 0.2, 0, 360);
}


///----------BEAT FORCE ------------------------------------------------------
void ParticleSystemApp::particleBeatForceLeft()
{
    int ip  = 0;
    int fftCount = 0;
    float beatValueLeft  = mAudio->getBeatValueLeft();
    float beatValueRight = mAudio->getBeatValueRight();
    float beatForce      = mAudio->getBeatForceLeft();
    float beatDrums      = mAudio->getBeatValueDrums();
    float *levels        = mAudio->getLeftLevels();
    
    int numThreshold = int(NUMPARTICLE/2.0);
    
   // console()<<beatValueLeft<<" force"<<endl;
    
    for(auto it = mParticleSystem->begin(); it != mParticleSystem->end(); ++it, ++ip, ++fftCount){
        
        if(ip < numThreshold){
            ci::Vec3f attrForce = attrPositionLeft - (*it)->position;
            attrForce *= attrFactor + beatValueLeft;
            if(attrPositionLeft.distance((*it)->position) < particleRadius){
                attrForce = (*it)->position - attrPositionLeft;
                attrForce *= beatValueLeft + 0.2f;//0.2f
            }
            
            if(beatValueLeft > beatParticleThreshold){
                (*it)->forces += attrForce;
                ci::Vec3f beatForceVec = (*it)->position - attrPositionLeft;
                beatForceVec.normalize();
                  if(beatValueLeft> 0.01)
                beatForceVec.rotate(randVec3f(), randAngle);
                beatForceVec *= (beatValueLeft +beatValueRight + 0.03) * (beatForce*(1.0 +beatDrums)); //0.01, 3
                (*it)->forces += beatForceVec + randVec3f()*((levels[fftCount])*0.4 );
                int ip2 =0;
                for(auto it2 = mParticleSystem->begin(); it2 != mParticleSystem->end(); ++it2, ++ip2){
                    if(ip2 < numThreshold)
                        (*it)->forces += ((*it)->position - (*it2)->position ) * forceIncrement * 0.5;
                    else
                        break;
                }
            }
        }else{
            return;
        }
        fftCount = (fftCount +1)%BANDS;
    }
    randAngle = lmap<float>(beatValueLeft, 0.0, 0.2, 0, 360);
}

void ParticleSystemApp::particleBeatForceRight()
{
    int ip =0;
    int fftCount =0;
    float beatValueLeft  = mAudio->getBeatValueLeft();
    float beatValueRight = mAudio->getBeatValueRight();
    float beatForce      = mAudio->getBeatForceRight();
    float beatDrums      = mAudio->getBeatValueDrums();
    float *levels        = mAudio->getRightLevels();
    
    int numThreshold = int(NUMPARTICLE/2.0);
    
    //reverse iterator
    for(auto it = mParticleSystem->rbegin(); it != mParticleSystem->rend(); ++it, ++ip, ++fftCount){
        if(ip < numThreshold){
            ci::Vec3f attrForce = attrPositionRight - (*it)->position;
            attrForce *= attrFactor + beatValueLeft;
            if(attrPositionRight.distance((*it)->position) < particleRadius){
                attrForce = (*it)->position - attrPositionRight;
                attrForce *= beatValueLeft + 0.2f;//0.2f
            }
            
            if(beatValueLeft > beatParticleThreshold){
                (*it)->forces += attrForce;
                ci::Vec3f beatForceVec = (*it)->position - attrPositionRight;
                beatForceVec.normalize();
                 if(beatValueLeft> 0.01)
                beatForceVec.rotate(randVec3f(), randAngle);
                beatForceVec *= (beatValueLeft +beatValueRight + 0.03) * (beatForce*(1.0 +beatDrums)); //0.01, 3
                (*it)->forces += beatForceVec + randVec3f()*((levels[fftCount])*0.4 );
                
                int itp2 =0;
                for(auto it2 = mParticleSystem->rbegin(); it2 != mParticleSystem->rend(); ++it2, ++itp2){
                    if(itp2 < numThreshold)
                        (*it)->forces += ((*it)->position - (*it2)->position ) * forceIncrement * 0.5;
                    else
                        break;
                }
            }
        }else{
            return;
        }
         fftCount = (fftCount +1)%BANDS;
    }
    randAngle = lmap<float>(beatValueLeft, 0.0, 0.2, 0, 360);
}


//------------DRAW------------------------------------------------------------------
void ParticleSystemApp::draw()
{
    gl::color(ci::ColorA(grayBackground, grayBackground, grayBackground, alphaBackground));
    gl::drawSolidRect(ci::Rectf(0, 0, APP_WIDTH, APP_HEIGHT));
    drawMain();
}

void ParticleSystemApp::drawMain()
{
    updateBlur();
    
	gl::pushModelView();
	gl::translate(Vec2f(0, getWindowHeight()));
	gl::scale(Vec3f(1, -1, 1));
	gl::color(Color::white());
	gl::draw(mFboScene.getTexture(), getWindowBounds());
	
	gl::enableAdditiveBlending();
	gl::draw(mFboBlur2.getTexture(), getWindowBounds());
	gl::disableAlphaBlending();
	gl::popModelView();

      /*
	gl::pushModelView();
    gl::pushMatrices();
    
    gl::clear( ColorA( 0, 0, 0, 0 ), true );
	gl::color( ColorA( 1, 1, 1, 1 ) );
    
	gl::setMatricesWindow( getWindowSize(), false );
	gl::setViewport( getWindowBounds() );
    
    gl::translate(APP_WIDTH/2.0, APP_HEIGHT/2.0, 0);
    gl::scale(scaleVal + 0.11, scaleVal, scaleVal);
	gl::disableDepthRead();
	gl::disableDepthWrite();
	gl::enableAlphaBlending();
	gl::enable( GL_TEXTURE_2D );
    

    
	// DRAW ROOM
  
	mRoomFbo.bindTexture();
    gl::translate(ci::Vec2f(-APP_WIDTH/2.0, -APP_HEIGHT/2.0));
	gl::drawSolidRect( getWindowBounds());
    mRoomFbo.unbindTexture();

    
   //mSpringCam.setEye(ci::Vec3f(APP_WIDTH/2.0, APP_WIDTH/2.0, -466));//distanceCamera));
    gl::setMatrices(mSpringCam.getCam());
    

    gl::enableDepthRead();
	gl::enableDepthWrite();
	
	drawSphere();
    
    gl::popModelView();
    gl::popMatrices();
     */
    
    if(activateMask){
        gl::disableDepthRead();
        gl::disableDepthWrite();
        gl::disable( GL_TEXTURE_2D );
        gl::pushModelView();
        gl::enableAlphaBlending();
        gl::setViewport( getWindowBounds() );
        gl::color(1.0, 1.0, 1.0, 0.4);
        gl::draw(mask);
        gl::disableAlphaBlending();
        gl::popModelView();
    }
    
    mParams->draw();
    
    // DRAW ROOM
	//mRoomFbo.bindTexture();
	//gl::draw( mRoomFbo.getTexture(), getWindowBounds() );
  //  mRoomFbo.unbindTexture();
}

void ParticleSystemApp::drawParticles()
{
    mParticleSystem->draw();
    mParticleSystem->drawConnections();
}

void ParticleSystemApp::drawShadedParticles()
{
    glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
    
    gl::enableAlphaBlending();
    
    gl::setViewport(getWindowBounds());
    //gl::color(ci::ColorA(grayBackground, alphaBackground));
    //gl::drawSolidRect(getWindowBounds());
    
    gl::pushModelView();
    
    ci::Vec3f mEye = Vec3f( 0.0f, 0.0f, distanceCamera );
    mCam.lookAt( mEye, mCenter, mUp );
    mCam.setOrientation(camOrientation);
    gl::setModelView(mCam);
    
    float beatDrums = mAudio->getBeatValueDrums();
    
    if( beatDrums > 0.01 ){
        attrPositionCenter += ci::Vec3f(ci::randFloat(-1, 1)*beatDrums*5000, ci::randFloat(-1, 1)*beatDrums*5500,ci::randFloat()*beatDrums*2500);
    }else{
        attrPositionCenter = ci::Vec3f(0, 0, 0);
    }
    
   // if(
    float beatLeft = mAudio->getBeatValueLeft();
    
    float colorEmisionBeat = beatLeft * 6.9;
    
    if(T0GGLE_MODE == INTRO || T0GGLE_MODE == INICIO  || T0GGLE_MODE == PUENTE_INICIO || T0GGLE_MODE == PUENTE_CLIMAX_INICIO)
        colorEmission    = ci::ColorA(1.0 - colorEmisionBeat, 1.0 - colorEmisionBeat, 1.0 - colorEmisionBeat, 1.0f);
    else{
        //colorEmission   +=
    }
    
    rotateScene();
    
    colorConectionLeft.a = beatLeft*5.0 +0.2;

    gl::lineWidth(1.0 + beatLeft*lineWidthFactor);
    gl::color(colorConectionLeft);
    mParticleSystem->drawConnections();
    //gl::lineWidth(1.0);
    

    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
	glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    
    //gl::color(ColorA(0.f, 0.f, 0.f, 1.f));
    glMaterialfv( GL_FRONT, GL_DIFFUSE,	colorDifuse );
    glMaterialfv( GL_FRONT, GL_AMBIENT,	colorAmbient  );
    
    glMaterialfv( GL_FRONT, GL_SPECULAR, colorSpecular );
    glMaterialfv( GL_FRONT, GL_SHININESS, shininess_mat );
    
    glMaterialfv( GL_FRONT, GL_EMISSION, colorEmission );
    mParticleSystem->draw();
    glDisable( GL_LIGHTING );
    glDisable( GL_LIGHT0 );
    
    // gl::color(1.0, 0, 0);
    //gl::drawSphere(attrPosition, 10);
    
    
    gl::popModelView();
    
    mAudio->drawFftLeftScene();

    glDisable(GL_CULL_FACE);
}

void ParticleSystemApp::drawShadedParticlesLeft()
{
    glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
    gl::pushModelView();
    gl::pushMatrices();
    
    float beatDrums = mAudio->getBeatValueDrums();
    
    if( beatDrums > 0.005 ){
        attrPositionLeft += ci::Vec3f(ci::randFloat(-0.5, 0.8)*beatDrums*4400, ci::randFloat(-0.5, 0.5)*beatDrums*5800,ci::randFloat()*beatDrums*2500);
    }else{
        attrPositionLeft = ci::Vec3f(-attrPositionFactor, 0, 0);
    }
    
    // if(
    float beatLeft = mAudio->getBeatValueLeft();
    
    float colorEmisionBeat = beatLeft * 6.5;
    
    if(T0GGLE_MODE == INTRO || T0GGLE_MODE == INICIO  || T0GGLE_MODE == PUENTE_INICIO || T0GGLE_MODE == PUENTE_CLIMAX_INICIO)
        colorEmission    = ci::ColorA(1.0 - colorEmisionBeat, 1.0 - colorEmisionBeat, 1.0 - colorEmisionBeat, 1.0f);
    else{
        //colorEmission   +=
    }
    
    colorConectionLeft.a = beatLeft*5.0 +0.1 + beatDrums*2;
    gl::lineWidth(1.0 + beatLeft*lineWidthFactor);
    gl::color(colorConectionLeft);
    mParticleSystem->drawConnectionsLeft();

    
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
	glLightfv( GL_LIGHT0, GL_POSITION, light_positionLeft );
    
    //gl::color(ColorA(0.f, 0.f, 0.f, 1.f));
    glMaterialfv( GL_FRONT, GL_DIFFUSE,	colorDifuse );
    glMaterialfv( GL_FRONT, GL_AMBIENT,	colorAmbient  );
    glMaterialfv( GL_FRONT, GL_SPECULAR, colorSpecular );
    glMaterialfv( GL_FRONT, GL_SHININESS, shininess_mat );
    glMaterialfv( GL_FRONT, GL_EMISSION, colorEmission );
    
    mParticleSystem->drawLeft();
    
    glDisable( GL_LIGHTING );
    glDisable( GL_LIGHT0 );
    glDisable(GL_CULL_FACE);
    glDisable(GL_COLOR_MATERIAL);
    
    gl::translate(attrPositionLeft);
    rotateScene();
    
    gl::popModelView();
    gl::popMatrices();

    gl::lineWidth(1.0);
    

}

void ParticleSystemApp::drawShadedParticlesRight()
{
    glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
    
    gl::pushModelView();
    gl::pushMatrices();
    
    float beatDrums = mAudio->getBeatValueDrums();
    
    if( beatDrums > 0.005 ){
        attrPositionRight += ci::Vec3f(ci::randFloat(-0.8, 0.6)*beatDrums*4500, ci::randFloat(-0.6, 0.6)*beatDrums*5800,ci::randFloat()*beatDrums*2500);
    }else{
        attrPositionRight= ci::Vec3f(attrPositionFactor, 0, 0);
    }
    
    // if(
    float beatRight = mAudio->getBeatValueRight();
    
    float colorEmisionBeat = beatRight * 9.5;
    
    if(T0GGLE_MODE == INTRO || T0GGLE_MODE == INICIO  || T0GGLE_MODE == PUENTE_INICIO || T0GGLE_MODE == PUENTE_CLIMAX_INICIO)
        colorEmission    = ci::ColorA(1.0 - colorEmisionBeat, 1.0 - colorEmisionBeat, 1.0 - colorEmisionBeat, 1.0f);
    else{
        //colorEmission   +=
    }
    
    colorConectionLeft.a = beatRight*5.0 +0.1 + beatDrums*2;
    gl::lineWidth(1.0 + beatRight * lineWidthFactor);
    gl::color(colorConectionRight);
    mParticleSystem->drawConnectionsRight();
    
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
	glLightfv( GL_LIGHT0, GL_POSITION, light_positionRight );
    
    //gl::color(ColorA(0.f, 0.f, 0.f, 1.f));
    glMaterialfv( GL_FRONT, GL_DIFFUSE,	colorDifuse );
    glMaterialfv( GL_FRONT, GL_AMBIENT,	colorAmbient  );
    glMaterialfv( GL_FRONT, GL_SPECULAR, colorSpecular );
    glMaterialfv( GL_FRONT, GL_SHININESS, shininess_mat );
    glMaterialfv( GL_FRONT, GL_EMISSION, colorEmission );
    
    mParticleSystem->drawRight();

    glDisable( GL_LIGHTING );
    glDisable( GL_LIGHT0 );
    glDisable(GL_CULL_FACE);
    glDisable(GL_COLOR_MATERIAL);
    
    gl::translate(attrPositionRight);
    rotateScene();
    
    gl::popModelView();
    gl::popMatrices();
  
    gl::lineWidth(1.0);

}

void ParticleSystemApp::contrasteChanger()
{
    
}

//--------DRAW INTO FBO---------------------------------------------
void ParticleSystemApp::drawIntoFboScene()
{
    mFboScene.bindFramebuffer();

    gl::pushModelView();
    gl::pushMatrices();
    
    gl::clear( ColorA( 0, 0, 0, 0 ), true );
	gl::color( ColorA( 1, 1, 1, 1 ) );
    gl::clear(ColorA(grayBackground, grayBackground, grayBackground, 1.0));
    
	gl::setMatricesWindow( getWindowSize(), false );
	gl::setViewport( getWindowBounds() );
    
    gl::translate(APP_WIDTH/2.0, APP_HEIGHT/2.0, 0);
    gl::scale(scaleVal + 0.11, scaleVal, scaleVal);
	gl::disableDepthRead();
	gl::disableDepthWrite();
	gl::enableAlphaBlending();
	gl::enable( GL_TEXTURE_2D );
    
	// DRAW ROOM
    
	mRoomFbo.bindTexture();
    gl::translate(ci::Vec2f(-APP_WIDTH/2.0, -APP_HEIGHT/2.0));
	gl::drawSolidRect( getWindowBounds());
    mRoomFbo.unbindTexture();
    
    //mSpringCam.setEye(ci::Vec3f(APP_WIDTH/2.0, APP_WIDTH/2.0, -466));//distanceCamera));
    gl::setMatrices(mSpringCam.getCam());
    
    //gl::enableDepthRead();
	//gl::enableDepthWrite();
	
	drawSphere();
    
    //gl::disableDepthRead();
	//gl::disableDepthWrite();
    
    gl::popModelView();
    gl::popMatrices();
    
    
    /*gl::pushModelView();
    gl::pushMatrices();
    
    gl::enableAlphaBlending();
	gl::color(ci::ColorA(grayBackground, grayBackground, grayBackground, alphaBackground));
    gl::drawSolidRect(ci::Rectf(0, 0, APP_WIDTH, APP_HEIGHT));
    
	gl::setViewport(mFboScene.getBounds());
    
    ci::Vec3f mEye = Vec3f( 0.0f, 0.0f, distanceCamera );
    mCam.lookAt( mEye, mCenter, mUp );
    mCam.setOrientation(camOrientation);
    gl::setModelView(mCam);
    
    drawShadedParticlesLeft();
    drawShadedParticlesRight();
    
    gl::popMatrices();
    gl::popModelView();
     */
	mFboScene.unbindFramebuffer();
}

void ParticleSystemApp::rotateScene()
{
    float beatValueLeft = mAudio->getBeatValueLeft();
    if( beatValueLeft > 0.075){
        if( rotationMax < rotationInc){
            rotation    =  beatValueLeft / 30.0;
            rotationMax =  beatValueLeft * 6.5;
            rotationInc = 0;
        }
    }
    
    float r = getElapsedSeconds() * (0.8f + rotationInc);
    gl::rotate(ci::Vec3f::one() * r); //*r
    
    if( rotationMax > rotationInc)
        rotationInc+=rotation;
}

//--------DRAW CUBE---------------------------------------------
void ParticleSystemApp::drawCube()
{
	mRoomFbo.bindFramebuffer();
	gl::clear( ColorA( 0.0f, 0.0f, 0.0f, 0.0f ), true );
	
	gl::setMatricesWindow( mRoomFbo.getSize(), false );
	gl::setViewport( mRoomFbo.getBounds() );
	gl::disableAlphaBlending();
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	Matrix44f m;
	m.setToIdentity();
	m.scale( mRoom.getDims() );
    
    Matrix44f cameraProjection;
    ci::Vec3f cameraEyeView;
	
    gl::color(1.0, 1.0, 1.0, 0.0);
	mRoom.draw();
	
	mRoomFbo.unbindFramebuffer();
	glDisable( GL_CULL_FACE );
}


///----------BLUR-----------------------------------
void ParticleSystemApp::setupBlur()
{
	gl::Fbo::Format format;
	format.enableMipmapping(false);
	format.setWrap(GL_CLAMP_TO_BORDER_ARB, GL_CLAMP_TO_BORDER_ARB);
	
	mFboBlur1 = gl::Fbo(APP_WIDTH/4.0f, APP_HEIGHT/4.0f, format);
	mFboBlur2 = gl::Fbo(APP_WIDTH/4.0f, APP_HEIGHT/4.0f, format);
    
	format.setCoverageSamples(16);
	format.setSamples(4);
    
	mFboScene = gl::Fbo(APP_WIDTH/1.0f, APP_HEIGHT/1.0f, format);
	
	try {
		mShaderBlur = gl::GlslProg( loadResource( RES_VERT_GLSL ), loadResource( RES_FRAG_GLSL ) );
	} catch (...) {
		console() << "Can't load/compile blur shader" << endl;
		quit();
	}
    
}

void ParticleSystemApp::updateBlur()
{
    //drawShadedParticles();
   // drawParticles();
	Area  viewport = gl::getViewport();
    
	drawIntoFboScene();
    
	mShaderBlur.bind();
    mShaderBlur.uniform("tex0", 0);
    mShaderBlur.uniform("sample_offset", Vec2f(1.0f / mFboBlur1.getWidth(), 0.0f));
	mShaderBlur.uniform("attenuation", attenuationBloom);
    
    mFboBlur1.bindFramebuffer();
    mFboScene.bindTexture(0);
    gl::setViewport(mFboBlur1.getBounds());
    gl::clear(Color::black());
    gl::drawSolidRect(getWindowBounds());
    mFboScene.unbindTexture();
    mFboBlur1.unbindFramebuffer();
	
    mShaderBlur.uniform("sample_offset", Vec2f(0.0f, 1.0f / mFboBlur2.getHeight()));
	mShaderBlur.uniform("attenuation", attenuationBloom);
    
    mFboBlur2.bindFramebuffer();
    mFboBlur1.bindTexture(0);
    gl::setViewport(mFboBlur2.getBounds());
    gl::clear(Color::black());
    gl::drawSolidRect(getWindowBounds());
    mFboBlur1.unbindTexture();
    mFboBlur2.unbindFramebuffer();
	
    mShaderBlur.unbind();
	
	gl::setViewport(viewport);
}

void ParticleSystemApp::createNewParticles()
{
    mParticleSystem->clear();
    int particleNUMThird = (NUMPARTICLE*2)/3.0;
    for(int i = 0; i < NUMPARTICLE; i++){
        float x = ci::randFloat(-getWindowWidth()/2, getWindowWidth()/2);
        float y = ci::randFloat(-getWindowHeight()/2, getWindowHeight()/2);
        float z = ci::randFloat(-200, 200);
        float radius = ci::randFloat(1.8f, 8.0f);
        float mass   = radius*10;//*8.0;
        if(i > particleNUMThird){
            radius = 2.8f;
            mass   = 20.0f;
        }
        float drag   = 0.94f;
        Particle * particle = new Particle(ci::Vec3f(x, y, z), radius, mass, drag);
        mParticleSystem->addParticle(particle);
    }
}


ci::Vec3f ParticleSystemApp::sphericalToCartesians(float radius, float theta, float phi)
{
    float x = radius * sinf( theta ) * cosf( phi );
    float y = radius * sinf( theta ) * sinf( phi );
    float z = radius * cosf( theta );
    return ci::Vec3f(x, y, z);
}

void ParticleSystemApp::drawIntoRoomFbo()
{
	mRoomFbo.bindFramebuffer();
//	gl::clear( ColorA( 0.0f, 0.0f, 0.0f, 0.0f ), true );
	
	gl::setMatricesWindow( mRoomFbo.getSize(), false );
	gl::setViewport( mRoomFbo.getBounds() );
	gl::disableAlphaBlending();
	gl::enable( GL_TEXTURE_2D );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	Matrix44f m;
	m.setToIdentity();
	m.scale( mRoom.getDims() );
    
    Matrix44f cameraProjection;
    ci::Vec3f cameraEyeView;

    cameraProjection = mSpringCam.mMvpMatrix;
    cameraEyeView    = mSpringCam.mEye;
	
	//	mLightsTex.bind( 0 );
	mRoomShader.bind();
    mRoomShader.uniform("mvpMatrix", cameraProjection);
	mRoomShader.uniform( "mMatrix", m );
	mRoomShader.uniform( "eyePos", cameraEyeView);
	mRoomShader.uniform( "roomDims", mRoom.getDims() );
	mRoomShader.uniform( "mainPower", mRoom.getPower() );
	mRoomShader.uniform( "lightPower", mRoom.getLightPower() );
	mRoom.draw();
	mRoomShader.unbind();
	
	mRoomFbo.unbindFramebuffer();
	glDisable( GL_CULL_FACE );
}

void ParticleSystemApp::drawSphere()
{
    
	//gl::color( ColorA( 1, 1, 1, 1 ) );
	gl::disable( GL_TEXTURE_2D );
   /*
	mCubeMap.bind();
	mSphereShader.bind();
	mSphereShader.uniform( "cubeMap", 0 );
	mSphereShader.uniform( "radius", 80.0f );
    
	mSphereShader.uniform( "mvpMatrix", cameraProjection );
	mSphereShader.uniform( "eyePos", cameraEyeView);
	mSphereShader.uniform( "power", mRoom.getPower() );
	mSphereShader.uniform( "roomDim", mRoom.getDims() );
    */
	//mSphereShader.unbind();
    
    
    gl::pushModelView();
    gl::pushMatrices();
    
	gl::setViewport(mFboScene.getBounds());
    
    ci::Vec3f mEye = Vec3f( 0.0f, 0.0f, distanceCamera );
    mCam.lookAt( mEye, mCenter, mUp );
    mCam.setOrientation(camOrientation);
    gl::setModelView(mCam);

    
    
    gl::enableDepthRead();
	gl::enableDepthWrite();
    
    drawShadedParticlesLeft();
    drawShadedParticlesRight();
    
    gl::disableDepthRead();
	gl::disableDepthWrite();
    
    gl::popMatrices();
    gl::popModelView();
}

CINDER_APP_NATIVE( ParticleSystemApp, RendererGl  )
