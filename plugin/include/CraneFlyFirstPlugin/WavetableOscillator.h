#pragma once
#include <vector>
class WavetableOscillator 
{
private:
    std::vector<float> waveTable;
    double sampleRate;
    float index = 0.f;
    float indexIncrement = 0.f;
    float interpolateLinearly();

public:
    WavetableOscillator(std::vector<float> waveTable, double sampleRate);
    void setFrequency(float frequency);
    float getSample();
    void stop();
    bool isPlaying();
};
