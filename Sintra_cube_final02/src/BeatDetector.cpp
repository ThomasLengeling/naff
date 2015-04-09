//
//  BeatDetector.cpp
//  ParticleSystemBloom
//
//  Created by tom on 4/24/14.
//
//
#include "BeatDetector.h"

BeatDetector::~BeatDetector()
{

}

BeatDetector::BeatDetector()
{
    std::fill(boost::begin(fftSmooth), boost::end(fftSmooth), 0.0f);
    std::fill(boost::begin(fftBands), boost::end(fftBands), 0.0f);
    std::fill(boost::begin(averageEnergy), boost::end(averageEnergy), 0.0f);
    memset(energyHistory, 0, sizeof(energyHistory));
    
    fftInit = true;
    detectBeats = true;
    
    beatValue = 120;
}

void BeatDetector::update(float *fftRef)
{
    for(int i = 0; i < BINS; i++){
        if(fftSmooth[i] < fftRef[i])
            fftSmooth[i] = fftRef[i];
        fftSmooth[i] *= 0.90f;
    }
    

    if(detectBeats){
        //BANDS
        int   bb_int   = BINS/BANDS;
        float bb_float = (float)BINS/(float)BANDS;
        for(int i = 0; i < BANDS; i++){
            fftBands[i] = 0.0f;

            for(int j = 0; j < bb_int; j++){
                fftBands[i] += fftSmooth[i*bb_int + j];
            }
            fftBands[i] = fftBands[i] * ( bb_float );
        }
        //ENERGY
        for(int i = 0; i < BANDS; i++){
            averageEnergy[i] = 0.0f;
            for(int e = 0; e < ENERGY; e++){
                averageEnergy[i] += energyHistory[i][e];
            }
            averageEnergy[i] /= (float)ENERGY;
        }
        //Energy History
        for(int i = 0; i < BANDS; i++){
            energyHistory[i][historyPos] = fftBands[i];
        }
        historyPos = (historyPos + 1) % ENERGY;
    }
}


bool BeatDetector::isBeat(int band)
{
    return (fftBands[band] > averageEnergy[band]*beatValue );
}

bool BeatDetector::isKick()
{
    return isBeat(0);
}

bool BeatDetector::isSnare()
{
    int low = 1.0;
    int hi  = BANDS/3.0;
    int threshold = (hi - low)/3.0;
    return isBeatRange(low, hi, threshold);
}

bool BeatDetector::isHat()
{
    int low = BANDS/2.0;
    int hi  = BANDS - 1.0;
    int threshold = (hi - low)/3.0;
    return isBeatRange(low, hi, threshold);
}

bool BeatDetector::isHigh()
{
    int low = 4 * (BANDS/5.0);
    int hi  = BANDS - 1.0;
    int thresh = (hi - low)/5.0;
    return isBeatRange(low, hi, thresh);
}

bool BeatDetector::isMidHigh()
{
    int low = 3 * (BANDS/5.0);
    int hi  = 4 * (BANDS/5.0);
    int thresh = (hi - low)/5.0;
    return isBeatRange(low, hi, thresh);
    
}

bool BeatDetector::isMid()
{
    int low = 2 * (BANDS/5.0);
    int hi  = 3 * (BANDS/5.0);
    int thresh = (hi - low)/5.0;
    return isBeatRange(low, hi, thresh);
}

bool BeatDetector::isMidLow()
{
	int low = (BANDS/5.0);
    int hi  = 2 * (BANDS/5.0);
    int thresh = (hi - low)/5.0;
    return isBeatRange(low, hi, thresh);
}

bool BeatDetector::isLow()
{
    int low = 1;
    int hi  = (BANDS/5.0);
    int thresh = (hi - low)/5.0;
    return isBeatRange(low, hi, thresh);
}

bool BeatDetector::isBeatRange(int low, int high, int threshold)
{
    int num = 0;
    for(int  i = low; i < high +1; i++)
        if(isBeat(i))
            num++;
    return (num > threshold);
}


float * BeatDetector::getFftSmooth()
{
    return fftSmooth;
}

float * BeatDetector::getFftBands()
{
    return fftBands;
}
