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

 name:             SynthUsingMidiInputTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Synthesiser with midi input.

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
//#include "LowPassFilter.h"
//==============================================================================
struct SquareSound : public SynthesiserSound
{
	SquareSound() {}

	bool appliesToNote(int) override { return true; }
	bool appliesToChannel(int) override { return true; }
};

struct SquareVoice : public SynthesiserVoice
{
	SquareVoice()
	{
	}

	~SquareVoice() {
	}

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<SquareSound*> (sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity,
		SynthesiserSound*, int /*currentPitchWheelPosition*/) override
	{
		currentAngle = 0;
		level = velocity * 0.15;
		tailOff = 0.0;

		auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		auto cyclesPerSample = cyclesPerSecond / getSampleRate();

		angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
	}

	void stopNote(float /*velocity*/, bool allowTailOff) override
	{
		if (allowTailOff)
		{
			if (tailOff == 0.0)
				tailOff = 1.0;
		}
		else
		{
			clearCurrentNote();
			angleDelta = 0.0;
		}
	}

	void pitchWheelMoved(int) override {}
	void controllerMoved(int, int) override {}

	void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
	{
		if (angleDelta != 0.0)
		{
			if (tailOff > 0.0) // [7]
			{
				while (--numSamples >= 0)
				{
					auto currentWave = sgn(std::sin(currentAngle));
					auto currentSample = (float)(sgn(std::sin(currentAngle)) * level * tailOff);


					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					currentAngle += angleDelta;
					++startSample;

					tailOff *= 0.99; // [8]

					if (tailOff <= 0.005)
					{
						clearCurrentNote(); // [9]

						angleDelta = 0.0;
						break;
					}
				}
			}
			else
			{
				while (--numSamples >= 0) // [6]
				{
					auto currentSample = (float)(sgn(std::sin(currentAngle)) * level);

					for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					currentAngle += angleDelta;
					++startSample;
				}
			}
		}
	}

	template <typename T> int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}

private:
	double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
};
struct SineWaveSound   : public SynthesiserSound
{
    SineWaveSound() {}

    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};

//==============================================================================
struct SineWaveVoice   : public SynthesiserVoice
{
    SineWaveVoice()
	{
		//SerialPort port("COM3", 9600);
		//port.Open();
	}

    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0;
        level = velocity * 0.15;
        tailOff = 0.0;

        auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
    }

    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            if (tailOff == 0.0)
                tailOff = 1.0;
        }
        else
        {
            clearCurrentNote();
            angleDelta = 0.0;
        }
    }

    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0.0) // [7]
            {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float) (std::sin (currentAngle) * level * tailOff);

                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample (i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;

                    tailOff *= 0.99; // [8]

                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote(); // [9]

                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0) // [6]
                {
                    auto currentSample = (float) (std::sin (currentAngle) * level);

                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample (i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    }

private:
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
};

//==============================================================================
class SynthAudioSource   : public AudioSource
{
public:
    SynthAudioSource (MidiKeyboardState& keyState)
        : keyboardState (keyState)
    {

        for (auto i = 0; i < 4; ++i)                // [1]
            synth.addVoice (new SquareVoice());

        synth.addSound (new SquareSound());       // [2]
    }

    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate (sampleRate); // [3]
    }

    void releaseResources() override {}

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();

        MidiBuffer incomingMidi;
        keyboardState.processNextMidiBuffer (incomingMidi, bufferToFill.startSample,
                                             bufferToFill.numSamples, true);       // [4]

        synth.renderNextBlock (*bufferToFill.buffer, incomingMidi,
                               bufferToFill.startSample, bufferToFill.numSamples); // [5]
    }

private:
    MidiKeyboardState& keyboardState;
    Synthesiser synth;
};

//==============================================================================
class MainContentComponent   : public AudioAppComponent,
                               private Timer
								//public AsyncUpdater,
								//public Slider::Listener
{
public:
    MainContentComponent()
        : synthAudioSource  (keyboardState),
          keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard),
			filterSource(&synthAudioSource, false),
			port("COM3"),
			input(&port)
    {
        addAndMakeVisible (keyboardComponent);
        setAudioChannels (0, 2);

		addAndMakeVisible(cutoffLabel);
		cutoffLabel.setText("Frequency", dontSendNotification);
		cutoffLabel.attachToComponent(&filterCutoffDial, true);
		addAndMakeVisible(filterCutoffDial);
		filterCutoffDial.setAlwaysOnTop(true);
		filterCutoffDial.setRange(20.0, 20000.0, 1.0);
		filterCutoffDial.setValue(20000.0);
		//filterCutoffDial.addListener(this);
            // change the size of keyboard
        setSize (1000, 300);
        startTimer (400);
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void resized() override
    {
		filterCutoffDial.setBounds(200, 10, getWidth() - 210, 20);
		keyboardComponent.setBounds(10, 40, getWidth() - 20, getHeight() - 50);
    }
	/*
	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &filterCutoffDial)
			filterSource.setCoefficients(IIRCoefficients::makeLowPass(lastSampleRate, slider->getValue()));
			
	}
	*/


    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        //synthAudioSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
		lastSampleRate = sampleRate;
		filterSource.setCoefficients(IIRCoefficients::makeLowPass(sampleRate, 20000));
		filterSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        //synthAudioSource.getNextAudioBlock (bufferToFill);
		updateSlider();
		filterSource.getNextAudioBlock(bufferToFill);
    }

	void updateSlider() {
		//const ScopedLock myScopedLock(section);
		

		int controls;
		controls = input.readNextLine().getIntValue();
		auto controlval = controls + 500.0;
		printf("%f\n", controlval);
		if (filterCutoffDial.getValue() <= 0.0)
			filterSource.setCoefficients(IIRCoefficients::makeLowPass(lastSampleRate, 20.0, 50.0));
		else
			filterSource.setCoefficients(IIRCoefficients::makeLowPass(lastSampleRate, controlval, 50.0));

	}

    void releaseResources() override
    {
		port.close();
		filterSource.releaseResources();
		synthAudioSource.releaseResources();
    }
	/*
	void handleAsyncUpdate() override
	{
		updateSlider();
	}
	void sliderValueChanged(Slider * slider)
	{
		if (slider == &filterCutoffDial)
			triggerAsyncUpdate();
	}
	*/
private:
    void timerCallback() override
    {
        keyboardComponent.grabKeyboardFocus();

		//filterCutoffDial.setValue(controlval);
        stopTimer();
    }

    //==========================================================================
    SynthAudioSource synthAudioSource;
	IIRFilterAudioSource filterSource;
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
	Slider filterCutoffDial;
	Slider filterResDial;
	Label cutoffLabel;
	CriticalSection section;
	float lastSampleRate;
	SerialPort port;
	SerialPortInputStream input;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
