#include "driver/i2s_std.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "config.h"

// Heavy DSP (hvcc outputs from c/)
#include "Heavy_Untitled.h"

// Global sample rate and Heavy context
static uint32_t sr = 48000;
static HeavyContextInterface* hv_ctx = nullptr;

// ADC configuration (single potentiometer on ADC1_CHANNEL_5 = GPIO33 by default)
static const adc1_channel_t POT_ADC_CH = ADC1_CHANNEL_5; // GPIO33
static const adc_atten_t POT_ATTEN = ADC_ATTEN_DB_12;     // new alias; behaves same as 11 dB on ESP32
static const adc_bits_width_t POT_WIDTH = ADC_WIDTH_BIT_12; // 12-bit resolution
static const uint32_t DEFAULT_VREF_MV = 1100; // used if no eFuse calibration
static esp_adc_cal_characteristics_t adc_chars;

// Heavy parameter mapping (receiver name to control)
static const char* HV_PARAM_NAME = "Knob1"; // Pure Data receiver name marked with @hv_param
static hv_uint32_t hv_param_hash = 0;      // resolved at runtime
static float hv_param_min = 0.0f;
static float hv_param_max = 1.0f;

// Simple smoothing for noisy ADC (one-pole low-pass)
static float pot_norm_smooth = 0.0f;
static const float pot_alpha = 0.1f; // smoothing factor (0..1), smaller = smoother

static void adc_init()
{
    // Configure ADC1 width and attenuation for the chosen channel
    adc1_config_width(POT_WIDTH);
    adc1_config_channel_atten(POT_ADC_CH, POT_ATTEN);

    // Characterize ADC (calibration curve)
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, POT_ATTEN, POT_WIDTH, DEFAULT_VREF_MV, &adc_chars);
    (void)val_type; // optional: could log calibration source
}

// Read potentiometer, return normalized [0.0, 1.0] value with simple smoothing
static float pot_read_norm()
{
    // Multi-sampling to reduce noise
    const int samples = 16;
    uint32_t acc_raw = 0;
    for (int i = 0; i < samples; i++) {
        acc_raw += adc1_get_raw(POT_ADC_CH);
    }
    uint32_t raw = acc_raw / samples;

    // Convert to millivolts using calibration
    uint32_t mv = esp_adc_cal_raw_to_voltage(raw, &adc_chars);

    // Normalize by expected max for 11 dB attenuation (~2450 mV)
    float norm = (float)mv / 2450.0f;
    if (norm < 0.0f) norm = 0.0f;
    if (norm > 1.0f) norm = 1.0f;

    // Apply smoothing
    pot_norm_smooth += pot_alpha * (norm - pot_norm_smooth);
    return pot_norm_smooth;
}

// Audio callback: process one stereo frame through Heavy and write to I2S
static void audio_callback()
{
    float outLR[2] = {0.f, 0.f};
    // Heavy patch has 0 inputs and 2 outputs; process one frame interleaved
    hv_processInlineInterleaved(hv_ctx, nullptr, outLR, 1);
    to_audio_write(outLR[0], outLR[1]);
}

extern "C" void app_main(void)
{
    // Initialize audio driver
    audio_init(sr);

    // Initialize Heavy context
    hv_ctx = hv_Untitled_new(static_cast<double>(sr));

    // Resolve receiver hash for the chosen parameter name
    hv_param_hash = hv_stringToHash(HV_PARAM_NAME);
    // Try to find parameter range via hv_getParameterInfo; fallback to first PARAMETER_IN if name not found
    int total = hv_getParameterInfo(hv_ctx, 0, NULL);
    hv_uint32_t fallback_hash = 0;
    for (int i = 0; i < total; i++) {
        HvParameterInfo info;
        hv_getParameterInfo(hv_ctx, i, &info);
        if (info.hash == hv_param_hash) {
            hv_param_min = info.minVal;
            hv_param_max = info.maxVal;
            fallback_hash = 0;
            break;
        }
        if (fallback_hash == 0 && info.type == HV_PARAM_TYPE_PARAMETER_IN) {
            fallback_hash = info.hash;
            hv_param_min = info.minVal;
            hv_param_max = info.maxVal;
        }
    }
    if (hv_param_hash == 0 && fallback_hash != 0) {
        hv_param_hash = fallback_hash;
    }

    // Initialize ADC for potentiometer
    adc_init();

    // Main audio loop
    while (1)
    {
        // Read pot at a lower rate to minimize overhead (every N samples)
        static int ctr = 0;
        if ((ctr++ & 0xFF) == 0) { // roughly every 256 iterations
            float norm = pot_read_norm();
            // Map normalized [0..1] into the PD parameter's min/max if available
            float mapped = hv_param_min + norm * (hv_param_max - hv_param_min);
            if (hv_param_hash != 0) {
                hv_sendFloatToReceiver(hv_ctx, hv_param_hash, mapped);
            }
        }

        audio_callback();
    }
}

