cmake_minimum_required(VERSION 3.22)
project(R2RMusicEasyAnalogCompressor VERSION 1.0.0)

include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.6
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(R2RMusicCompressor
    COMPANY_NAME "DJ R2R Music"
    BUNDLE_ID "com.djr2rmusic.easyanalogcompressor"
    PLUGIN_MANUFACTURER_CODE R2RM
    PLUGIN_CODE R2EC
    FORMATS VST3
    PRODUCT_NAME "R2R MUSIC"
    DESCRIPTION "Easy Analog Compressor"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES Fx Dynamics)

juce_generate_juce_header(R2RMusicCompressor)

target_sources(R2RMusicCompressor PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h)

target_compile_features(R2RMusicCompressor PRIVATE cxx_std_17)
target_compile_definitions(R2RMusicCompressor PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(R2RMusicCompressor PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags)
