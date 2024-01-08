#include "CraneFlyFirstPlugin/WavetableSynth.h"

void WavetableSynth::prepareToPlay(double sampleRate)
{
    this->sampleRate = sampleRate;
}

void WavetableSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Starting sample of processing block
    auto currentSample = 0;

    // Iterate over MIDI buffer
    for(const auto midiMessage : midiMessages)
    {
        // Get the MIDI message
        const auto midiEvent = midiMessage.getMessage();
        // Find the time of the event and cast to <int> allows iteration
        const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());

        // Render between currentSample and midiEvent since during that time the environment stable
        render(buffer, currentSample, midiEventSample);

        //Handle the midiEvent
        handleMidiEvent(midiEvent);

        //Update current sample to be at the index of the MIDI event
        currentSample = midiEventSample;
    }

    // After all MIDI events have been handled, render the last portion of Audio buffer
    render(buffer, currentSample, buffer);
}

void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
    // Check MIDI Event type
    if(midiEvent.isNoteOn())
    {
        // Assign an oscillator from the MIDI control data
        const auto oscillatorId = midiEvent.getNoteNumber();

        // Assign the frequency of the note based on the oscillatorId
        const auto frequency = midiNoteNumberToFrequency(oscillatorId);

        // Pick an oscillator from oscillator set to initialize with frequency
        oscillators[oscillatorId].setFrequency(frequency);
    } else if (midiEvent.isNoteOff())
    {

    } else if (midiEvent.isAllNotesOff())
    {

    }
}
