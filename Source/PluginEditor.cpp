#include "PluginEditor.h"

R2RMusicCompressorAudioProcessorEditor::R2RMusicCompressorAudioProcessorEditor(R2RMusicCompressorAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(760, 470);
    title.setText("R2R MUSIC", juce::dontSendNotification);
    title.setFont(juce::Font(juce::FontOptions(30.0f, juce::Font::bold)));
    title.setColour(juce::Label::textColourId, juce::Colour(0xffff7a18));
    addAndMakeVisible(title);
    subtitle.setText("EASY ANALOG COMPRESSOR  •  VST3", juce::dontSendNotification);
    subtitle.setFont(juce::Font(juce::FontOptions(12.0f)));
    subtitle.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(subtitle);
    meterLabel.setJustificationType(juce::Justification::centred);
    meterLabel.setColour(juce::Label::textColourId, juce::Colour(0xffff9a36));
    addAndMakeVisible(meterLabel);

    setupSlider(threshold, "THRESHOLD", "dB");
    setupSlider(ratio, "RATIO", ":1");
    setupSlider(attack, "ATTACK", "ms");
    setupSlider(release, "RELEASE", "ms");
    setupSlider(knee, "KNEE", "dB");
    setupSlider(makeup, "MAKEUP", "dB");
    setupSlider(mix, "MIX", "%");
    setupSlider(output, "OUTPUT", "dB");

    juce::Slider* sliders[] = { &threshold, &ratio, &attack, &release, &knee, &makeup, &mix, &output };
    const char* ids[] = { "threshold", "ratio", "attack", "release", "knee", "makeup", "mix", "output" };
    for (int i = 0; i < 8; ++i)
        attachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, ids[i], *sliders[i]);
}

void R2RMusicCompressorAudioProcessorEditor::setupSlider(Slider& s, const juce::String& name, const juce::String& suffix)
{
    s.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(Slider::TextBoxBelow, false, 76, 20);
    s.setTextValueSuffix(" " + suffix);
    s.setColour(Slider::rotarySliderFillColourId, juce::Colour(0xffff7a18));
    s.setColour(Slider::rotarySliderOutlineColourId, juce::Colour(0xff3b3b3b));
    s.setColour(Slider::thumbColourId, juce::Colours::white);
    s.setColour(Slider::textBoxTextColourId, juce::Colours::white);
    s.setColour(Slider::textBoxBackgroundColourId, juce::Colour(0xff202020));
    s.setColour(Slider::textBoxOutlineColourId, juce::Colour(0xff444444));
    addAndMakeVisible(s);
    s.setName(name);
}

void R2RMusicCompressorAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff101010));
    g.setColour(juce::Colour(0xff242424));
    g.fillRoundedRectangle(18.0f, 18.0f, 724.0f, 434.0f, 14.0f);
    g.setColour(juce::Colour(0xffff7a18));
    g.fillRoundedRectangle(18.0f, 18.0f, 724.0f, 4.0f, 2.0f);
    g.setColour(juce::Colour(0xff555555));
    g.drawRoundedRectangle(18.0f, 18.0f, 724.0f, 434.0f, 14.0f, 1.0f);
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawText("STEREO LINK  •  SOFT KNEE  •  ANALOG DRIVE", 35, 420, 690, 18, juce::Justification::centred);
}

void R2RMusicCompressorAudioProcessorEditor::resized()
{
    title.setBounds(38, 28, 300, 42);
    subtitle.setBounds(40, 68, 360, 22);
    meterLabel.setBounds(540, 38, 170, 32);
    const int x0 = 38, y0 = 120, w = 165, h = 135, gap = 8;
    juce::Slider* sliders[] = { &threshold, &ratio, &attack, &release, &knee, &makeup, &mix, &output };
    for (int i = 0; i < 8; ++i)
    {
        const int row = i / 4, col = i % 4;
        sliders[i]->setBounds(x0 + col * (w + gap), y0 + row * 145, w, h);
    }
}
