//
// Created by Max Ball on 1/7/24.
//

#ifndef WOLFSOUNDTEMPLATE_WAVETABLESYNTH_H
#define WOLFSOUNDTEMPLATE_WAVETABLESYNTH_H

#include "JuceHeader.h"

class WavetableSynth
{
public:
    void prepareToPlay(double sampleRate);
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);

private: 
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    double sampleRate;
};


#endif //WOLFSOUNDTEMPLATE_WAVETABLESYNTH_H
