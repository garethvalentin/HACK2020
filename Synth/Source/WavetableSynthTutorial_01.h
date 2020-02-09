/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2017 - ROLI Ltd.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             WavetableSynthTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Wavetable synthesiser.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2017, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
class SineOscillator
{
public:
    SineOscillator() {}

    void setFrequency (float frequency, float sampleRate)
    {
        auto cyclesPerSample = frequency / sampleRate;
        angleDelta = cyclesPerSample * MathConstants<float>::twoPi;
    }

    forcedinline void updateAngle() noexcept
    {
        currentAngle += angleDelta;
        if (currentAngle >= MathConstants<float>::twoPi)
            currentAngle -= MathConstants<float>::twoPi;
    }

    forcedinline float getNextSample() noexcept
    {
        auto currentSample = std::sin (currentAngle);
        updateAngle();
        return currentSample;
    }

private:
    float currentAngle = 0.0f, angleDelta = 0.0f;
};

//==============================================================================
class MainContentComponent   : public AudioAppComponent,
                               public Timer
{
public:
    MainContentComponent()
    {
        cpuUsageLabel.setText ("CPU Usage", dontSendNotification);
        cpuUsageText.setJustificationType (Justification::right);
        addAndMakeVisible (cpuUsageLabel);
        addAndMakeVisible (cpuUsageText);

        setSize (400, 200);
        setAudioChannels (0, 2); // no inputs, two outputs
        startTimer (50);
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void resized() override
    {
        cpuUsageLabel.setBounds (10, 10, getWidth() - 20, 20);
        cpuUsageText .setBounds (10, 10, getWidth() - 20, 20);
    }

    void timerCallback() override
    {
        auto cpu = deviceManager.getCpuUsage() * 100;
        cpuUsageText.setText (String (cpu, 6) + " %", dontSendNotification);
    }

    void prepareToPlay (int, double sampleRate) override
    {
        auto numberOfOscillators = 200;

        for (auto i = 0; i < numberOfOscillators; ++i)
        {
            auto* oscillator = new SineOscillator();

            auto midiNote = Random::getSystemRandom().nextDouble() * 36.0 + 48.0;
            auto frequency = 440.0 * pow (2.0, (midiNote - 69.0) / 12.0);

            oscillator->setFrequency ((float) frequency, (float) sampleRate);
            oscillators.add (oscillator);
        }

        level = 0.25f / numberOfOscillators;
    }

    void releaseResources() override {}

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);

        bufferToFill.clearActiveBufferRegion();

        for (auto oscillatorIndex = 0; oscillatorIndex < oscillators.size(); ++oscillatorIndex)
        {
            auto* oscillator = oscillators.getUnchecked (oscillatorIndex);

            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                auto levelSample = oscillator->getNextSample() * level;

                leftBuffer[sample]  += levelSample;
                rightBuffer[sample] += levelSample;
            }
        }
    }

private:
    Label cpuUsageLabel;
    Label cpuUsageText;

    float level = 0.0f;
    OwnedArray<SineOscillator> oscillators;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
