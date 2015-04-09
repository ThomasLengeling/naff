#pragma once

#define BINS    254
#define BANDS   16
#define ENERGY  43

class BeatDetector{
private:
    float fftSmooth[BINS];
    float fftBands[BANDS];
    float averageEnergy[BANDS];
    float energyHistory[BANDS][ENERGY];
    
    float beatValue;
    int   historyPos;
    bool  detectBeats;
    
    bool fftInit;
    
    bool isBeatRange(int low, int high, int threshold);
    
public:
    
    BeatDetector();
    ~BeatDetector();
    
    void update(float * fftRef);
    
    void enableBeatDetect(){detectBeats = true;}
    bool isBeat(int band);
    bool isKick();
    bool isSnare();
    bool isHat();
    
    bool isHigh();
    bool isMidHigh();
    bool isMid();
    bool isMidLow();
    bool isLow();

    void setBeatValue(float v){beatValue = v;}
    float & getBeatValue(){ return beatValue;}
    
    float * getFftSmooth();
    float * getFftBands();
    
    int getBandsNumber(){return BANDS;}
    int getBinsbumber(){return BINS;}
    
};