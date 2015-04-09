#pragma once

#include "cinder/audio/Io.h"
#include "cinder/audio/Output.h"
#include "cinder/audio/FftProcessor.h"
#include "cinder/app/AppNative.h"

#include "Resources.h"


using namespace ci;
using namespace ci::app;

//#define DRAWFFT

#define BANDS 8

class AudioAnalyzer{
    
private:
    void     drawFftRight();
    void     drawFftLeft();
    
public:
    
    AudioAnalyzer();
    
    void     updateLevelsLeft();
    void     beatDetectionLeft();
    
    void     updateLevelsRight();
    void     beatDetectionRight();
    
    void     beatDetectionDrums();
    
    void     drawFftLeftScene();
    void     drawFftRightScene();
    
    float  &  getBeatValueLeft(){return beatValueLeft;}
    float  &  getBeatValueRight(){return beatValueRight;}
    float  &  getBeatValueDrums(){return beatValueDrums;}
    float  &  getBeatValueDrumsPrev(){return beatValueDrumsPrev;}
    
    float  &  getBeatForceLeft(){return beatForceLeft;}
    float  &  getBeatForceRight(){return beatForceRight;}

    void      stopTrack(){mTrack->stop();}
    void      playTrack(){mTrack->play();}
    void      setVolumeTrack(float val){mTrack->setVolume(val);}
    
    float      getTime(){return mTrack->getTime();}
    
    void      stopDrums(){mTrackDrums->stop();}
    void      playDrums(){mTrackDrums->play();}
    void      setVolumeDrums(float val){mTrackDrums->setVolume(val);}
    
    float *   getLeftLevels(){return levelsLeft;}
    float *   getRightLevels(){return levelsRight;}
    
private:
    
    //AUDIO
    audio::TrackRef         mTrack;
    audio::PcmBuffer32fRef  mPcmBuffer;
    
    audio::TrackRef         mTrackDrums;
    audio::PcmBuffer32fRef  mPcmBufferDrums;
    
    float                   beatForceDrums;
    float                   avgLvlOldDrums;
    float                   beatValueDrums;
    float                   beatValueDrumsPrev;
    float                   beatSensitivityDrums;
    
    float                   beatSensitivity;

    uint16_t                bandCount;
    
    float                   levelsLeft[8];
    float                   levelsPtsLeft[8];
    float                   beatForceLeft;
    float                   avgLvlOldLeft;
    float                   beatValueLeft;
    
    float                   levelsRight[8];
    float                   levelsPtsRight[8];
    float                   beatForceRight;
    float                   avgLvlOldRight;
    float                   beatValueRight;
    
};