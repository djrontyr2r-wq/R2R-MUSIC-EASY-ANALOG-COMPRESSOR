#pragma once
#include "PluginProcessor.h"

class R2RMusicCompressorAudioProcessorEditor final : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit R2RMusicCompressorAudioProcessorEditor(R2RMusicCompressorAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
private:
    using Slider = juce::Slider;
    R2RMusicCompressorAudioProcessor& processor;
    juce::Label title, subtitle, meterLabel;
    Slider threshold, ratio, attack, release, knee, makeup, mix, output;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachments[8];
    void setupSlider(Slider&, const juce::String&, const juce::String&);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(R2RMusicCompressorAudioProcessorEditor)
};
