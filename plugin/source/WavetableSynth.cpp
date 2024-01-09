#include "CraneFlyFirstPlugin/WavetableSynth.h"

std::vector<float> WavetableSynth::generateSineWaveTable()
{
    constexpr auto WAVETABLE_LENGTH = 64;

    std::vector<float> sineWaveTable(WAVETABLE_LENGTH);

    const auto PI = std::atanf(1.f) * 4;

    for (auto i = 0; i < WAVETABLE_LENGTH; i++)
    {
        sineWaveTable[i] = std::sinf(
            2 * PI * static_cast<float>(i) / static_cast<float>(WAVETABLE_LENGTH)
        );
    }
    
    return sineWaveTable;
}

void WavetableSynth::initializeOscillators()
{
    // Set oscillator count - 128 = number of MIDI notes available
    constexpr auto OSCILLATORS_COUNT = 128;

    const auto waveTable = generateSineWaveTable();

    oscillators.clear();
    for (auto i = 0; i < OSCILLATORS_COUNT; i++)
    {
        oscillators.emplace_back(waveTable, sampleRate);
    }
    
}

void WavetableSynth::prepareToPlay(double sampleRate)
{
    this->sampleRate = sampleRate;
    initializeOscillators();
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

void WavetableSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    auto* firstChannel = buffer.getWritePointer(0);

    for (auto& oscillator : oscillators)
    {
        if(oscillator.isPlaying())
        {
            for (auto sample = startSample; sample < endSample; sample++)
            {
                firstChannel[sample] += oscillator.getSample();
            }
            
            // Copy to all remaining channels
            for (auto channel = 1; channel < buffer.getNumChannels(); channel++)
            {
                std::copy(firstChannel + startSample, firstChannel + endSample, 
                    buffer.getWritePointer(channel) + startSample);
            }
            
        }
    }
    
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
    } 
    else if (midiEvent.isNoteOff())
    { 
        const auto oscillatorId = midiEvent.getNoteNumber();
        oscillators[oscillatorId].stop();
    } 
    else if (midiEvent.isAllNotesOff())
    {
        for (auto& oscillator : oscillators)
        {
            oscillator.stop();
        }
        
    }
}
float WavetableSynth::midiNoteNumberToFrequency(int midiNoteNumber)
{
    // Set fundamental frequency to A440hz
    constexpr auto A4_FREQUENCY = 440.f;
    // Corresponding MIDI note of A440hz
    constexpr auto A4_NOTE_NUMBER = 69.f;
    // 12 semitones in an equal temperament octave
    constexpr auto SEMITONES_IN_AN_OCTAVE = 12.f;

    // Return frequency shifted by it's relative position. base (2.f) raised to the power of exponent (midiNoteNumber - A4_NOTE_NUMBER) divided by SEMITONES_IN_OCTAVE
    return A4_FREQUENCY * std::powf(2.f, (midiNoteNumber - A4_NOTE_NUMBER) / SEMITONES_IN_AN_OCTAVE);
}