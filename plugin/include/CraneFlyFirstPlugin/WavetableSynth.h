#pragma once
#include "JuceHeader.h"
#include "WavetableOscillator.h"
#include <vector>

class WavetableSynth
{
public:
    void prepareToPlay(double sampleRate);
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);

private: 
    double sampleRate;
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    float midiNoteNumberToFrequency(int midiNoteNumber);
    void initializeOscillators();
    std::vector<WavetableOscillator> oscillators;
    std::vector<float> generateSineWaveTable();
    void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
};