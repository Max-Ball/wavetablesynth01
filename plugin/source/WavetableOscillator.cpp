#include "CraneFlyFirstPlugin/WavetableOscillator.h"
#include <cmath>

WavetableOscillator::WavetableOscillator(std::vector<float> waveTable, double sampleRate)
    : waveTable{std::move(waveTable)},
        sampleRate{sampleRate}
{}

void WavetableOscillator::setFrequency(float frequency)
{
    indexIncrement = frequency * static_cast<float>(waveTable.size()) / static_cast<float>(sampleRate);
}

float WavetableOscillator::getSample()
{
    // Linear interpolation
    const auto sample = interpolateLinearly();

    //Increment index
    index += indexIncrement;

    // Take index and bring back to 0-wavetablesize range
    index = std::fmod(index, static_cast<float>(waveTable.size())); 
    return sample;
}

/* Linear interpolation takes the index between two integer indexes then returns the weighted sum of the wavetable values that correspond to the nearest indexes. The weighted values that we take for these two 
values are the distances from one index to the other index. With this we may weigh the sample that the index is nearer to with a larger weight.
*/
float WavetableOscillator::interpolateLinearly()
{
    const auto truncatedIndex = static_cast<int>(index);
    const auto nextIndex = (truncatedIndex + 1) % static_cast<int>(waveTable.size());

    const auto nextIndexWeight = index - static_cast<int>(truncatedIndex);
    const auto truncatedIndexWeight = 1.f - nextIndexWeight;

    return truncatedIndexWeight * waveTable[truncatedIndex] + nextIndexWeight * waveTable[nextIndex];
}

void WavetableOscillator::stop()
{
    // Reset index and index increment to 0
    index = 0.f;
    indexIncrement = 0.f;
}

bool WavetableOscillator::isPlaying()
{
    return indexIncrement != 0.f;
}