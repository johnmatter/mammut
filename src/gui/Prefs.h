/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JUCE_Includes.h"
#include <memory>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Prefs  : public Component,
               public Button::Listener
{
public:
    //==============================================================================
    Prefs ();
    ~Prefs();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    PropertiesFile *propertiesfile;
    bool firstRun_questionmark();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ToggleButton> soundonoffButton;
    std::unique_ptr<ToggleButton> movingcameraButton;
    std::unique_ptr<ToggleButton> animationButton;
    std::unique_ptr<ToggleButton> pictureButton;
    std::unique_ptr<ToggleButton> loopButton;
    std::unique_ptr<TextButton> audioSettingsButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Prefs)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
