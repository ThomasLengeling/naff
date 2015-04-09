//
//  AudioAnaylzer.cpp
//  ParticleSystemBloom
//
//  Created by tom on 4/19/14.
//
//

#include "AudioAnaylzer.h"



AudioAnalyzer::AudioAnalyzer()
{
    beatForceRight       = 250.f;
    beatForceLeft        = 250.f;
    
    beatValueDrums       = 0.0;
    
    beatSensitivity      = 0.003f;
    beatSensitivityDrums = 0.03;
    
    avgLvlOldRight       = 0.f;
    avgLvlOldLeft        = 0.f;
    
    beatValueLeft        = 0.0f;
    beatValueRight       = 0.0f;
    
    //AUDIO
    try{ //satie.wav Anxiety.mp3
        //satie_01.mp3
        int startTime  = 160;//110;
        mTrack = audio::Output::addTrack(audio::load(loadResource(SATIE)));
        mTrack->setTime(startTime); //156
        mTrack->enablePcmBuffering(true);
        mTrack->stop();
        
        mTrackDrums = audio::Output::addTrack(audio::load(loadResource(DRUMS)));
        mTrackDrums->setTime(startTime);
        mTrackDrums->enablePcmBuffering(true);
        mTrackDrums->stop();
    }catch( ... ){
        console()<<"error loding sound"<<std::endl;
    }
    
    bandCount = 8;
    std::fill(boost::begin(levelsRight), boost::end(levelsRight), 0.f);
    std::fill(boost::begin(levelsPtsRight), boost::end(levelsPtsRight), 0.f);
    
    std::fill(boost::begin(levelsLeft), boost::end(levelsLeft), 0.f);
    std::fill(boost::begin(levelsPtsLeft), boost::end(levelsPtsLeft), 0.f);
    
}




void AudioAnalyzer::drawFftLeft()
{
    
    if( !mPcmBuffer ) return;
    std::shared_ptr<float> fftRef = audio::calculateFft(mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT),bandCount );
    
    if( !fftRef ) {
        return;
    }
    float *fftBuffer = fftRef.get();
    
    #ifdef DRAWFFT
    float centerMargin= 15.0f;
    gl::color( Color::black() );
    gl::drawSolidCircle(Vec2f::zero(), 5.f);
    glLineWidth(3.f);
    #endif
    
    float avgLvlLeft= 0.f;
    
    for( int i= 0; i < bandCount; i++ ) {
        Vec2f p = Vec2f(0.f, 75.f);
        p.rotate( 2.f * M_PI * (i / (float)bandCount) );
        float lvl = fftBuffer[i] / bandCount * p.length();
        lvl = fmin(lvl, p.length());
        
        levelsLeft[i] = fmax(levelsLeft[i], lvl);
        levelsPtsLeft[i] = fmax(levelsPtsLeft[i], levelsLeft[i]);
        
        #ifdef DRAWFFT
        p.limit(1.f + centerMargin + levelsLeft[i]);
        gl::drawLine(p.limited(centerMargin), p);
        glPointSize(2.f);
        glBegin(GL_POINTS);
        gl::vertex(p + p.normalized()*levelsPtsLeft[i]);
        glEnd();
        glPointSize(1.f);
        #endif
        avgLvlLeft += lvl;
    }
    avgLvlLeft /= (float)bandCount;
    
    #ifdef DRAWFFT
    glLineWidth(1.f);
    gl::color( ColorA(0.f,0.f,0.f, 0.1f) );
    gl::drawSolidCircle(Vec2f::zero(), 5.f + avgLvlLeft);
    #endif
}

void AudioAnalyzer::drawFftRight()
{
    if( !mPcmBuffer ) return;
    std::shared_ptr<float> fftRef = audio::calculateFft(mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_RIGHT),bandCount );
    
    if( !fftRef ) {
        return;
    }
    float *fftBuffer = fftRef.get();
    
    #ifdef DRAWFFT
    float centerMargin = 15.0f;
    gl::color( Color::black() );
    gl::drawSolidCircle(Vec2f::zero(), 5.f);
    glLineWidth(3.f);
    #endif
    
    float avgLvlRight= 0.f;
    
    for( int i= 0; i < bandCount; i++ ) {
        Vec2f p = Vec2f(0.f, 75.f);
        p.rotate( 2.f * M_PI * (i / (float)bandCount) );
        
        float lvl = fftBuffer[i] / bandCount * p.length();
        lvl = fmin(lvl, p.length());
        
        levelsRight[i]    = fmax(levelsRight[i], lvl);
        levelsPtsRight[i] = fmax(levelsPtsRight[i], levelsRight[i]);
        
        #ifdef DRAWFFT
        p.limit(1.f + centerMargin + levelsRight[i]);
        gl::drawLine(p.limited(centerMargin), p);
        glPointSize(2.f);
        glBegin(GL_POINTS);
        gl::vertex(p + p.normalized() * levelsPtsRight[i]);
        glEnd();
        glPointSize(1.f);
        #endif
        
        avgLvlRight += lvl;
    }
    
    avgLvlRight /= (float)bandCount;
    
    #ifdef DRAWFFT
    glLineWidth(1.f);
    gl::color( ColorA(0.f,0.f,0.f, 0.1f) );
    gl::drawSolidCircle(Vec2f::zero(), 5.f+avgLvlRight);
    #endif
}


void AudioAnalyzer::beatDetectionLeft()
{
    beatValueLeft = 0.0f;
    mPcmBuffer = mTrack->getPcmBuffer();
    if(mPcmBuffer){
        std::shared_ptr<float> fftRef = audio::calculateFft(mPcmBuffer->getChannelData(audio::CHANNEL_FRONT_LEFT), bandCount);
        if(fftRef){
            float * fftBuffer = fftRef.get();
            float avgLv1Left = 0.0f;
            for(int i = 0; i < bandCount; i++){
                avgLv1Left += fftBuffer[i] / (float)bandCount;
            }
            avgLv1Left /= (float)bandCount;
            if(avgLv1Left > avgLvlOldLeft + beatSensitivity){
                beatValueLeft = avgLv1Left - beatSensitivity;
            }
            avgLvlOldLeft = avgLv1Left;
            
            //DETECT BEAT
            //update(fftBuffer);
        }
    }
    updateLevelsLeft();
}

void AudioAnalyzer::beatDetectionRight()
{
    beatValueRight = 0.0f;
    mPcmBuffer = mTrack->getPcmBuffer();
    if(mPcmBuffer){
        std::shared_ptr<float> fftRef = audio::calculateFft(mPcmBuffer->getChannelData(audio::CHANNEL_FRONT_RIGHT), bandCount);
        if(fftRef){
            float * fftBuffer = fftRef.get();
            float avgLv1Right = 0.0f;
            for(int i = 0; i < bandCount; i++){
                avgLv1Right += fftBuffer[i] / (float)bandCount;
            }
            avgLv1Right /= (float)bandCount;
            if(avgLv1Right > avgLvlOldRight + beatSensitivity){
                beatValueRight = avgLv1Right - beatSensitivity;
            }
            avgLvlOldRight = avgLv1Right;
        }
    }
    updateLevelsRight();
}

void AudioAnalyzer::beatDetectionDrums()
{
    beatValueDrumsPrev = beatValueDrums;
    beatValueDrums = 0.0f;
    mPcmBufferDrums = mTrackDrums->getPcmBuffer();
    if(mPcmBufferDrums){
        std::shared_ptr<float> fftRef = audio::calculateFft(mPcmBufferDrums->getChannelData(audio::CHANNEL_FRONT_LEFT), bandCount);
        if(fftRef){
            float * fftBuffer = fftRef.get();
            float avgLv1Drums = 0.0f;
            for(int i = 0; i < bandCount; i++){
                avgLv1Drums += fftBuffer[i] / (float)bandCount;
            }
            avgLv1Drums /= (float)bandCount;
            if(avgLv1Drums > avgLvlOldDrums + beatSensitivityDrums){
                beatValueDrums = avgLv1Drums - beatSensitivityDrums;
            }
            avgLvlOldDrums = avgLv1Drums;
        }
    }
    // updateLevelsRight();
}



void AudioAnalyzer::updateLevelsLeft()
{
    for(int i = 0; i < (bandCount); i++){
        levelsLeft[i]    = fmax(0.0f, levelsLeft[i] - 1.0f);
        levelsPtsLeft[i] = fmax(0.0f, levelsPtsLeft[i] - 0.95f);
    }
}

void AudioAnalyzer::updateLevelsRight()
{
    for(int i = 0; i < (bandCount); i++){
        levelsRight[i] = fmax(0.0f, levelsRight[i] - 1.0f);
        levelsPtsRight[i] = fmax(0.0f, levelsPtsRight[i] - 0.95f);
    }
}

void AudioAnalyzer::drawFftLeftScene()
{
    #ifdef DRAWFFT
    gl::pushMatrices();
    gl::translate(ci::Vec2i(200, 800));
    gl::rotate( getElapsedSeconds() * 10.f );
    gl::scale(ci::Vec3f(4.0, 4.0, 4.0));
    #endif
    drawFftLeft();
    #ifdef DRAWFFT
    gl::popMatrices();
    #endif
    
}

void AudioAnalyzer::drawFftRightScene()
{
    #ifdef DRAWFFT
    gl::pushMatrices();
    gl::translate(ci::Vec2i(200, 600));
    gl::rotate( getElapsedSeconds() * 10.f );
    gl::scale(ci::Vec3f(4.0, 4.0, 4.0));
    #endif
    drawFftRight();
    #ifdef DRAWFFT
    gl::popMatrices();
    #endif
}