/*
  ==============================================================================

    This file was auto-generated and contains the startup code for a PIP.

  ==============================================================================
*/

#include <JuceHeader.h>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "SynthUsingMidiInputTutorial_01.h"
//#include "WavetableSynthTutorial_01.h"
//#include "WavetableSynthTutorial_02.h"
//#include "WavetableSynthTutorial_03.h"
//#include "WavetableSynthTutorial_04.h"

//using namespace System;
//using namespace System::IO::Ports;

class Application    : public JUCEApplication
{
public:
    //==============================================================================
    Application() {}

    const String getApplicationName() override       {
        
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!
        
        //return "SynthUsingMidiInputTutorial"; 
        
        // uncomment if trying using wavetable synth
        return "WavetableSynthTutorial"; 
    }
    const String getApplicationVersion() override    {
        
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!
        
        // uncomment if trying to use different wavetbale tutorial file
         
        //return "1.0.0";
        //return "2.0.0"; 
        //return "3.0.0"; 
        return "4.0.0"; 
    }

    void initialise (const String&) override         { mainWindow.reset (new MainWindow ("SynthUsingMidiInputTutorial", new MainContentComponent(), *this)); }
    void shutdown() override                         { mainWindow = nullptr; }

private:
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (const String& name, Component* c, JUCEApplication& a)
            : DocumentWindow (name, Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons),
              app (a)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (c, true);

           #if JUCE_ANDROID || JUCE_IOS
            setFullScreen (true);
           #else
            setResizable (true, false);
            setResizeLimits (300, 250, 10000, 10000);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            app.systemRequestedQuit();
        }

    private:
        JUCEApplication& app;

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
START_JUCE_APPLICATION (Application)
