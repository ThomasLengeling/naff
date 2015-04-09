
enum{
    INTRO,
    INICIO,
    PUENTE_INICIO,
    INICIO_REGRESA,
    INTRO_PUENTE,
    PUENTE,
    PUENTE_CLIMAX_INICIO,
    CLIMAX_INICIO,
    CLIMAX_MEDIO,
    CLIMAX_INTENSO,
    VOZ,
    INESPERADO,
    INTERUPCION,
    CONTRASTE,
    PUENTE_FINAL,
    TEMA_NUEVO,
    CLIMAX_FINAL
};

void ParticleSystemApp::timeToggle()
{
    if(activateEvent){
        switch (T0GGLE_MODE) {
            case INTRO:
                colorDifuse       = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
                colorAmbient      = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
                colorSpecular     = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
                colorEmission     = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
                
                activateBeatGrow =  true;
                light_position[3] = 0;
                attenuationBloom  = 0.5f;
                alphaBackground   = 0.26;
                grayBackground    = 0.00;
                attrFactor        = 0.001f;
                forceIncrement    = 0.00095;
                
                lineWidthFactor   = 20;
                break;
            case INICIO:
                colorDifuse       = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
                colorAmbient      = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
                colorSpecular     = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
                colorEmission     = ci::ColorA(1.0, 1.0, 1.0, 1.0f);
                attrFactor        = 0.27;
              //  particleRadius    = 140;
                distanceCamera    = 430;
                
                light_position[3] = 0.0;
                attenuationBloom  = 0.5f;
                alphaBackground   = 0.36;
                grayBackground    = 0.00;
                lineWidthFactor   = 20;
                break;
            case PUENTE_INICIO:
                beatParticleThreshold = 0.001;
                light_position[3] = 0.0;
                attenuationBloom  = 1.0f;
                alphaBackground   = 0.48;
                grayBackground    = 0.00;
                forceIncrement    = 0.00050;
                break;
            case INTRO_PUENTE:
                 attrFactor        = 0.001;
                break;
                
            case PUENTE_CLIMAX_INICIO:
                attrFactor        = 0.45;
                colorConectionLeft = ci::ColorA(0.5, 0.5, 0.5, 0.7);
                break;
                
            case CLIMAX_INICIO:
                lineWidthFactor   = 50;
                colorDifuse   = ci::ColorA(0.9, 0.9, 0.9, 1.0f);
                colorAmbient  = ci::ColorA(0.7, 0.5, 0.4, 1.0f);
                colorSpecular = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                colorEmission = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                
                beatParticleThreshold = 0.001;
                activateBeatGrow  =  false;
                particleRadius    = 150;
                distanceCamera    =  550;
                light_position[3] = -1.0;
                attenuationBloom  = 0.65f;
                alphaBackground   = 0.48;
                grayBackground    = 0.0;
                distanceCamera    = 600;
                
                break;
            case CLIMAX_MEDIO:
                 lineWidthFactor   = 55;
                colorEmission = ci::ColorA(0.0, 0.0, 0.0, 1.0f);

                activateBeatGrow  =  true;
                particleRadius    = 150;
                distanceCamera    = 550;
                light_position[3] = -1.0;
                attenuationBloom  = 0.9f;
                alphaBackground   = 0.38;
                grayBackground    = 0.00;
                break;
            case CLIMAX_INTENSO:
                lineWidthFactor    = 66;
                light_position[3] = -1.0;
                attenuationBloom  = 1.5f;
                attrFactor        = 0.45f;
                alphaBackground   = 0.28;
                forceIncrement    = 0.00002;
                break;
            case INICIO_REGRESA:
                lineWidthFactor = 30;
                light_position[3] = -1.0;
                beatParticleThreshold = 0.003;
                activateBeatGrow  =  false;
                attrFactor        = 0.001f;
                forceIncrement    = 0.00002;
                break;
            case VOZ:
                lineWidthFactor = 60;
                colorDifuse   = ci::ColorA(0.9, 0.9, 0.9, 1.0);
                colorAmbient  = ci::ColorA(0.7, 0.5, 0.4, 1.0f);
                colorSpecular = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                colorEmission = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                
                beatParticleThreshold = 0.005;
                distanceCamera        = 320;
                light_position[3]     = -1.0;
                attenuationBloom  = 0.7f;
                alphaBackground   = 0.35;
                grayBackground    = 0.0;
                
                attrFactor        = 0.35f;
                forceIncrement    = 0.00002;
                
                createNewParticles();
                break;
            case INESPERADO:
                lineWidthFactor = 45;
                activateBeatGrow  =  true;
                attrFactor        = 0.31f;
                forceIncrement    = 0.00019;
                 distanceCamera  = 520;
                light_position[3] = -1.0;
                attenuationBloom  = 0.7f;
                alphaBackground   = 0.31;
                grayBackground    = 0.0;
                break;
            case INTERUPCION:
                lineWidthFactor = 60;
                light_position[3] = -1.0;
                attenuationBloom  = 0.7f;
                alphaBackground   = 0.21;
                grayBackground    = 0.0;
                break;
            case CONTRASTE:
                lineWidthFactor = 60;
                colorDifuse       = ci::ColorA(0.0, 0.0, 0.0, 1.0);
                colorAmbient      = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                colorSpecular     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                colorEmission     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                colorConectionLeft    = ci::ColorA(0.5, 0.5, 0.5, 0.6);
                light_position[3] = -1.0;
                attenuationBloom  = 0.2f;
                alphaBackground   = 0.54;
                grayBackground    = 1.0;
                break;
            case PUENTE_FINAL:
                lineWidthFactor = 30;
                beatParticleThreshold = 0.02;
                colorDifuse       = ci::ColorA(0.0, 0.0, 0.0, 1.0);
                colorAmbient      = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                colorSpecular     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                colorEmission     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                colorConectionLeft    = ci::ColorA(0.4, 0.4, 0.4, 0.6);
                light_position[3] = -1.0;
                attenuationBloom  = 0.2f;
                alphaBackground   = 0.40;
                grayBackground    = 1.0;
                break;
                
            case TEMA_NUEVO:
                lineWidthFactor = 60;
                //  beatParticleThreshold = 0.003;
                colorDifuse       = ci::ColorA(0.9, 0.9, 0.9, 1.0);
                colorAmbient      = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                colorSpecular     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                colorEmission     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                
                colorConectionLeft    = ci::ColorA(0.3, 0.3, 0.3, 0.6);
                
                light_position[3] = -1.0;
                attenuationBloom  = 0.0f;
                alphaBackground   = 0.1;
                grayBackground    = 1.0;
                break;
            case CLIMAX_FINAL:
                lineWidthFactor = 70;
                colorDifuse       = ci::ColorA(0.9, 0.9, 0.9, 1.0);
                colorAmbient      = ci::ColorA(0.7, 0.5, 0.4, 1.0f);
                colorSpecular     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                colorEmission     = ci::ColorA(0.0, 0.0, 0.0, 1.0f);
                
                colorConectionLeft    = ci::ColorA(0.5, 0.5, 0.5, 0.8);
                
                light_position[3] = -1.0;
                attenuationBloom  = 1.5f;
                alphaBackground   = 1.0;
                grayBackground    = 0.00;
                distanceCamera    = 450;
                break;
            default:
                break;
        }
        
        activateEvent = false;
    }
    
    if(T0GGLE_MODE == INICIO || INICIO_REGRESA == INICIO){
        
    }
    
}


void ParticleSystemApp::timeActivator()
{
    int timeTrack = (int)mAudio->getTime();
    
  //  console()<<mAudio->beatValueLeft<<" left"<<std::endl;
    
    if(timeTrack == 1){
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"INICIO 1 ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = INTRO;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 46){
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"INTRO 2 ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = INICIO;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 95){
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"PUENTE ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = PUENTE_INICIO;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 131){
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"INTRO 2 ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE     = INTRO_PUENTE;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 155){
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"PUENTE CLIMAX INICIO"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE     = PUENTE_CLIMAX_INICIO;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 179){
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"CLIMAX INICIO ACTIVATOR"<<std::endl;
            T0GGLE_MODE = true;
            T0GGLE_MODE = CLIMAX_INICIO;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 196){  //CLIMAX
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"CLIMAX MEDIO ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = CLIMAX_MEDIO;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 226){  //CLIMAX
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"CLIMAX ALTO ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = CLIMAX_INTENSO;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 267){ //REGRESA
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"REGRESA INICIO 1 ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = INICIO_REGRESA;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 312){ //PUNTO CLAVE
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"VOZ ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = VOZ;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 315){ //inesperado
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"INESPERADO ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = INESPERADO;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 380){ //interrumpcion
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"INTERRUPCION ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = INTERUPCION;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 383){
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"CONSTRASTANTE ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = CONTRASTE;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 398){
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"PUENTE ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = PUENTE_FINAL;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 422){
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"TEMA NUEVO ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = TEMA_NUEVO;
            eventTimePrev = timeTrack;
        }
    }
    
    if(timeTrack == 467){
        if(timeTrack != eventTimePrev){
            ci::app::console()<<"CLIMAX ACTIVATOR"<<std::endl;
            activateEvent = true;
            T0GGLE_MODE   = CLIMAX_FINAL;
            eventTimePrev = timeTrack;
        }
    }

    timeToggle();
}