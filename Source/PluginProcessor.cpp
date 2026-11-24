#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
constexpr float pi = 3.14159265358979323846f;
float dbToGain(float db) { return std::pow(10.0f, db / 20.0f); }
}

R2RMusicCompressorAudioProcessor::R2RMusicCompressorAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

R2RMusicCompressorAudioProcessor::APVTS::ParameterLayout
R2RMusicCompressorAudioProcessor::createParameterLayout()
{
    APVTS::ParameterLayout p;
    auto add = [&p](const char* id, const char* name, float lo, float hi, float def, float step = 0.0f)
    {
        p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{id, 1}, name,
            juce::NormalisableRange<float>(lo, hi, step), def));
    };
    add("threshold", "Threshold", -60.0f, 0.0f, -18.0f);
    add("ratio", "Ratio", 1.0f, 20.0f, 4.0f, 0.1f);
    add("attack", "Attack", 0.1f, 100.0f, 10.0f);
    add("release", "Release", 10.0f, 1000.0f, 120.0f);
    add("knee", "Knee", 0.0f, 18.0f, 6.0f);
    add("makeup", "Makeup", 0.0f, 24.0f, 3.0f);
    add("mix", "Mix", 0.0f, 100.0f, 100.0f);
    add("output", "Output", -18.0f, 12.0f, 0.0f);
    return p;
}

void R2RMusicCompressorAudioProcessor::prepareToPlay(double sampleRate, int)
{
    sr = sampleRate;
    envelope = 0.0f;
    gainReductionDb.store(0.0f);
}

void R2RMusicCompressorAudioProcessor::releaseResources() {}

bool R2RMusicCompressorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto in = layouts.getMainInputChannelSet();
    const auto out = layouts.getMainOutputChannelSet();
    return in == out && (out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo());
}

void R2RMusicCompressorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int channels = buffer.getNumChannels();
    const int samples = buffer.getNumSamples();
    auto value = [this](const char* id) { return parameters.getRawParameterValue(id)->load(); };

    const float threshold = value("threshold");
    const float ratio = juce::jmax(1.0f, value("ratio"));
    const float knee = value("knee");
    const float attack = std::exp(-1.0f / (float(sr) * value("attack") * 0.001f));
    const float release = std::exp(-1.0f / (float(sr) * value("release") * 0.001f));
    const float makeup = dbToGain(value("makeup"));
    const float outGain = dbToGain(value("output"));
    const float wet = value("mix") * 0.01f;
    float maxGr = 0.0f;

    for (int i = 0; i < samples; ++i)
    {
        float peak = 0.0f;
        for (int ch = 0; ch < channels; ++ch)
            peak = juce::jmax(peak, std::abs(buffer.getReadPointer(ch)[i]));

        const float coeff = peak > envelope ? attack : release;
        envelope = coeff * envelope + (1.0f - coeff) * peak;
        const float levelDb = 20.0f * std::log10(juce::jmax(envelope, 1.0e-8f));
        const float over = levelDb - threshold;
        float compressedDb = 0.0f;

        if (knee > 0.0f && over > -knee * 0.5f && over < knee * 0.5f)
        {
            const float x = over + knee * 0.5f;
            compressedDb = (1.0f / ratio - 1.0f) * x * x / (2.0f * knee);
        }
        else if (over >= knee * 0.5f)
            compressedDb = (threshold + over / ratio) - levelDb;

        const float reduction = juce::jmin(0.0f, compressedDb);
        maxGr = juce::jmax(maxGr, -reduction);
        const float compGain = dbToGain(reduction) * makeup;

        for (int ch = 0; ch < channels; ++ch)
        {
            auto* data = buffer.getWritePointer(ch);
            const float dry = data[i];
            const float compressed = std::tanh(dry * compGain * 1.015f) / std::tanh(1.015f);
            data[i] = (dry + (compressed - dry) * wet) * outGain;
        }
    }
    gainReductionDb.store(maxGr);
}

juce::AudioProcessorEditor* R2RMusicCompressorAudioProcessor::createEditor()
{
    return new R2RMusicCompressorAudioProcessorEditor(*this);
}

void R2RMusicCompressorAudioProcessor::getStateInformation(juce::MemoryBlock& dest)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, dest);
}

void R2RMusicCompressorAudioProcessor::setStateInformation(const void* data, int size)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size));
    if (xml && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new R2RMusicCompressorAudioProcessor();
}
