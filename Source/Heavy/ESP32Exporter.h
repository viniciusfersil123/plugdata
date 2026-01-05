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
    TextButton flashButton = TextButton("Flash");
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

    ESP32Exporter(PluginEditor* editor, ExportingProgressView* exportingView)
        : ExporterBase(editor, exportingView)
    {
        // ESP32-specific options section
        {
            PropertiesArray properties;
            properties.add(new PropertiesPanel::ComboComponent("Audio Output", audioOutputValue, { "ESP32 DAC", "External DAC" }));
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
            for (auto* property : properties) property->setPreferredHeight(28);
            panel.addSection("ESP32", properties);
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
    }

    void valueChanged(Value& v) override {
        // Preserve base behavior (patch selection etc.)
        ExporterBase::valueChanged(v);

        // Show/hide sections based on audio output and advanced toggle
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
                configH << "    dac_continuous_config_t cfg = {\n";
                configH << "        .chan_mask = (dac_channel_mask_t)((LEFT_IS_CH0 ? DAC_CHANNEL_MASK_CH0 : DAC_CHANNEL_MASK_CH1) | (LEFT_IS_CH0 ? DAC_CHANNEL_MASK_CH1 : DAC_CHANNEL_MASK_CH0)),\n";
                configH << "        .desc_num = " << String(descNum) << ",\n";
                configH << "        .buf_size = " << String(bufSize) << ",\n";
                configH << "        .freq_hz = sample_rate,\n";
                configH << "        .offset = " << String(offset) << ",\n";
                configH << "        .clk_src = " << clkConst << ",\n";
                configH << "        .chan_mode = " << modeConst << ",\n";
                configH << "    };\n\n";
                configH << "    ESP_ERROR_CHECK(dac_continuous_new_channels(&cfg, &s_dac));\n";
                configH << "    ESP_ERROR_CHECK(dac_continuous_enable(s_dac));\n";
                configH << "}\n\n";
                configH << "// Convert stereo float [-1.0, 1.0] to mono 8-bit [0,255] and stream to DAC via DMA\n";
                configH << "void to_audio_write(float left_channel, float right_channel)\n";
                configH << "{\n";
                configH << "    // Map stereo [-1,1] to 8-bit [0,255] and interleave as required by DAC alternating mode\n";
                configH << "    auto to_u8 = [](float x) -> uint8_t {\n";
                configH << "        if (x > 1.0f) x = 1.0f;\n";
                configH << "        if (x < -1.0f) x = -1.0f;\n";
                configH << "        float scaled = (x * 0.5f + 0.5f) * 255.0f;\n";
                configH << "        return (scaled < 0.0f) ? 0 : (scaled > 255.0f ? 255 : (uint8_t)lroundf(scaled));\n";
                configH << "    };\n\n";
                configH << "    uint8_t L = to_u8(left_channel);\n";
                configH << "    uint8_t R = to_u8(right_channel);\n\n";
                configH << "    if (LEFT_IS_CH0) {\n";
                configH << "        s_dac_buf[s_dac_idx++] = L;\n";
                configH << "        s_dac_buf[s_dac_idx++] = R;\n";
                configH << "    } else {\n";
                configH << "        s_dac_buf[s_dac_idx++] = R;\n";
                configH << "        s_dac_buf[s_dac_idx++] = L;\n";
                configH << "    }\n\n";
                configH << "    if (s_dac_idx >= sizeof(s_dac_buf)) {\n";
                configH << "        size_t loaded = 0;\n";
                int writeTimeout = getValue<int>(dacWriteTimeoutValue);
                configH << "        ESP_ERROR_CHECK(dac_continuous_write(s_dac, s_dac_buf, s_dac_idx, &loaded, " << String(writeTimeout) << "));\n";
                configH << "        (void)loaded;\n";
                configH << "        s_dac_idx = 0;\n";
                configH << "    }\n";
                configH << "}\n\n";
                configH << "#endif\n";
            } else {
                // External DAC via I2S (existing behaviour)
                configH << "#ifndef CONFIG_H\n#define CONFIG_H\n\n";
                configH << "#include <stdint.h>\n\n";
                configH << "static i2s_chan_handle_t tx_handle;\n\n";
                configH << "void audio_init(uint32_t& sample_rate)\n{\n";
                configH << "    static const i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(\n";
                configH << "        I2S_NUM_AUTO,\n        I2S_ROLE_MASTER\n    );\n\n";
                configH << "    static const i2s_std_config_t i2s_config = {\n";
                configH << "        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(sample_rate),\n";
                configH << "        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(\n";
                configH << "            I2S_DATA_BIT_WIDTH_16BIT,\n            I2S_SLOT_MODE_STEREO\n        ),\n";
                configH << "        .gpio_cfg = {\n";
                configH << "            .mclk = I2S_GPIO_UNUSED,\n";
                configH << "            .bclk = GPIO_NUM_27,\n";
                configH << "            .ws = GPIO_NUM_26,\n";
                configH << "            .dout = GPIO_NUM_25,\n";
                configH << "            .din = I2S_GPIO_UNUSED,\n";
                configH << "            .invert_flags = { .mclk_inv = false, .bclk_inv = false, .ws_inv = false },\n";
                configH << "        },\n";
                configH << "    };\n\n";
                configH << "    i2s_new_channel(&chan_cfg, &tx_handle, NULL);\n";
                configH << "    i2s_channel_init_std_mode(tx_handle, &i2s_config);\n";
                configH << "    i2s_channel_enable(tx_handle);\n";
                configH << "}\n\n";
                configH << "void to_audio_write(float left_channel, float right_channel)\n{\n";
                configH << "    int16_t L = static_cast<int16_t>(left_channel * 32767.0f * 0.5f);\n";
                configH << "    int16_t R = static_cast<int16_t>(right_channel * 32767.0f * 0.5f);\n";
                configH << "    int16_t buf[2] = { L, R };\n";
                configH << "    size_t bytes = 0;\n";
                configH << "    i2s_channel_write(tx_handle, buf, sizeof(buf), &bytes, portMAX_DELAY);\n";
                configH << "}\n\n";
                configH << "#endif\n";
            }
            mainDir.getChildFile("config.h").replaceWithText(configH);

            String appMain;
            appMain << "#include \"driver/i2s_std.h\"\n";
            appMain << "#include \"driver/adc.h\"\n";
            appMain << "#include \"esp_adc_cal.h\"\n";
            appMain << "#include \"freertos/FreeRTOS.h\"\n";
            appMain << "#include \"config.h\"\n\n";
            appMain << "#include \"Heavy_Untitled.h\"\n\n";
            appMain << "static uint32_t sr = 48000;\n";
            appMain << "static HeavyContextInterface* hv_ctx = nullptr;\n\n";
            appMain << "static const adc1_channel_t POT_ADC_CH = ADC1_CHANNEL_5;\n";
            appMain << "static const adc_atten_t POT_ATTEN = ADC_ATTEN_DB_12;\n";
            appMain << "static const adc_bits_width_t POT_WIDTH = ADC_WIDTH_BIT_12;\n";
            appMain << "static const uint32_t DEFAULT_VREF_MV = 1100;\n";
            appMain << "static esp_adc_cal_characteristics_t adc_chars;\n\n";
            appMain << "static const char* HV_PARAM_NAME = \"Knob1\";\n";
            appMain << "static hv_uint32_t hv_param_hash = 0;\n";
            appMain << "static float hv_param_min = 0.0f;\n";
            appMain << "static float hv_param_max = 1.0f;\n\n";
            appMain << "static float pot_norm_smooth = 0.0f;\n";
            appMain << "static const float pot_alpha = 0.1f;\n\n";
            appMain << "static void adc_init()\n{\n";
            appMain << "    adc1_config_width(POT_WIDTH);\n";
            appMain << "    adc1_config_channel_atten(POT_ADC_CH, POT_ATTEN);\n";
            appMain << "    (void) esp_adc_cal_characterize(ADC_UNIT_1, POT_ATTEN, POT_WIDTH, DEFAULT_VREF_MV, &adc_chars);\n";
            appMain << "}\n\n";
            appMain << "static float pot_read_norm()\n{\n";
            appMain << "    const int samples = 16;\n";
            appMain << "    uint32_t acc_raw = 0;\n";
            appMain << "    for (int i = 0; i < samples; i++) acc_raw += adc1_get_raw(POT_ADC_CH);\n";
            appMain << "    uint32_t raw = acc_raw / samples;\n";
            appMain << "    uint32_t mv = esp_adc_cal_raw_to_voltage(raw, &adc_chars);\n";
            appMain << "    float norm = (float)mv / 2450.0f;\n";
            appMain << "    if (norm < 0.0f) { norm = 0.0f; }\n";
            appMain << "    if (norm > 1.0f) { norm = 1.0f; }\n";
            appMain << "    pot_norm_smooth += pot_alpha * (norm - pot_norm_smooth);\n";
            appMain << "    return pot_norm_smooth;\n";
            appMain << "}\n\n";
            appMain << "static void audio_callback()\n{\n";
            appMain << "    float outLR[2] = {0.f, 0.f};\n";
            appMain << "    hv_processInlineInterleaved(hv_ctx, nullptr, outLR, 1);\n";
            appMain << "    to_audio_write(outLR[0], outLR[1]);\n";
            appMain << "}\n\n";
            appMain << "extern \"C\" void app_main(void)\n{\n";
            appMain << "    audio_init(sr);\n";
            appMain << "    hv_ctx = hv_Untitled_new(static_cast<double>(sr));\n";
            appMain << "    hv_param_hash = hv_stringToHash(HV_PARAM_NAME);\n";
            appMain << "    int total = hv_getParameterInfo(hv_ctx, 0, NULL);\n";
            appMain << "    hv_uint32_t fallback_hash = 0;\n";
            appMain << "    for (int i = 0; i < total; i++) { HvParameterInfo info; hv_getParameterInfo(hv_ctx, i, &info);\n";
            appMain << "        if (info.hash == hv_param_hash) { hv_param_min = info.minVal; hv_param_max = info.maxVal; fallback_hash = 0; break; }\n";
            appMain << "        if (fallback_hash == 0 && info.type == HV_PARAM_TYPE_PARAMETER_IN) { fallback_hash = info.hash; hv_param_min = info.minVal; hv_param_max = info.maxVal; } }\n";
            appMain << "    if (hv_param_hash == 0 && fallback_hash != 0) hv_param_hash = fallback_hash;\n";
            appMain << "    adc_init();\n";
            appMain << "    while (1) { static int ctr = 0; if ((ctr++ & 0xFF) == 0) { float norm = pot_read_norm(); float mapped = hv_param_min + norm * (hv_param_max - hv_param_min); if (hv_param_hash != 0) hv_sendFloatToReceiver(hv_ctx, hv_param_hash, mapped); } audio_callback(); }\n";
            appMain << "}\n";
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