/*
 // Copyright (c) 2022 Timothy Schoen and Wasted Audio
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */
#pragma once

#include "ExporterBase.h"
#include "ExportingProgressView.h"
#include "Toolchain.h"
#include "Utility/OSUtils.h"
#include <juce_gui_basics/juce_gui_basics.h>

// Custom property: pin selection combo with actual GPIO ids
class AdcPinProperty final : public PropertiesPanelProperty, public Value::Listener {
public:
    AdcPinProperty(String const& propertyName, Value& pinValue)
        : PropertiesPanelProperty(propertyName), value(pinValue)
    {
        // Common ADC-capable pins on ESP32
        addItem("Unassigned", 0);
        addItem("GPIO32", 32);
        addItem("GPIO33", 33);
        addItem("GPIO34", 34);
        addItem("GPIO35", 35);
        addItem("GPIO36", 36);
        addItem("GPIO39", 39);
        // ADC2 pins (availability may vary if Wi-Fi is used)
        addItem("GPIO4", 4);
        addItem("GPIO0", 0);
        addItem("GPIO2", 2);
        addItem("GPIO12", 12);
        addItem("GPIO13", 13);
        addItem("GPIO14", 14);
        addItem("GPIO15", 15);
        addItem("GPIO25", 25);
        addItem("GPIO26", 26);
        addItem("GPIO27", 27);

        combo.onChange = [this] {
            value = combo.getSelectedId();
        };
        value.addListener(this);
        addAndMakeVisible(combo);
        refresh();
    }

    void valueChanged(Value& v) override
    {
        const int pin = getValue<int>(v);
        if (combo.getSelectedId() != pin)
            combo.setSelectedId(pin, dontSendNotification);
    }

    void resized() override
    {
        combo.setBounds(getLocalBounds().removeFromRight(getWidth() / (2 - hideLabel)));
    }

private:
    void addItem(String const& label, int id)
    {
        combo.addItem(label, id);
    }

    Value& value;
    ComboBox combo;
};

// Custom property: Add/Remove ADC controls in a single row
class AdcControlsProperty final : public PropertiesPanelProperty {
public:
    AdcControlsProperty(std::function<void()> onAddFn, std::function<void()> onRemoveFn)
        : PropertiesPanelProperty("ADC Controls"), onAdd(std::move(onAddFn)), onRemove(std::move(onRemoveFn))
    {
        addButton.setButtonText("Add ADC");
        removeButton.setButtonText("Remove ADC");
        addButton.onClick = [this]{ if (onAdd) onAdd(); };
        removeButton.onClick = [this]{ if (onRemove) onRemove(); };
        addAndMakeVisible(addButton);
        addAndMakeVisible(removeButton);
    }

    void resized() override
    {
        auto r = getLocalBounds().removeFromRight(getWidth() / (2 - hideLabel));
        const int w = 120;
        const int h = jmax(24, r.getHeight() - 6);
        removeButton.setBounds(r.removeFromRight(w).withTrimmedTop(3).withHeight(h));
        addButton.setBounds(r.removeFromRight(w).withTrimmedTop(3).withHeight(h));
    }

private:
    TextButton addButton, removeButton;
    std::function<void()> onAdd, onRemove;
};

// Convenience property: quickly enable a third potentiometer
class AdcPreset3Property final : public PropertiesPanelProperty {
public:
    AdcPreset3Property(std::function<void()> onEnableFn)
        : PropertiesPanelProperty("Add 3rd Pot"), onEnable(std::move(onEnableFn))
    {
        enableButton.setButtonText("Enable 3rd Potentiometer");
        enableButton.onClick = [this]{ if (onEnable) onEnable(); };
        addAndMakeVisible(enableButton);
    }

    void resized() override
    {
        auto r = getLocalBounds().removeFromRight(getWidth() / (2 - hideLabel));
        const int w = 220;
        const int h = jmax(24, r.getHeight() - 6);
        enableButton.setBounds(r.removeFromRight(w).withTrimmedTop(3).withHeight(h));
    }

private:
    TextButton enableButton;
    std::function<void()> onEnable;
};

class ESP32Exporter final : public ExporterBase {
public:
    // Audio output selection: 1 = ESP32 DAC, 2 = External DAC
    Value audioOutputValue = SynchronousValue(var(1));
    // DAC pin selection (only applies when ESP32 DAC is selected)
    // 1 = GPIO25 (DAC CH0), 2 = GPIO26 (DAC CH1)
    Value leftDacPinValue = SynchronousValue(var(1));
    Value rightDacPinValue = SynchronousValue(var(2));
    // Advanced DAC options (collapsed by default)
    Value advancedOptionsValue = SynchronousValue(var(false));
    // Advanced parameters with defaults matching current implementation
    Value dacDescNumValue = SynchronousValue(var(4));
    Value dacBufSizeValue = SynchronousValue(var(512));
    Value dacOffsetValue = SynchronousValue(var(0));
    // 1 = DEFAULT, 2 = APLL
    Value dacClkSrcValue = SynchronousValue(var(1));
    // 1 = Alternating, 2 = Simultaneous
    Value dacChanModeValue = SynchronousValue(var(1));
    // Write timeout in ticks (-1 for blocking)
    Value dacWriteTimeoutValue = SynchronousValue(var(-1));
    // Staging buffer size for s_dac_buf
    Value dacStagingBufferValue = SynchronousValue(var(256));
    // Optional frequency override
    Value dacFreqOverrideEnable = SynchronousValue(var(false));
    Value dacFreqOverrideValue = SynchronousValue(var(48000));
    // I2S basic pins (only applies when External DAC is selected)
    Value i2sUseMclkValue = SynchronousValue(var(false));
    Value i2sMclkPinValue = SynchronousValue(var(0));
    Value i2sBclkPinValue = SynchronousValue(var(27));
    Value i2sWsPinValue = SynchronousValue(var(26));
    Value i2sDoutPinValue = SynchronousValue(var(25));
    Value i2sUseDinValue = SynchronousValue(var(false));
    Value i2sDinPinValue = SynchronousValue(var(19));
    // I2S advanced options
    Value i2sAdvancedOptionsValue = SynchronousValue(var(false));
    Value i2sPortValue = SynchronousValue(var(1)); // 1=Auto, 2=I2S0, 3=I2S1
    Value i2sRoleValue = SynchronousValue(var(1)); // 1=Master, 2=Slave
    Value i2sClockSrcValue = SynchronousValue(var(1)); // 1=Default, 2=PLL160M, 3=APLL
    Value i2sMclkMultipleValue = SynchronousValue(var(256));
    Value i2sDataBitWidthValue = SynchronousValue(var(16)); // 16/24/32
    Value i2sSlotModeValue = SynchronousValue(var(2)); // 1=Mono, 2=Stereo
    Value i2sInvertMclkValue = SynchronousValue(var(false));
    Value i2sInvertBclkValue = SynchronousValue(var(false));
    Value i2sInvertWsValue = SynchronousValue(var(false));
    Value i2sWriteTimeoutMsValue = SynchronousValue(var(0)); // 0=forever
    TextButton flashButton = TextButton("Flash");
    // Audio processing block size (frames)
    Value audioBlockSizeValue = SynchronousValue(var(128));
    PropertiesPanelProperty* leftPinProperty = nullptr;
    PropertiesPanelProperty* rightPinProperty = nullptr;
    PropertiesPanelProperty* advancedToggleProperty = nullptr;
    PropertiesPanelProperty* descNumProperty = nullptr;
    PropertiesPanelProperty* bufSizeProperty = nullptr;
    PropertiesPanelProperty* offsetProperty = nullptr;
    PropertiesPanelProperty* clkSrcProperty = nullptr;
    PropertiesPanelProperty* chanModeProperty = nullptr;
    PropertiesPanelProperty* writeTimeoutProperty = nullptr;
    PropertiesPanelProperty* stagingBufferProperty = nullptr;
    PropertiesPanelProperty* freqOverrideEnableProperty = nullptr;
    PropertiesPanelProperty* freqOverrideHzProperty = nullptr;
    // I2S UI properties
    PropertiesPanelProperty* i2sUseMclkProperty = nullptr;
    PropertiesPanelProperty* i2sMclkPinProperty = nullptr;
    PropertiesPanelProperty* i2sBclkPinProperty = nullptr;
    PropertiesPanelProperty* i2sWsPinProperty = nullptr;
    PropertiesPanelProperty* i2sDoutPinProperty = nullptr;
    PropertiesPanelProperty* i2sUseDinProperty = nullptr;
    PropertiesPanelProperty* i2sDinPinProperty = nullptr;
    PropertiesPanelProperty* i2sAdvancedToggleProperty = nullptr;
    PropertiesPanelProperty* i2sPortProperty = nullptr;
    PropertiesPanelProperty* i2sRoleProperty = nullptr;
    PropertiesPanelProperty* i2sClockSrcProperty = nullptr;
    PropertiesPanelProperty* i2sMclkMultipleProperty = nullptr;
    PropertiesPanelProperty* i2sDataBitWidthProperty = nullptr;
    PropertiesPanelProperty* i2sSlotModeProperty = nullptr;
    PropertiesPanelProperty* i2sInvertMclkProperty = nullptr;
    PropertiesPanelProperty* i2sInvertBclkProperty = nullptr;
    PropertiesPanelProperty* i2sInvertWsProperty = nullptr;
    PropertiesPanelProperty* i2sWriteTimeoutProperty = nullptr;

    // ADC GUI (UI-only for now): allow adding/removing ADCs with Name + Pin
    static constexpr int kMaxAdc = 8; // UI supports up to 8 ADC entries
    Value adcCountValue = SynchronousValue(var(3));
    Value adcNameValues[kMaxAdc];
    Value adcPinValues[kMaxAdc];
    PropertiesPanelProperty* adcNameProps[kMaxAdc] {};
    PropertiesPanelProperty* adcPinProps[kMaxAdc] {};
    TextButton addAdcButton { "Add ADC" };
    TextButton removeAdcButton { "Remove ADC" };
    // ADC poll period control
    Value adcPollMsValue = SynchronousValue(var(10));
    PropertiesPanelProperty* adcPollMsProperty = nullptr;

    ESP32Exporter(PluginEditor* editor, ExportingProgressView* exportingView)
        : ExporterBase(editor, exportingView)
    {
        // Audio Output section
        {
            PropertiesArray properties;
            properties.add(new PropertiesPanel::ComboComponent("Audio Output", audioOutputValue, { "ESP32 DAC", "External DAC" }));
            properties.add(new PropertiesPanel::EditableComponent<int>("Audio block size (frames)", audioBlockSizeValue, 16, 512));
            leftPinProperty = new PropertiesPanel::ComboComponent("Left pin (DAC)", leftDacPinValue, { "GPIO25 (CH0)", "GPIO26 (CH1)" });
            rightPinProperty = new PropertiesPanel::ComboComponent("Right pin (DAC)", rightDacPinValue, { "GPIO25 (CH0)", "GPIO26 (CH1)" });
            properties.add(leftPinProperty);
            properties.add(rightPinProperty);
            // Advanced options toggle and fields
            advancedToggleProperty = new PropertiesPanel::BoolComponent("Advanced DAC options", advancedOptionsValue, { "Off", "On" });
            properties.add(advancedToggleProperty);

            descNumProperty = new PropertiesPanel::EditableComponent<int>("Descriptors (desc_num)", dacDescNumValue, 1, 64);
            bufSizeProperty = new PropertiesPanel::EditableComponent<int>("Buffer size (bytes)", dacBufSizeValue, 32, 8192);
            offsetProperty = new PropertiesPanel::EditableComponent<int>("Offset", dacOffsetValue, 0, 255);
            clkSrcProperty = new PropertiesPanel::ComboComponent("Clock source", dacClkSrcValue, { "DEFAULT", "APLL" });
            chanModeProperty = new PropertiesPanel::ComboComponent("Channel mode", dacChanModeValue, { "Alternating", "Simultaneous" });
            writeTimeoutProperty = new PropertiesPanel::EditableComponent<int>("Write timeout (ticks)", dacWriteTimeoutValue, -1, 10000);
            stagingBufferProperty = new PropertiesPanel::EditableComponent<int>("Staging buffer (bytes)", dacStagingBufferValue, 32, 8192);
            freqOverrideEnableProperty = new PropertiesPanel::BoolComponent("Override frequency", dacFreqOverrideEnable, { "No", "Yes" });
            freqOverrideHzProperty = new PropertiesPanel::EditableComponent<int>("Frequency (Hz)", dacFreqOverrideValue, 8000, 192000);

            properties.add(descNumProperty);
            properties.add(bufSizeProperty);
            properties.add(offsetProperty);
            properties.add(clkSrcProperty);
            properties.add(chanModeProperty);
            properties.add(writeTimeoutProperty);
            properties.add(stagingBufferProperty);
            properties.add(freqOverrideEnableProperty);
            properties.add(freqOverrideHzProperty);
            // I2S basic pins
            i2sUseMclkProperty = new PropertiesPanel::BoolComponent("Use MCLK", i2sUseMclkValue, { "No", "Yes" });
            i2sMclkPinProperty = new PropertiesPanel::EditableComponent<int>("MCLK pin", i2sMclkPinValue, 0, 39);
            i2sBclkPinProperty = new PropertiesPanel::EditableComponent<int>("BCLK pin", i2sBclkPinValue, 0, 39);
            i2sWsPinProperty = new PropertiesPanel::EditableComponent<int>("WS pin", i2sWsPinValue, 0, 39);
            i2sDoutPinProperty = new PropertiesPanel::EditableComponent<int>("DOUT pin", i2sDoutPinValue, 0, 39);
            i2sUseDinProperty = new PropertiesPanel::BoolComponent("Use DIN", i2sUseDinValue, { "No", "Yes" });
            i2sDinPinProperty = new PropertiesPanel::EditableComponent<int>("DIN pin", i2sDinPinValue, 0, 39);
            properties.add(i2sUseMclkProperty);
            properties.add(i2sMclkPinProperty);
            properties.add(i2sBclkPinProperty);
            properties.add(i2sWsPinProperty);
            properties.add(i2sDoutPinProperty);
            properties.add(i2sUseDinProperty);
            properties.add(i2sDinPinProperty);

            // I2S advanced options
            i2sAdvancedToggleProperty = new PropertiesPanel::BoolComponent("Advanced I2S options", i2sAdvancedOptionsValue, { "Off", "On" });
            i2sPortProperty = new PropertiesPanel::ComboComponent("I2S Port", i2sPortValue, { "Auto", "I2S0", "I2S1" });
            i2sRoleProperty = new PropertiesPanel::ComboComponent("Role", i2sRoleValue, { "Master", "Slave" });
            i2sClockSrcProperty = new PropertiesPanel::ComboComponent("Clock source", i2sClockSrcValue, { "Default", "PLL_160M", "APLL" });
            i2sMclkMultipleProperty = new PropertiesPanel::ComboComponent("MCLK multiple", i2sMclkMultipleValue, { "128", "192", "256", "384", "512", "576", "768", "1024", "1152" });
            i2sDataBitWidthProperty = new PropertiesPanel::ComboComponent("Data bit width", i2sDataBitWidthValue, { "16", "24", "32" });
            i2sSlotModeProperty = new PropertiesPanel::ComboComponent("Slot mode", i2sSlotModeValue, { "Mono", "Stereo" });
            i2sInvertMclkProperty = new PropertiesPanel::BoolComponent("Invert MCLK", i2sInvertMclkValue, { "No", "Yes" });
            i2sInvertBclkProperty = new PropertiesPanel::BoolComponent("Invert BCLK", i2sInvertBclkValue, { "No", "Yes" });
            i2sInvertWsProperty = new PropertiesPanel::BoolComponent("Invert WS", i2sInvertWsValue, { "No", "Yes" });
            i2sWriteTimeoutProperty = new PropertiesPanel::EditableComponent<int>("Write timeout (ms)", i2sWriteTimeoutMsValue, 0, 600000);
            properties.add(i2sAdvancedToggleProperty);
            properties.add(i2sPortProperty);
            properties.add(i2sRoleProperty);
            properties.add(i2sClockSrcProperty);
            properties.add(i2sMclkMultipleProperty);
            properties.add(i2sDataBitWidthProperty);
            properties.add(i2sSlotModeProperty);
            properties.add(i2sInvertMclkProperty);
            properties.add(i2sInvertBclkProperty);
            properties.add(i2sInvertWsProperty);
            properties.add(i2sWriteTimeoutProperty);
            for (auto* property : properties) property->setPreferredHeight(28);
            panel.addSection("Audio Output", properties);
        }

        // ADC Inputs section (UI only)
        {
            // Initialize default values for ADC rows
            for (int i = 0; i < kMaxAdc; ++i) {
                adcNameValues[i] = SynchronousValue(var("Knob " + String(i + 1)));
                int defaultPin = 0;
                if (i == 0) defaultPin = 33; // GPIO33 (ADC1_CH5)
                else if (i == 1) defaultPin = 32; // GPIO32 (ADC1_CH4)
                else if (i == 2) defaultPin = 34; // GPIO34 (ADC1_CH6)
                adcPinValues[i]  = SynchronousValue(var(defaultPin));
            }

            PropertiesArray adcProps;
            for (int i = 0; i < kMaxAdc; ++i) {
                adcNameProps[i] = new PropertiesPanel::EditableComponent<String>("ADC " + String(i + 1) + " Name", adcNameValues[i]);
                adcPinProps[i]  = new AdcPinProperty("ADC " + String(i + 1) + " Pin", adcPinValues[i]);
                adcProps.add(adcNameProps[i]);
                adcProps.add(adcPinProps[i]);
            }
            // Controls row goes last, so it's near the title when no ADCs are visible, and pushed down as rows are added
            auto* controls = new AdcControlsProperty(
                [this]{
                    int count = getValue<int>(adcCountValue);
                    if (count < kMaxAdc) {
                        adcCountValue = count + 1;
                        if (adcNameProps[count]) adcNameProps[count]->setVisible(true);
                        if (adcPinProps[count])  adcPinProps[count]->setVisible(true);
                        panel.updatePropHolderLayout();
                    }
                },
                [this]{
                    int count = getValue<int>(adcCountValue);
                    if (count > 0) {
                        count -= 1;
                        adcCountValue = count;
                        if (adcNameProps[count]) adcNameProps[count]->setVisible(false);
                        if (adcPinProps[count])  adcPinProps[count]->setVisible(false);
                        panel.updatePropHolderLayout();
                    }
                }
            );
            adcProps.add(controls);
            // ADC poll period (ms), controls how often ADCs are sampled off the audio thread
            adcPollMsProperty = new PropertiesPanel::EditableComponent<int>("ADC poll period (ms)", adcPollMsValue, 1, 100);
            adcProps.add(adcPollMsProperty);
            for (auto* property : adcProps) property->setPreferredHeight(28);
            panel.addSection("ADC Inputs", adcProps);
        }

        // Simplified panel: only a Flash button
        exportButton.setVisible(false);
        addAndMakeVisible(flashButton);

        auto const backgroundColour = findColour(PlugDataColour::panelBackgroundColourId);
        flashButton.setColour(TextButton::buttonColourId, backgroundColour.contrasting(0.05f));
        flashButton.setColour(TextButton::buttonOnColourId, backgroundColour.contrasting(0.1f));
        flashButton.setColour(ComboBox::outlineColourId, Colours::transparentBlack);

        flashButton.onClick = [this] {
            // Use a persistent project directory to enable incremental builds between flashes
            auto appRoot = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("plugdata").getChildFile("esp32");
            appRoot.createDirectory();
            String projName = getValue<String>(projectNameValue);
            if (projName.isEmpty()) projName = "plugdata_esp_export";
            auto const projFolder = appRoot.getChildFile(projName);
            startExport(projFolder);
        };

        // Listen for changes to visibility/validate pin choices and advanced options
        audioOutputValue.addListener(this);
        leftDacPinValue.addListener(this);
        rightDacPinValue.addListener(this);
        advancedOptionsValue.addListener(this);
        dacFreqOverrideEnable.addListener(this);
        // I2S listeners
        i2sUseMclkValue.addListener(this);
        i2sUseDinValue.addListener(this);
        i2sAdvancedOptionsValue.addListener(this);
        // Initial enable state
        auto initVisibility = [this]() {
            bool const useDac = getValue<int>(audioOutputValue) == 1;
            bool const advOn = useDac && getValue<bool>(advancedOptionsValue);
            if (leftPinProperty && rightPinProperty) {
                leftPinProperty->setVisible(useDac);
                rightPinProperty->setVisible(useDac);
            }
            if (advancedToggleProperty) advancedToggleProperty->setVisible(useDac);
            auto setAdvVis = [advOn](PropertiesPanelProperty* p){ if (p) p->setVisible(advOn); };
            setAdvVis(descNumProperty);
            setAdvVis(bufSizeProperty);
            setAdvVis(offsetProperty);
            setAdvVis(clkSrcProperty);
            setAdvVis(chanModeProperty);
            setAdvVis(writeTimeoutProperty);
            setAdvVis(stagingBufferProperty);
            if (freqOverrideEnableProperty) freqOverrideEnableProperty->setVisible(advOn);
            bool const freqOn = advOn && getValue<bool>(dacFreqOverrideEnable);
            if (freqOverrideHzProperty) freqOverrideHzProperty->setVisible(freqOn);
            // I2S visibility
            bool const useI2S = getValue<int>(audioOutputValue) == 2;
            auto setI2SVis = [useI2S](PropertiesPanelProperty* p){ if (p) p->setVisible(useI2S); };
            setI2SVis(i2sUseMclkProperty);
            bool const showMclkPin = useI2S && getValue<bool>(i2sUseMclkValue);
            if (i2sMclkPinProperty) i2sMclkPinProperty->setVisible(showMclkPin);
            setI2SVis(i2sBclkPinProperty);
            setI2SVis(i2sWsPinProperty);
            setI2SVis(i2sDoutPinProperty);
            setI2SVis(i2sUseDinProperty);
            bool const showDinPin = useI2S && getValue<bool>(i2sUseDinValue);
            if (i2sDinPinProperty) i2sDinPinProperty->setVisible(showDinPin);
            bool const i2sAdv = useI2S && getValue<bool>(i2sAdvancedOptionsValue);
            if (i2sAdvancedToggleProperty) i2sAdvancedToggleProperty->setVisible(useI2S);
            auto setI2SAdvVis = [i2sAdv](PropertiesPanelProperty* p){ if (p) p->setVisible(i2sAdv); };
            setI2SAdvVis(i2sPortProperty);
            setI2SAdvVis(i2sRoleProperty);
            setI2SAdvVis(i2sClockSrcProperty);
            setI2SAdvVis(i2sMclkMultipleProperty);
            setI2SAdvVis(i2sDataBitWidthProperty);
            setI2SAdvVis(i2sSlotModeProperty);
            setI2SAdvVis(i2sInvertMclkProperty);
            setI2SAdvVis(i2sInvertBclkProperty);
            setI2SAdvVis(i2sInvertWsProperty);
            setI2SAdvVis(i2sWriteTimeoutProperty);
            // ADC rows visibility based on adcCountValue
            int count = jlimit(0, kMaxAdc, getValue<int>(adcCountValue));
            for (int i = 0; i < kMaxAdc; ++i) {
                const bool vis = (i < count);
                if (adcNameProps[i]) adcNameProps[i]->setVisible(vis);
                if (adcPinProps[i])  adcPinProps[i]->setVisible(vis);
            }
            panel.updatePropHolderLayout();
        };
        initVisibility();

    }

    void resized() override {
        ExporterBase::resized();
        flashButton.setBounds(exportButton.getBounds());
    }

    ValueTree getState() override {
        ValueTree stateTree("ESP32Exporter");
        stateTree.setProperty("inputPatchValue", getValue<String>(inputPatchValue), nullptr);
        stateTree.setProperty("projectNameValue", getValue<String>(projectNameValue), nullptr);
        stateTree.setProperty("projectCopyrightValue", getValue<String>(projectCopyrightValue), nullptr);
        stateTree.setProperty("audioOutputValue", getValue<int>(audioOutputValue), nullptr);
        stateTree.setProperty("leftDacPinValue", getValue<int>(leftDacPinValue), nullptr);
        stateTree.setProperty("rightDacPinValue", getValue<int>(rightDacPinValue), nullptr);
        // Advanced
        stateTree.setProperty("advancedOptionsValue", getValue<bool>(advancedOptionsValue), nullptr);
        stateTree.setProperty("dacDescNumValue", getValue<int>(dacDescNumValue), nullptr);
        stateTree.setProperty("dacBufSizeValue", getValue<int>(dacBufSizeValue), nullptr);
        stateTree.setProperty("dacOffsetValue", getValue<int>(dacOffsetValue), nullptr);
        stateTree.setProperty("dacClkSrcValue", getValue<int>(dacClkSrcValue), nullptr);
        stateTree.setProperty("dacChanModeValue", getValue<int>(dacChanModeValue), nullptr);
        stateTree.setProperty("dacWriteTimeoutValue", getValue<int>(dacWriteTimeoutValue), nullptr);
        stateTree.setProperty("dacStagingBufferValue", getValue<int>(dacStagingBufferValue), nullptr);
        stateTree.setProperty("dacFreqOverrideEnable", getValue<bool>(dacFreqOverrideEnable), nullptr);
        stateTree.setProperty("dacFreqOverrideValue", getValue<int>(dacFreqOverrideValue), nullptr);
        // I2S
        stateTree.setProperty("i2sUseMclkValue", getValue<bool>(i2sUseMclkValue), nullptr);
        stateTree.setProperty("i2sMclkPinValue", getValue<int>(i2sMclkPinValue), nullptr);
        stateTree.setProperty("i2sBclkPinValue", getValue<int>(i2sBclkPinValue), nullptr);
        stateTree.setProperty("i2sWsPinValue", getValue<int>(i2sWsPinValue), nullptr);
        stateTree.setProperty("i2sDoutPinValue", getValue<int>(i2sDoutPinValue), nullptr);
        stateTree.setProperty("i2sUseDinValue", getValue<bool>(i2sUseDinValue), nullptr);
        stateTree.setProperty("i2sDinPinValue", getValue<int>(i2sDinPinValue), nullptr);
        stateTree.setProperty("i2sAdvancedOptionsValue", getValue<bool>(i2sAdvancedOptionsValue), nullptr);
        stateTree.setProperty("i2sPortValue", getValue<int>(i2sPortValue), nullptr);
        stateTree.setProperty("i2sRoleValue", getValue<int>(i2sRoleValue), nullptr);
        stateTree.setProperty("i2sClockSrcValue", getValue<int>(i2sClockSrcValue), nullptr);
        stateTree.setProperty("i2sMclkMultipleValue", getValue<int>(i2sMclkMultipleValue), nullptr);
        stateTree.setProperty("i2sDataBitWidthValue", getValue<int>(i2sDataBitWidthValue), nullptr);
        stateTree.setProperty("i2sSlotModeValue", getValue<int>(i2sSlotModeValue), nullptr);
        stateTree.setProperty("i2sInvertMclkValue", getValue<bool>(i2sInvertMclkValue), nullptr);
        stateTree.setProperty("i2sInvertBclkValue", getValue<bool>(i2sInvertBclkValue), nullptr);
        stateTree.setProperty("i2sInvertWsValue", getValue<bool>(i2sInvertWsValue), nullptr);
        stateTree.setProperty("i2sWriteTimeoutMsValue", getValue<int>(i2sWriteTimeoutMsValue), nullptr);
        // ADC (UI only)
    stateTree.setProperty("adcCountValue", getValue<int>(adcCountValue), nullptr);
    stateTree.setProperty("adcPollMsValue", getValue<int>(adcPollMsValue), nullptr);
        ValueTree adcTree("ADCInputs");
        int count = jlimit(0, kMaxAdc, getValue<int>(adcCountValue));
        for (int i = 0; i < count; ++i) {
            ValueTree item("ADC");
            item.setProperty("name", getValue<String>(adcNameValues[i]), nullptr);
            item.setProperty("pin",  getValue<int>(adcPinValues[i]), nullptr);
            adcTree.addChild(item, -1, nullptr);
        }
        stateTree.addChild(adcTree, -1, nullptr);
        return stateTree;
    }

    void setState(ValueTree& stateTree) override {
        auto const tree = stateTree.getChildWithName("ESP32Exporter");
        inputPatchValue = tree.getProperty("inputPatchValue");
        projectNameValue = tree.getProperty("projectNameValue");
        projectCopyrightValue = tree.getProperty("projectCopyrightValue");
        if (tree.hasProperty("audioOutputValue"))
            audioOutputValue = tree.getProperty("audioOutputValue");
        if (tree.hasProperty("leftDacPinValue"))
            leftDacPinValue = tree.getProperty("leftDacPinValue");
        if (tree.hasProperty("rightDacPinValue"))
            rightDacPinValue = tree.getProperty("rightDacPinValue");
    // Advanced
    if (tree.hasProperty("advancedOptionsValue")) advancedOptionsValue = tree.getProperty("advancedOptionsValue");
    if (tree.hasProperty("dacDescNumValue")) dacDescNumValue = tree.getProperty("dacDescNumValue");
    if (tree.hasProperty("dacBufSizeValue")) dacBufSizeValue = tree.getProperty("dacBufSizeValue");
    if (tree.hasProperty("dacOffsetValue")) dacOffsetValue = tree.getProperty("dacOffsetValue");
    if (tree.hasProperty("dacClkSrcValue")) dacClkSrcValue = tree.getProperty("dacClkSrcValue");
    if (tree.hasProperty("dacChanModeValue")) dacChanModeValue = tree.getProperty("dacChanModeValue");
    if (tree.hasProperty("dacWriteTimeoutValue")) dacWriteTimeoutValue = tree.getProperty("dacWriteTimeoutValue");
    if (tree.hasProperty("dacStagingBufferValue")) dacStagingBufferValue = tree.getProperty("dacStagingBufferValue");
    if (tree.hasProperty("dacFreqOverrideEnable")) dacFreqOverrideEnable = tree.getProperty("dacFreqOverrideEnable");
    if (tree.hasProperty("dacFreqOverrideValue")) dacFreqOverrideValue = tree.getProperty("dacFreqOverrideValue");
        // I2S
        if (tree.hasProperty("i2sUseMclkValue")) i2sUseMclkValue = tree.getProperty("i2sUseMclkValue");
        if (tree.hasProperty("i2sMclkPinValue")) i2sMclkPinValue = tree.getProperty("i2sMclkPinValue");
        if (tree.hasProperty("i2sBclkPinValue")) i2sBclkPinValue = tree.getProperty("i2sBclkPinValue");
        if (tree.hasProperty("i2sWsPinValue")) i2sWsPinValue = tree.getProperty("i2sWsPinValue");
        if (tree.hasProperty("i2sDoutPinValue")) i2sDoutPinValue = tree.getProperty("i2sDoutPinValue");
        if (tree.hasProperty("i2sUseDinValue")) i2sUseDinValue = tree.getProperty("i2sUseDinValue");
        if (tree.hasProperty("i2sDinPinValue")) i2sDinPinValue = tree.getProperty("i2sDinPinValue");
        if (tree.hasProperty("i2sAdvancedOptionsValue")) i2sAdvancedOptionsValue = tree.getProperty("i2sAdvancedOptionsValue");
        if (tree.hasProperty("i2sPortValue")) i2sPortValue = tree.getProperty("i2sPortValue");
        if (tree.hasProperty("i2sRoleValue")) i2sRoleValue = tree.getProperty("i2sRoleValue");
        if (tree.hasProperty("i2sClockSrcValue")) i2sClockSrcValue = tree.getProperty("i2sClockSrcValue");
        if (tree.hasProperty("i2sMclkMultipleValue")) i2sMclkMultipleValue = tree.getProperty("i2sMclkMultipleValue");
        if (tree.hasProperty("i2sDataBitWidthValue")) i2sDataBitWidthValue = tree.getProperty("i2sDataBitWidthValue");
        if (tree.hasProperty("i2sSlotModeValue")) i2sSlotModeValue = tree.getProperty("i2sSlotModeValue");
        if (tree.hasProperty("i2sInvertMclkValue")) i2sInvertMclkValue = tree.getProperty("i2sInvertMclkValue");
        if (tree.hasProperty("i2sInvertBclkValue")) i2sInvertBclkValue = tree.getProperty("i2sInvertBclkValue");
        if (tree.hasProperty("i2sInvertWsValue")) i2sInvertWsValue = tree.getProperty("i2sInvertWsValue");
        if (tree.hasProperty("i2sWriteTimeoutMsValue")) i2sWriteTimeoutMsValue = tree.getProperty("i2sWriteTimeoutMsValue");
        // ADC (UI only)
        if (tree.hasProperty("adcCountValue")) adcCountValue = tree.getProperty("adcCountValue");
    if (tree.hasProperty("adcPollMsValue")) adcPollMsValue = tree.getProperty("adcPollMsValue");
        if (auto adcTree = tree.getChildWithName("ADCInputs"); adcTree.isValid()) {
            auto count = jmin(adcTree.getNumChildren(), kMaxAdc);
            for (int i = 0; i < count; ++i) {
                auto item = adcTree.getChild(i);
                if (item.hasProperty("name")) adcNameValues[i] = item.getProperty("name");
                if (item.hasProperty("pin"))  adcPinValues[i]  = item.getProperty("pin");
            }
        }
        // Update visibility after loading
        {
            int count = jlimit(0, kMaxAdc, getValue<int>(adcCountValue));
            for (int i = 0; i < kMaxAdc; ++i) {
                const bool vis = (i < count);
                if (adcNameProps[i]) adcNameProps[i]->setVisible(vis);
                if (adcPinProps[i])  adcPinProps[i]->setVisible(vis);
            }
            panel.updatePropHolderLayout();
        }
    }

    void valueChanged(Value& v) override {
        // Preserve base behavior (patch selection etc.)
        ExporterBase::valueChanged(v);

        // Show/hide sections based on audio output and advanced toggles
        if (leftPinProperty && rightPinProperty) {
            bool const useDac = getValue<int>(audioOutputValue) == 1;
            leftPinProperty->setVisible(useDac);
            rightPinProperty->setVisible(useDac);
        }
        if (advancedToggleProperty || descNumProperty) {
            bool const useDac = getValue<int>(audioOutputValue) == 1;
            bool const advOn = useDac && getValue<bool>(advancedOptionsValue);
            if (advancedToggleProperty) advancedToggleProperty->setVisible(useDac);
            auto setAdvVis = [advOn](PropertiesPanelProperty* p){ if (p) p->setVisible(advOn); };
            setAdvVis(descNumProperty);
            setAdvVis(bufSizeProperty);
            setAdvVis(offsetProperty);
            setAdvVis(clkSrcProperty);
            setAdvVis(chanModeProperty);
            setAdvVis(writeTimeoutProperty);
            setAdvVis(stagingBufferProperty);
            if (freqOverrideEnableProperty) freqOverrideEnableProperty->setVisible(advOn);
            bool const freqOn = advOn && getValue<bool>(dacFreqOverrideEnable);
            if (freqOverrideHzProperty) freqOverrideHzProperty->setVisible(freqOn);
        }
        // I2S visibility changes
        if (i2sUseMclkProperty || i2sBclkPinProperty) {
            bool const useI2S = getValue<int>(audioOutputValue) == 2;
            auto setI2SVis = [useI2S](PropertiesPanelProperty* p){ if (p) p->setVisible(useI2S); };
            setI2SVis(i2sUseMclkProperty);
            bool const showMclkPin = useI2S && getValue<bool>(i2sUseMclkValue);
            if (i2sMclkPinProperty) i2sMclkPinProperty->setVisible(showMclkPin);
            setI2SVis(i2sBclkPinProperty);
            setI2SVis(i2sWsPinProperty);
            setI2SVis(i2sDoutPinProperty);
            setI2SVis(i2sUseDinProperty);
            bool const showDinPin = useI2S && getValue<bool>(i2sUseDinValue);
            if (i2sDinPinProperty) i2sDinPinProperty->setVisible(showDinPin);
            bool const i2sAdv = useI2S && getValue<bool>(i2sAdvancedOptionsValue);
            if (i2sAdvancedToggleProperty) i2sAdvancedToggleProperty->setVisible(useI2S);
            auto setI2SAdvVis = [i2sAdv](PropertiesPanelProperty* p){ if (p) p->setVisible(i2sAdv); };
            setI2SAdvVis(i2sPortProperty);
            setI2SAdvVis(i2sRoleProperty);
            setI2SAdvVis(i2sClockSrcProperty);
            setI2SAdvVis(i2sMclkMultipleProperty);
            setI2SAdvVis(i2sDataBitWidthProperty);
            setI2SAdvVis(i2sSlotModeProperty);
            setI2SAdvVis(i2sInvertMclkProperty);
            setI2SAdvVis(i2sInvertBclkProperty);
            setI2SAdvVis(i2sInvertWsProperty);
            setI2SAdvVis(i2sWriteTimeoutProperty);
        }
        panel.updatePropHolderLayout();

        // Ensure left/right aren't the same channel when DAC is used
        if (v.refersToSameSourceAs(leftDacPinValue) || v.refersToSameSourceAs(rightDacPinValue) || v.refersToSameSourceAs(audioOutputValue)) {
            if (getValue<int>(audioOutputValue) == 1) {
                int left = getValue<int>(leftDacPinValue);
                int right = getValue<int>(rightDacPinValue);
                if (left == right) {
                    // Flip the right channel to the other pin
                    rightDacPinValue = (left == 1 ? 2 : 1);
                }
            }
        }
    }

    bool performExport(String const& pdPatch, String const& outdir, String const& name,
                       String const& copyright, StringArray const& searchPaths) override
    {
    // 1) Create a temporary ESP-IDF project skeleton on-demand
        File tempProjRoot(outdir);
        if (! tempProjRoot.exists()) tempProjRoot.createDirectory();
        auto projectName = String("plugdata_esp_export");

        // Directories
        File mainDir = tempProjRoot.getChildFile("main");
        File cDir = tempProjRoot.getChildFile("c"); // hvcc will generate here
        mainDir.createDirectory();
        cDir.createDirectory();

        // Top-level CMakeLists.txt
        {
            String cmakeTop;
            cmakeTop << "cmake_minimum_required(VERSION 3.16)\n";
            cmakeTop << "include($ENV{IDF_PATH}/tools/cmake/project.cmake)\n";
            cmakeTop << "project(" << projectName << ")\n";
            cmakeTop << "idf_build_set_property(MINIMAL_BUILD ON)\n";
            tempProjRoot.getChildFile("CMakeLists.txt").replaceWithText(cmakeTop);
        }

        // main/CMakeLists.txt (glob hvcc sources)
        {
            String cmakeMain;
            cmakeMain << "cmake_minimum_required(VERSION 3.16)\n\n";
            cmakeMain << "set(srcs \"app_main.cpp\")\n";
            cmakeMain << "file(GLOB hvcc_c \"../c/*.c\")\n";
            cmakeMain << "file(GLOB hvcc_cpp \"../c/*.cpp\")\n";
            cmakeMain << "list(APPEND srcs ${hvcc_c} ${hvcc_cpp})\n\n";
            cmakeMain << "idf_component_register(\n";
            cmakeMain << "    SRCS ${srcs}\n";
            cmakeMain << "    INCLUDE_DIRS \".\" \"../c\"\n";
            // Rely on core 'driver' component for DAC continuous on ESP32.
            // Some ESP-IDF versions package DAC as an external 'esp_driver_dac' component; if needed, users can add it via idf.py add-dependency.
            cmakeMain << "    REQUIRES driver esp_adc\n";
            cmakeMain << ")\n";
            mainDir.getChildFile("CMakeLists.txt").replaceWithText(cmakeMain);
        }

        // main/config.h and main/app_main.cpp
        {
            String configH;
            int blockSize = getValue<int>(audioBlockSizeValue);
            // Resolve up to two ADC knob names and pins from the UI (fallbacks if none defined)
            int adcCount = getValue<int>(adcCountValue);
            String knob1Name = "Knob1";
            int knob1Pin = 33; // default to GPIO33
            String knob2Name = "Knob2";
            int knob2Pin = 32; // default to GPIO32
            if (adcCount > 0) {
                knob1Name = getValue<String>(adcNameValues[0]);
                knob1Pin = getValue<int>(adcPinValues[0]);
                if (knob1Name.isEmpty()) knob1Name = "Knob1";
                if (knob1Pin <= 0) knob1Pin = 33;
            }
            if (adcCount > 1) {
                knob2Name = getValue<String>(adcNameValues[1]);
                knob2Pin = getValue<int>(adcPinValues[1]);
                if (knob2Name.isEmpty()) knob2Name = "Knob2";
                if (knob2Pin <= 0) knob2Pin = 32;
            }
            if (getValue<int>(audioOutputValue) == 1) {
                // ESP32 DAC path (continuous mode)
                configH << "#ifndef CONFIG_H\n#define CONFIG_H\n\n";
                configH << "#include <stdint.h>\n";
                configH << "#include <math.h>\n";
                configH << "#include \"esp_check.h\"\n";
                configH << "#include \"driver/dac_continuous.h\"\n\n";
                configH << "// DAC handle and small staging buffer for efficient DMA writes\n";
                configH << "static dac_continuous_handle_t s_dac = nullptr;\n";
                // Staging buffer size (configurable)
                int stagingSize = getValue<int>(dacStagingBufferValue);
                if (stagingSize < blockSize * 2) stagingSize = blockSize * 2; // ensure staging buffer flush aligns with at least one audio block
                configH << "static uint8_t s_dac_buf[" << String(stagingSize) << "];\n";
                configH << "static size_t s_dac_idx = 0;\n\n";
                // Determine which DAC channel outputs LEFT: CH0=GPIO25, CH1=GPIO26
                bool leftIsCh0 = (getValue<int>(leftDacPinValue) == 1);
                configH << String("static const bool LEFT_IS_CH0 = ") << (leftIsCh0 ? "true" : "false") << ";\n\n";
                configH << "// Initialize DAC continuous mode on GPIO25 (DAC channel 0) at the given sample rate\n";
                configH << "void audio_init(uint32_t& sample_rate)\n";
                configH << "{\n";
                // Advanced config values
                int descNum = getValue<int>(dacDescNumValue);
                int bufSize = getValue<int>(dacBufSizeValue);
                int offset = getValue<int>(dacOffsetValue);
                int clkSel = getValue<int>(dacClkSrcValue);
                String clkConst = (clkSel == 2 ? "DAC_DIGI_CLK_SRC_APLL" : "DAC_DIGI_CLK_SRC_DEFAULT");
                int modeSel = getValue<int>(dacChanModeValue);
                String modeConst = (modeSel == 2 ? "DAC_CHANNEL_MODE_SIMUL" : "DAC_CHANNEL_MODE_ALTER");
                bool freqOverride = getValue<bool>(dacFreqOverrideEnable);
                int freqHz = getValue<int>(dacFreqOverrideValue);
                configH << "    if (" << (freqOverride ? "true" : "false") << ") { sample_rate = " << String(freqHz) << "; }\n";
                // In ALTERNATING mode the driver alternates channel bytes, halving the per-channel sample rate.
                // Compensate by doubling the stream frequency so each channel runs at the requested sample_rate.
                configH << "    uint32_t stream_freq = sample_rate * (" << (modeSel == 1 ? "2" : "1") << ");\n";
                configH << "    dac_continuous_config_t cfg = {\n";
                configH << "        .chan_mask = (dac_channel_mask_t)((LEFT_IS_CH0 ? DAC_CHANNEL_MASK_CH0 : DAC_CHANNEL_MASK_CH1) | (LEFT_IS_CH0 ? DAC_CHANNEL_MASK_CH1 : DAC_CHANNEL_MASK_CH0)),\n";
                configH << "        .desc_num = " << String(descNum) << ",\n";
                configH << "        .buf_size = " << String(bufSize) << ",\n";
                configH << "        .freq_hz = stream_freq,\n";
                configH << "        .offset = " << String(offset) << ",\n";
                configH << "        .clk_src = " << clkConst << ",\n";
                configH << "        .chan_mode = " << modeConst << ",\n";
                configH << "    };\n\n";
                configH << "    ESP_ERROR_CHECK(dac_continuous_new_channels(&cfg, &s_dac));\n";
                configH << "    ESP_ERROR_CHECK(dac_continuous_enable(s_dac));\n";
                configH << "}\n\n";
                configH << "// Convert stereo float [-1.0, 1.0] to mono 8-bit [0,255] and stream to DAC via DMA\n";
                configH << "static inline uint8_t _to_u8(float x) {\n";
                configH << "    if (x > 1.0f) x = 1.0f;\n";
                configH << "    if (x < -1.0f) x = -1.0f;\n";
                configH << "    float scaled = (x * 0.5f + 0.5f) * 255.0f;\n";
                configH << "    return (scaled < 0.0f) ? 0 : (scaled > 255.0f ? 255 : (uint8_t)lroundf(scaled));\n";
                configH << "}\n\n";
                configH << "// Write a block of interleaved LR float frames to DAC (efficient burst writes)\n";
                configH << "static inline void to_audio_write_block(const float* interleavedLR, size_t frames) {\n";
                configH << "    for (size_t i = 0; i < frames; ++i) {\n";
                configH << "        uint8_t L = _to_u8(interleavedLR[2*i]);\n";
                configH << "        uint8_t R = _to_u8(interleavedLR[2*i+1]);\n";
                configH << "        if (LEFT_IS_CH0) { s_dac_buf[s_dac_idx++] = L; s_dac_buf[s_dac_idx++] = R; } else { s_dac_buf[s_dac_idx++] = R; s_dac_buf[s_dac_idx++] = L; }\n";
                configH << "        if (s_dac_idx >= sizeof(s_dac_buf)) {\n";
                configH << "            size_t loaded_total = 0;\n";
                configH << "            while (loaded_total < s_dac_idx) {\n";
                configH << "                size_t loaded = 0;\n";
                configH << "                ESP_ERROR_CHECK(dac_continuous_write(s_dac, s_dac_buf + loaded_total, s_dac_idx - loaded_total, &loaded, " << String(getValue<int>(dacWriteTimeoutValue) <= 0 ? "-1" : String(getValue<int>(dacWriteTimeoutValue))) << "));\n";
                configH << "                loaded_total += loaded;\n";
                configH << "            }\n";
                configH << "            s_dac_idx = 0;\n";
                configH << "        }\n";
                configH << "    }\n";
                configH << "}\n\n";
                configH << "// Single-frame convenience\n";
                configH << "static inline void to_audio_write(float L, float R) { float lr[2] = {L, R}; to_audio_write_block(lr, 1); }\n\n";
                configH << "#endif\n";
            } else {
                // External DAC via I2S with configurable pins and advanced options
                configH << "#ifndef CONFIG_H\n#define CONFIG_H\n\n";
                configH << "#include <stdint.h>\n";
                configH << "#include <math.h>\n";
                // Ensure i2s types are visible even if config.h is included before app_main.cpp
                configH << "#include \"driver/i2s_std.h\"\n\n";
                configH << "static i2s_chan_handle_t tx_handle;\n\n";
                configH << "void audio_init(uint32_t& sample_rate)\n{\n";
                // Channel config
                int portSel = getValue<int>(i2sPortValue);
                String portConst = (portSel == 2 ? "I2S_NUM_0" : (portSel == 3 ? "I2S_NUM_1" : "I2S_NUM_AUTO"));
                int roleSel = getValue<int>(i2sRoleValue);
                String roleConst = (roleSel == 2 ? "I2S_ROLE_SLAVE" : "I2S_ROLE_MASTER");
                configH << "    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(" << portConst << ", " << roleConst << ");\n\n";
                configH << "    i2s_new_channel(&chan_cfg, &tx_handle, NULL);\n";
                // Standard config
                // Build default structs then override from advanced options
                int dbw = getValue<int>(i2sDataBitWidthValue);
                String dbwConst = (dbw == 32 ? "I2S_DATA_BIT_WIDTH_32BIT" : (dbw == 24 ? "I2S_DATA_BIT_WIDTH_24BIT" : "I2S_DATA_BIT_WIDTH_16BIT"));
                int slotModeSel = getValue<int>(i2sSlotModeValue);
                String slotModeConst = (slotModeSel == 1 ? "I2S_SLOT_MODE_MONO" : "I2S_SLOT_MODE_STEREO");
                configH << "    i2s_std_config_t i2s_config = {\n";
                configH << "        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(sample_rate),\n";
                configH << "        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(" << dbwConst << ", " << slotModeConst << "),\n";
                // GPIO pins
                bool useMclk = getValue<bool>(i2sUseMclkValue);
                int mclkPin = getValue<int>(i2sMclkPinValue);
                int bclkPin = getValue<int>(i2sBclkPinValue);
                int wsPin = getValue<int>(i2sWsPinValue);
                int doutPin = getValue<int>(i2sDoutPinValue);
                bool useDin = getValue<bool>(i2sUseDinValue);
                int dinPin = getValue<int>(i2sDinPinValue);
                bool invM = getValue<bool>(i2sInvertMclkValue);
                bool invB = getValue<bool>(i2sInvertBclkValue);
                bool invW = getValue<bool>(i2sInvertWsValue);
                configH << "        .gpio_cfg = {\n";
                configH << "            .mclk = " << (useMclk ? (String("GPIO_NUM_") + String(mclkPin)) : String("I2S_GPIO_UNUSED")) << ",\n";
                configH << "            .bclk = GPIO_NUM_" << String(bclkPin) << ",\n";
                configH << "            .ws = GPIO_NUM_" << String(wsPin) << ",\n";
                configH << "            .dout = GPIO_NUM_" << String(doutPin) << ",\n";
                configH << "            .din = " << (useDin ? (String("GPIO_NUM_") + String(dinPin)) : String("I2S_GPIO_UNUSED")) << ",\n";
                configH << "            .invert_flags = { .mclk_inv = " << (invM ? "true" : "false") << ", .bclk_inv = " << (invB ? "true" : "false") << ", .ws_inv = " << (invW ? "true" : "false") << " },\n";
                configH << "        },\n";
                configH << "    };\n";
                // Override clock source and mclk multiple if advanced is enabled
                bool i2sAdv = getValue<bool>(i2sAdvancedOptionsValue);
                if (i2sAdv) {
                    int clkSel = getValue<int>(i2sClockSrcValue);
                    String clkSrcConst = (clkSel == 3 ? "I2S_CLK_SRC_APLL" : (clkSel == 2 ? "I2S_CLK_SRC_PLL_160M" : "I2S_CLK_SRC_DEFAULT"));
                    int mult = getValue<int>(i2sMclkMultipleValue);
                    String multConst;
                    if (mult == 128) multConst = "I2S_MCLK_MULTIPLE_128"; else if (mult == 192) multConst = "I2S_MCLK_MULTIPLE_192"; else if (mult == 256) multConst = "I2S_MCLK_MULTIPLE_256"; else if (mult == 384) multConst = "I2S_MCLK_MULTIPLE_384"; else if (mult == 512) multConst = "I2S_MCLK_MULTIPLE_512"; else if (mult == 576) multConst = "I2S_MCLK_MULTIPLE_576"; else if (mult == 768) multConst = "I2S_MCLK_MULTIPLE_768"; else if (mult == 1024) multConst = "I2S_MCLK_MULTIPLE_1024"; else multConst = "I2S_MCLK_MULTIPLE_1152";
                    configH << "    i2s_config.clk_cfg.clk_src = " << clkSrcConst << ";\n";
                    configH << "    i2s_config.clk_cfg.mclk_multiple = " << multConst << ";\n";
                }
                configH << "\n    i2s_channel_init_std_mode(tx_handle, &i2s_config);\n";
                configH << "    i2s_channel_enable(tx_handle);\n";
                configH << "}\n\n";
                configH << "// Write a block of interleaved LR float frames to I2S (packed per selected bit width)\n";
                configH << "static inline void to_audio_write_block(const float* interleavedLR, size_t frames)\n{\n";
                int wt = getValue<int>(i2sWriteTimeoutMsValue);
                String wtExpr = (wt <= 0 ? String("portMAX_DELAY") : String(wt));
                if (dbw == 24 || dbw == 32) {
                    configH << "    auto clampf = [](float x){ return x < -1.0f ? -1.0f : (x > 1.0f ? 1.0f : x); };\n";
                    configH << "    const float scale = " << (getValue<int>(i2sDataBitWidthValue) == 24 ? "8388607.0f" : "2147483647.0f") << ";\n";
                    configH << "    static int32_t packbuf[2*" << String(blockSize) << "];\n";
                    configH << "    size_t n = (frames > " << String(blockSize) << " ? " << String(blockSize) << " : frames);\n";
                    configH << "    for (size_t i = 0; i < n; ++i) {\n";
                    configH << "        int32_t L = (int32_t) lroundf(clampf(interleavedLR[2*i]) * scale);\n";
                    configH << "        int32_t R = (int32_t) lroundf(clampf(interleavedLR[2*i+1]) * scale);\n";
                    if (getValue<int>(i2sDataBitWidthValue) == 24) { configH << "        L <<= 8; R <<= 8;\n"; }
                    configH << "        packbuf[2*i] = L; packbuf[2*i+1] = R;\n";
                    configH << "    }\n";
                    configH << "    size_t total = n * sizeof(int32_t) * 2;\n";
                    configH << "    size_t written = 0;\n";
                    configH << "    uint8_t* ptr = (uint8_t*)packbuf;\n";
                    configH << "    while (written < total) { size_t chunk = 0; i2s_channel_write(tx_handle, ptr + written, total - written, &chunk, " << wtExpr << "); written += chunk; }\n";
                } else {
                    configH << "    auto clampf = [](float x){ return x < -1.0f ? -1.0f : (x > 1.0f ? 1.0f : x); };\n";
                    configH << "    static int16_t packbuf[2*" << String(blockSize) << "];\n";
                    configH << "    size_t n = (frames > " << String(blockSize) << " ? " << String(blockSize) << " : frames);\n";
                    configH << "    for (size_t i = 0; i < n; ++i) {\n";
                    configH << "        int16_t L = (int16_t) lroundf(clampf(interleavedLR[2*i]) * 32767.0f);\n";
                    configH << "        int16_t R = (int16_t) lroundf(clampf(interleavedLR[2*i+1]) * 32767.0f);\n";
                    configH << "        packbuf[2*i] = L; packbuf[2*i+1] = R;\n";
                    configH << "    }\n";
                    configH << "    size_t total = n * sizeof(int16_t) * 2;\n";
                    configH << "    size_t written = 0;\n";
                    configH << "    uint8_t* ptr = (uint8_t*)packbuf;\n";
                    configH << "    while (written < total) { size_t chunk = 0; i2s_channel_write(tx_handle, ptr + written, total - written, &chunk, " << wtExpr << "); written += chunk; }\n";
                }
                configH << "}\n\n";
                configH << "// Single-frame convenience\n";
                configH << "static inline void to_audio_write(float L, float R) { float lr[2] = {L, R}; to_audio_write_block(lr, 1); }\n\n";
                configH << "#endif\n";
            }
            mainDir.getChildFile("config.h").replaceWithText(configH);

            String appMain;
            appMain << "#include \"driver/i2s_std.h\"\n";
            appMain << "#include \"driver/adc.h\"\n";
            appMain << "#include \"esp_adc_cal.h\"\n";
            appMain << "#include \"freertos/FreeRTOS.h\"\n";
            appMain << "#include \"freertos/task.h\"\n";
            appMain << "#include \"driver/gpio.h\"\n";
            appMain << "#include \"esp_log.h\"\n";
            appMain << "#include \"config.h\"\n\n";
            appMain << "#include \"Heavy_Untitled.h\"\n\n";
            appMain << "static const char* TAG = \"plugdata_esp\";\n";
            appMain << "static const size_t AUDIO_BLOCK = " << String(blockSize) << ";\n";
            appMain << "static uint32_t sr = 48000;\n";
            appMain << "static HeavyContextInterface* hv_ctx = nullptr;\n\n";
            // Hardwired button: GPIO33 active-low with internal pull-up, bang-only
            appMain << "static const int BTN_PIN = 33;\n";
            appMain << "static const char* BTN_NAME = \"Button1\";\n";
            appMain << "static hv_uint32_t hv_btn_hash = 0;\n";
            appMain << "static volatile uint8_t btn_bang_dirty = 0;\n";
            appMain << "static volatile float btn_bang_value = 0.0f;\n";
            appMain << "static volatile int btn_prev_level = 1;\n";
            appMain << "static const TickType_t BTN_DEBOUNCE_TICKS = pdMS_TO_TICKS(20);\n";
            appMain << "static volatile TickType_t btn_last_tick = 0;\n\n";
            appMain << "static const int kAdcCount = " << String(adcCount) << ";\n";
            if (adcCount > 0) {
                String useArr("static const bool use_adc1[" + String(adcCount) + "] = { ");
                String a1Arr("static const adc1_channel_t adc1_ch[" + String(adcCount) + "] = { ");
                String a2Arr("static const adc2_channel_t adc2_ch[" + String(adcCount) + "] = { ");
                for (int i = 0; i < adcCount; ++i) {
                    int pin = getValue<int>(adcPinValues[i]);
                    if (pin <= 0) pin = 32;
                    bool useAdc1 = (pin == 32 || pin == 33 || pin == 34 || pin == 35 || pin == 36 || pin == 39);
                    String adc1ChConst = "ADC1_CHANNEL_4";
                    String adc2ChConst = "ADC2_CHANNEL_0";
                    if (pin == 36) adc1ChConst = "ADC1_CHANNEL_0";
                    else if (pin == 39) adc1ChConst = "ADC1_CHANNEL_3";
                    else if (pin == 32) adc1ChConst = "ADC1_CHANNEL_4";
                    else if (pin == 33) adc1ChConst = "ADC1_CHANNEL_5";
                    else if (pin == 34) adc1ChConst = "ADC1_CHANNEL_6";
                    else if (pin == 35) adc1ChConst = "ADC1_CHANNEL_7";
                    if (pin == 4) adc2ChConst = "ADC2_CHANNEL_0";
                    else if (pin == 0) adc2ChConst = "ADC2_CHANNEL_1";
                    else if (pin == 2) adc2ChConst = "ADC2_CHANNEL_2";
                    else if (pin == 15) adc2ChConst = "ADC2_CHANNEL_3";
                    else if (pin == 13) adc2ChConst = "ADC2_CHANNEL_4";
                    else if (pin == 12) adc2ChConst = "ADC2_CHANNEL_5";
                    else if (pin == 14) adc2ChConst = "ADC2_CHANNEL_6";
                    else if (pin == 27) adc2ChConst = "ADC2_CHANNEL_7";
                    else if (pin == 25) adc2ChConst = "ADC2_CHANNEL_8";
                    else if (pin == 26) adc2ChConst = "ADC2_CHANNEL_9";
                    useArr << (useAdc1 ? "true" : "false"); a1Arr << adc1ChConst; a2Arr << adc2ChConst;
                    if (i < adcCount - 1) { useArr << ", "; a1Arr << ", "; a2Arr << ", "; }
                }
                useArr << " };\n"; a1Arr << " };\n"; a2Arr << " };\n";
                appMain << useArr << a1Arr << a2Arr;
                String names("static const char* HV_PARAM_NAME[" + String(adcCount) + "] = { ");
                for (int i = 0; i < adcCount; ++i) {
                    String nm = getValue<String>(adcNameValues[i]);
                    if (nm.isEmpty()) nm = String("Knob") + String(i + 1);
                    names << nm.quoted(); if (i < adcCount - 1) names << ", ";
                }
                names << " };\n";
                appMain << names;
                appMain << "static hv_uint32_t hv_param_hash[" << String(adcCount) << "] = { 0 } ;\n";
                appMain << "static float hv_param_min[" << String(adcCount) << "] = { 0 } ;\n";
                appMain << "static float hv_param_max[" << String(adcCount) << "] = { 1 } ;\n";
                appMain << "static float pot_smooth[" << String(adcCount) << "] = { 0 } ;\n";
            }
            appMain << "static const float pot_alpha = 0.1f;\n";
            appMain << "static const adc_atten_t POT_ATTEN = ADC_ATTEN_DB_12;\n";
            appMain << "static const adc_bits_width_t POT_WIDTH = ADC_WIDTH_BIT_12;\n";
            appMain << "static const uint32_t DEFAULT_VREF_MV = 1100;\n";
            appMain << "static esp_adc_cal_characteristics_t adc_chars;\n\n";
            if (adcCount > 0) {
                appMain << "static void adc_init()\n{ adc1_config_width(POT_WIDTH); for (int i = 0; i < kAdcCount; ++i) { if (use_adc1[i]) adc1_config_channel_atten(adc1_ch[i], POT_ATTEN); else adc2_config_channel_atten(adc2_ch[i], POT_ATTEN); } }\n\n";
                appMain << "static float pot_read_norm(int idx)\n{ const int samples = 8; uint32_t acc_raw = 0; for (int s = 0; s < samples; ++s) { if (use_adc1[idx]) acc_raw += adc1_get_raw(adc1_ch[idx]); else { int v = 0; adc2_get_raw(adc2_ch[idx], POT_WIDTH, &v); acc_raw += (uint32_t)v; } } uint32_t raw = acc_raw / samples; float norm = (float)raw / 4095.0f; if (norm < 0.0f) norm = 0.0f; if (norm > 1.0f) norm = 1.0f; pot_smooth[idx] += pot_alpha * (norm - pot_smooth[idx]); return pot_smooth[idx]; }\n\n";
                appMain << "static volatile float pot_norm_latest[" << String(adcCount) << "] = { 0 } ;\n";
                appMain << "static volatile uint8_t pot_dirty[" << String(adcCount) << "] = { 0 } ;\n";
                int pollMs = jlimit(1, 100, getValue<int>(adcPollMsValue));
                appMain << "static void ctrl_task(void* arg)\n{ (void)arg; int idx = 0; const TickType_t tick = pdMS_TO_TICKS(" << String(pollMs) << "); while (1) { pot_norm_latest[idx] = pot_read_norm(idx); pot_dirty[idx] = 1; idx = (idx + 1) % kAdcCount; int level = gpio_get_level((gpio_num_t) BTN_PIN); TickType_t now = xTaskGetTickCount(); if (level != btn_prev_level && (now - btn_last_tick) >= BTN_DEBOUNCE_TICKS) { btn_last_tick = now; if (btn_prev_level == 1 && level == 0) { btn_prev_level = 0; ESP_LOGI(TAG, \"Button1 DOWN\"); btn_bang_value = 1.0f; btn_bang_dirty = 1; } else if (btn_prev_level == 0 && level == 1) { btn_prev_level = 1; ESP_LOGI(TAG, \"Button1 UP\"); } } vTaskDelay(tick); } }\n\n";
            } else {
                appMain << "static void adc_init(){}\n\n";
                int pollMsNoAdc = jlimit(1, 100, getValue<int>(adcPollMsValue));
                appMain << "static void ctrl_task(void* arg)\n{ (void)arg; const TickType_t tick = pdMS_TO_TICKS(" << String(pollMsNoAdc) << "); while (1) { int level = gpio_get_level((gpio_num_t) BTN_PIN); TickType_t now = xTaskGetTickCount(); if (level != btn_prev_level && (now - btn_last_tick) >= BTN_DEBOUNCE_TICKS) { btn_last_tick = now; if (btn_prev_level == 1 && level == 0) { btn_prev_level = 0; ESP_LOGI(TAG, \"Button1 DOWN\"); btn_bang_value = 1.0f; btn_bang_dirty = 1; } else if (btn_prev_level == 0 && level == 1) { btn_prev_level = 1; ESP_LOGI(TAG, \"Button1 UP\"); } } vTaskDelay(tick); } }\n\n";
            }
            appMain << "static void audio_callback()\n{ static float outLR[2*AUDIO_BLOCK]; hv_processInlineInterleaved(hv_ctx, nullptr, outLR, AUDIO_BLOCK); to_audio_write_block(outLR, AUDIO_BLOCK); }\n\n";
            appMain << "extern \"C\" void app_main(void)\n{ audio_init(sr); hv_ctx = hv_Untitled_new(static_cast<double>(sr)); ";
            if (adcCount > 0) {
                appMain << "for (int i = 0; i < kAdcCount; ++i) hv_param_hash[i] = hv_stringToHash(HV_PARAM_NAME[i]); int total = hv_getParameterInfo(hv_ctx, 0, NULL); hv_uint32_t fallback_hash = 0; float fb_min = 0.f, fb_max = 1.f; for (int p = 0; p < total; ++p) { HvParameterInfo info; hv_getParameterInfo(hv_ctx, p, &info); for (int i = 0; i < kAdcCount; ++i) { if (info.hash == hv_param_hash[i]) { hv_param_min[i] = info.minVal; hv_param_max[i] = info.maxVal; } } if (info.type == HV_PARAM_TYPE_PARAMETER_IN && fallback_hash == 0) { fallback_hash = info.hash; fb_min = info.minVal; fb_max = info.maxVal; } } for (int i = 0; i < kAdcCount; ++i) { if (hv_param_hash[i] == 0 && fallback_hash != 0) { hv_param_hash[i] = fallback_hash; hv_param_min[i] = fb_min; hv_param_max[i] = fb_max; } } ";
            }
            appMain << "adc_init();\n";
            appMain << "hv_btn_hash = hv_stringToHash(BTN_NAME);\n";
            appMain << "{ gpio_config_t io = {}; io.intr_type = GPIO_INTR_DISABLE; io.mode = GPIO_MODE_INPUT; io.pin_bit_mask = (1ULL << BTN_PIN); io.pull_down_en = GPIO_PULLDOWN_DISABLE; io.pull_up_en = GPIO_PULLUP_ENABLE; gpio_config(&io); gpio_pullup_en((gpio_num_t)BTN_PIN); btn_prev_level = gpio_get_level((gpio_num_t) BTN_PIN); }\n";
            appMain << "#if CONFIG_FREERTOS_UNICORE\n";
            appMain << "    xTaskCreate(ctrl_task, \"ctrl\", 4096, NULL, 4, NULL);\n";
            appMain << "#else\n";
            appMain << "    xTaskCreatePinnedToCore(ctrl_task, \"ctrl\", 4096, NULL, 4, NULL, 0);\n";
            appMain << "#endif\n";
            appMain << "    while (1) { ";
            if (adcCount > 0) appMain << "{ for (int i = 0; i < kAdcCount; ++i) { if (pot_dirty[i]) { pot_dirty[i] = 0; float norm = pot_norm_latest[i]; float mapped = hv_param_min[i] + norm * (hv_param_max[i] - hv_param_min[i]); if (hv_param_hash[i] != 0) hv_sendFloatToReceiver(hv_ctx, hv_param_hash[i], mapped); break; } } } ";
            appMain << "if (btn_bang_dirty) { btn_bang_dirty = 0; hv_sendBangToReceiver(hv_ctx, hv_btn_hash); } ";
            appMain << "audio_callback(); } }\n";
            mainDir.getChildFile("app_main.cpp").replaceWithText(appMain);
        }

        // 2) Run hvcc to generate Heavy C into temp project (force name to Untitled)
        if (exportingView)
            exportingView->logToConsole("ESP32: running hvcc...\nProject: " + tempProjRoot.getFullPathName() + "\n");

#if JUCE_WINDOWS
        auto const heavyPath = heavyExecutable.getFullPathName().replaceCharacter('\\', '/');
#else
        auto const heavyPath = heavyExecutable.getFullPathName();
#endif
    StringArray args = { heavyPath.quoted(), pdPatch.quoted(), "-o", tempProjRoot.getFullPathName().quoted() };
        args.add("-nUntitled");

        if (copyright.isNotEmpty()) {
            args.add("--copyright");
            args.add(copyright.quoted());
        }

        args.add("-v");
        args.add("-p");
        for (auto& path : searchPaths) {
            args.add(path);
        }

        if (shouldQuit) return true;
        auto const command = args.joinIntoString(" ");
        exportingView->logToConsole("Command: " + command + "\n");
        Toolchain::startShellScript(command, this);
        waitForProcessToFinish(-1);
        exportingView->flushConsole();
        if (shouldQuit) return true;

        // 3) Patch HvMessage.c formatting inside temp project c/
        File targetDir = cDir;

        // 3.5) Patch HvMessage.c formatting
    if (exportingView) exportingView->logToConsole("Starting (2/4) patching HvMessage formatting if needed\n");
        {
            auto hvMessageFile = targetDir.getChildFile("HvMessage.c");
            if (hvMessageFile.existsAsFile()) {
                auto content = hvMessageFile.loadFileAsString();
                bool changed = false;
                if (! content.contains("<inttypes.h>")) {
                    auto anchor = String("#include \"HvMessage.h\"");
                    int pos = content.indexOf(anchor);
                    if (pos >= 0) {
                        pos += anchor.length();
                        content = content.substring(0, pos) + "\n#include <inttypes.h>" + content.substring(pos);
                        changed = true;
                    }
                }
                if (content.contains("\"0x%X\"")) {
                    content = content.replace("\"0x%X\"", "\"0x%\" PRIX32");
                    changed = true;
                }
                if (changed) {
                    hvMessageFile.replaceWithText(content);
                    if (exportingView) exportingView->logToConsole("HvMessage.c patch applied successfully.\n");
                } else {
                    if (exportingView) exportingView->logToConsole("HvMessage.c already compatible, no patch needed.\n");
                }
            } else {
                if (exportingView) exportingView->logToConsole("Warning: HvMessage.c not found in " + targetDir.getFullPathName() + "\n");
            }
        }

        // 3.6) Update main/CMakeLists.txt SRCS: skip when project uses globbing
    if (exportingView) exportingView->logToConsole("Starting (4/4) sanity-check Heavy sources\n");
        {
            auto cmakeMain = mainDir.getChildFile("CMakeLists.txt");
            if (cmakeMain.existsAsFile()) {
                auto cmakeText = cmakeMain.loadFileAsString();
                if (cmakeText.contains("file(GLOB hvcc_c")) {
                    if (exportingView) exportingView->logToConsole("Detected globbing of ../c in CMake; SRCS already sync automatically.\n");
                } else {
                    int srci = cmakeText.indexOf("SRCS");
                    int incli = cmakeText.indexOf(srci + 1, "INCLUDE_DIRS");
                    if (srci >= 0 && incli > srci) {
                        String srcSection = cmakeText.substring(srci, incli);
                        StringArray candidates;
                        if (targetDir.isDirectory()) {
                            for (auto entry : RangedDirectoryIterator(cDir, false, "*", File::findFiles)) {
                                auto f = entry.getFile();
                                auto ext = f.getFileExtension();
                                if (ext.equalsIgnoreCase(".c") || ext.equalsIgnoreCase(".cpp")) {
                                    candidates.add("../c/" + f.getFileName());
                                }
                            }
                        }
                        StringArray toAdd;
                        for (auto& rel : candidates) {
                            if (! srcSection.contains(rel)) toAdd.add(rel);
                        }
                        if (toAdd.isEmpty()) {
                            if (exportingView) exportingView->logToConsole("No new files to add to SRCS.\n");
                        } else {
                            String addBuf;
                            for (auto& rel : toAdd) {
                                addBuf += "         \"";
                                addBuf += rel;
                                addBuf += "\"\n";
                            }
                            cmakeText = cmakeText.substring(0, incli) + addBuf + cmakeText.substring(incli);
                            cmakeMain.replaceWithText(cmakeText);
                            if (exportingView) exportingView->logToConsole("Added " + String(toAdd.size()) + " hvcc sources to SRCS.\n");
                        }
                    } else {
                        if (exportingView) exportingView->logToConsole("Warning: could not locate SRCS/INCLUDE_DIRS section in CMakeLists.txt.\n");
                    }
                }
            } else {
                if (exportingView) exportingView->logToConsole("Warning: main/CMakeLists.txt not found for sanity-check.\n");
            }
        }

        // Clean hvcc intermediates
        tempProjRoot.getChildFile("ir").deleteRecursively();
        tempProjRoot.getChildFile("hv").deleteRecursively();
        int hvccExit = getExitCode();

        // 4) Source ESP-IDF, build and flash the temp project
    if (exportingView) exportingView->logToConsole("ESP32: preparing environment, building and flashing temporary project...\n");
    exportingView->showState(ExportingProgressView::Flashing);

#if JUCE_WINDOWS
    int flashExit = 0;
#else
    // Prefer incremental builds: set IDF target via -D to avoid fullclean, enable ccache, and rely on Ninja/CMake to recompile only changed sources
    String shellCmd = String(". \"$HOME/esp/esp-idf/export.sh\" >/dev/null 2>&1; ")
            + "cd " + tempProjRoot.getFullPathName().quoted() + "; "
            + "export CMAKE_BUILD_PARALLEL_LEVEL=$(nproc); "
            + "idf.py --ccache -DIDF_TARGET=esp32 app-flash";

    StringArray argv;
    argv.add("/usr/bin/zsh");
    argv.add("-lc");
    argv.add(shellCmd);
    bool started = start(argv);
    waitForProcessToFinish(-1);
    exportingView->flushConsole();
    int flashExit = started ? getExitCode() : 1;
#endif

        // Optional: log bin path if present in temp project
        {
            File binPath = tempProjRoot.getChildFile("build").getChildFile(projectName + ".bin");
            if (binPath.existsAsFile()) {
                exportingView->logToConsole("ESP32: generated bin: " + binPath.getFullPathName() + " (" + String(binPath.getSize()) + " bytes)\n");
            }
        }

        // Keep project directory to allow incremental builds and reuse of ccache/build artifacts.
        if (flashExit != 0) {
            exportingView->logToConsole("Flash failed; keeping project directory for troubleshooting: " + tempProjRoot.getFullPathName() + "\n");
        }

        Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 300);
    return (hvccExit != 0) || (flashExit != 0);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ESP32Exporter)
};