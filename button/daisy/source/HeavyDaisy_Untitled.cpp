/**
 * Copyright (c) 2026 Enzien Audio, Ltd.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the phrase "powered by heavy",
 *    the heavy logo, and a hyperlink to https://enzienaudio.com, all in a visible
 *    form.
 * 
 *   2.1 If the Application is distributed in a store system (for example,
 *       the Apple "App Store" or "Google Play"), the phrase "powered by heavy"
 *       shall be included in the app description or the copyright text as well as
 *       the in the app itself. The heavy logo will shall be visible in the app
 *       itself as well.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "Heavy_Untitled.h"
#include "Heavy_Untitled.hpp"
#include "HeavyDaisy_Untitled.hpp"

#define SAMPLE_RATE 48000.f

#define HV_HASH_NOTEIN          0x67E37CA3
#define HV_HASH_CTLIN           0x41BE0f9C
#define HV_HASH_POLYTOUCHIN     0xBC530F59
#define HV_HASH_PGMIN           0x2E1EA03D
#define HV_HASH_TOUCHIN         0x553925BD
#define HV_HASH_BENDIN          0x3083F0F7
#define HV_HASH_MIDIIN          0x149631bE
#define HV_HASH_MIDIREALTIMEIN  0x6FFF0BCF

#define HV_HASH_NOTEOUT         0xD1D4AC2
#define HV_HASH_CTLOUT          0xE5e2A040
#define HV_HASH_POLYTOUCHOUT    0xD5ACA9D1
#define HV_HASH_PGMOUT          0x8753E39E
#define HV_HASH_TOUCHOUT        0x476D4387
#define HV_HASH_BENDOUT         0xE8458013
#define HV_HASH_MIDIOUT         0x6511DE55
#define HV_HASH_MIDIOUTPORT     0x165707E4

#define MIDI_RT_CLOCK           0xF8
#define MIDI_RT_START           0xFA
#define MIDI_RT_CONTINUE        0xFB
#define MIDI_RT_STOP            0xFC
#define MIDI_RT_ACTIVESENSE     0xFE
#define MIDI_RT_RESET           0xFF

#define MIDI_OUT_FIFO_SIZE      128


using namespace daisy;

json2daisy::DaisyPod hardware;

Heavy_Untitled* hv;

void audiocallback(daisy::AudioHandle::InputBuffer in, daisy::AudioHandle::OutputBuffer out, size_t size);
static void sendHook(HeavyContextInterface *c, const char *receiverName, uint32_t receiverHash, const HvMessage * m);
FIFO<uint8_t, MIDI_OUT_FIFO_SIZE> midi_tx_fifo;
// int midiOutCount;
// uint8_t* midiOutData;
void CallbackWriteIn(Heavy_Untitled* hv);
void LoopWriteIn(Heavy_Untitled* hv);
void CallbackWriteOut();
void LoopWriteOut();
void PostProcess();
void Display();



// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
  ScopedIrqBlocker block; //< Disables interrupts while in scope

  for (int i = 0; i <= 2; ++i) {
    hv->sendMessageToReceiverV(HV_HASH_MIDIIN, 0, "ff",
    (float) m.data[i],
    (float) m.channel);
  }

  switch(m.type)
  {
    case SystemRealTime: {
      float srtType;

      switch(m.srt_type)
      {
        case TimingClock:
          srtType = MIDI_RT_CLOCK;
          break;
        case Start:
          srtType = MIDI_RT_START;
          break;
        case Continue:
          srtType = MIDI_RT_CONTINUE;
          break;
        case Stop:
          srtType = MIDI_RT_STOP;
          break;
        case ActiveSensing:
          srtType = MIDI_RT_ACTIVESENSE;
          break;
        case Reset:
          srtType = MIDI_RT_RESET;
          break;
      }

      hv->sendMessageToReceiverV(HV_HASH_MIDIREALTIMEIN, 0, "ff",
        (float) srtType);
      break;
    }
    case NoteOff: {
      NoteOnEvent p = m.AsNoteOn();
      hv->sendMessageToReceiverV(HV_HASH_NOTEIN, 0, "fff",
        (float) p.note, // pitch
        (float) 0, // velocity
        (float) p.channel);
      break;
    }
    case NoteOn: {
      NoteOnEvent p = m.AsNoteOn();
      hv->sendMessageToReceiverV(HV_HASH_NOTEIN, 0, "fff",
        (float) p.note, // pitch
        (float) p.velocity, // velocity
        (float) p.channel);
      break;
    }
    case PolyphonicKeyPressure: { // polyphonic aftertouch
      PolyphonicKeyPressureEvent p = m.AsPolyphonicKeyPressure();
      hv->sendMessageToReceiverV(HV_HASH_POLYTOUCHIN, 0, "fff",
        (float) p.pressure, // pressure
        (float) p.note, // note
        (float) p.channel);
      break;
    }
    case ControlChange: {
      ControlChangeEvent p = m.AsControlChange();
      hv->sendMessageToReceiverV(HV_HASH_CTLIN, 0, "fff",
        (float) p.value, // value
        (float) p.control_number, // cc number
        (float) p.channel);
      break;
    }
    case ProgramChange: {
      ProgramChangeEvent p = m.AsProgramChange();
      hv->sendMessageToReceiverV(HV_HASH_PGMIN, 0, "ff",
        (float) p.program,
        (float) p.channel);
      break;
    }
    case ChannelPressure: {
      ChannelPressureEvent p = m.AsChannelPressure();
      hv->sendMessageToReceiverV(HV_HASH_TOUCHIN, 0, "ff",
        (float) p.pressure,
        (float) p.channel);
      break;
    }
    case PitchBend: {
      PitchBendEvent p = m.AsPitchBend();
      // combine 7bit lsb and msb into 32bit int
      hv_uint32_t value = (((hv_uint32_t) m.data[1]) << 7) | ((hv_uint32_t) m.data[0]);
      hv->sendMessageToReceiverV(HV_HASH_BENDIN, 0, "ff",
        (float) value,
        (float) p.channel);
      break;
    }

    default: break;
  }
}

int main(void)
{
  hardware.Init(true);
  hv = new Heavy_Untitled(SAMPLE_RATE, 10, 2, 2);

  hardware.SetAudioSampleRate(48000);
  MidiUartHandler::Config midi_config;
  hardware.midi.Init(midi_config);
  hardware.midi.StartReceive();

  hardware.StartAudio(audiocallback);
  hv->setSendHook(sendHook);

  for(;;)
  {

    hardware.LoopProcess();
    hardware.midi.Listen();
    while(hardware.midi.HasEvents())
    {
      HandleMidiMessage(hardware.midi.PopEvent());
    }
    Display();

    uint8_t midiData[MIDI_OUT_FIFO_SIZE];
    size_t numElements = 0;

    while(!midi_tx_fifo.IsEmpty() && numElements < MIDI_OUT_FIFO_SIZE)
    {
      midiData[numElements++] = midi_tx_fifo.PopFront();
    }

    if(numElements > 0)
    {
      hardware.midi.SendMessage(midiData, numElements);
    }

  }
}

/** The audio processing function. At the standard 48KHz sample rate and a block
 *  size of 48, this will fire every millisecond.
 */
void audiocallback(daisy::AudioHandle::InputBuffer in, daisy::AudioHandle::OutputBuffer out, size_t size)
{
  hardware.ProcessAllControls();
  CallbackWriteIn(hv);
  hv->process((float**)in, (float**)out, size);
  hardware.PostProcess();
}

void HandleMidiOut(uint8_t *midiData, const uint8_t numElements)
{
  for (int i = 0; i < numElements; i++) {
    midi_tx_fifo.PushBack(midiData[i]);
  }
}

void HandleMidiSend(uint32_t sendHash, const HvMessage *m)
{
  switch(sendHash){
    case HV_HASH_NOTEOUT: // __hv_noteout
    {
      uint8_t note = hv_msg_getFloat(m, 0);
      uint8_t velocity = hv_msg_getFloat(m, 1);
      uint8_t ch = hv_msg_getFloat(m, 2);
      ch %= 16;  // drop any pd "ports"

      const uint8_t numElements = 3;
      uint8_t midiData[numElements];

      if (velocity > 0){
        midiData[0] = 0x90 | ch; // noteon
      } else {
        midiData[0] = 0x80 | ch; // noteoff
      }
      midiData[1] = note;
      midiData[2] = velocity;

      HandleMidiOut(midiData, numElements);
      break;
    }
    case HV_HASH_POLYTOUCHOUT:
    {
      uint8_t value = hv_msg_getFloat(m, 0);
      uint8_t note = hv_msg_getFloat(m, 1);
      uint8_t ch = hv_msg_getFloat(m, 2);
      ch %= 16; // drop any pd "ports"

      const uint8_t numElements = 3;
      uint8_t midiData[numElements];
      midiData[0] = 0xA0 | ch; // send Poly Aftertouch
      midiData[1] = note;
      midiData[2] = value;

      HandleMidiOut(midiData, numElements);
      break;
    }
    case HV_HASH_CTLOUT:
    {
      uint8_t value = hv_msg_getFloat(m, 0);
      uint8_t cc = hv_msg_getFloat(m, 1);
      uint8_t ch = hv_msg_getFloat(m, 2);
      ch %= 16;

      const uint8_t numElements = 3;
      uint8_t midiData[numElements];
      midiData[0] = 0xB0 | ch; // send CC
      midiData[1] = cc;
      midiData[2] = value;

      HandleMidiOut(midiData, numElements);
      break;
    }
    case HV_HASH_PGMOUT:
    {
      uint8_t pgm = hv_msg_getFloat(m, 0);
      uint8_t ch = hv_msg_getFloat(m, 1);
      ch %= 16;

      const uint8_t numElements = 2;
      uint8_t midiData[numElements];
      midiData[0] = 0xC0 | ch; // send Program Change
      midiData[1] = pgm;

      HandleMidiOut(midiData, numElements);
      break;
    }
    case HV_HASH_TOUCHOUT:
    {
      uint8_t value = hv_msg_getFloat(m, 0);
      uint8_t ch = hv_msg_getFloat(m, 1);
      ch %= 16;

      const uint8_t numElements = 2;
      uint8_t midiData[numElements];
      midiData[0] = 0xD0 | ch; // send Touch
      midiData[1] = value;

      HandleMidiOut(midiData, numElements);
      break;
    }
    case HV_HASH_BENDOUT:
    {
      uint16_t value = hv_msg_getFloat(m, 0);
      uint8_t lsb  = value & 0x7F;
      uint8_t msb  = (value >> 7) & 0x7F;
      uint8_t ch = hv_msg_getFloat(m, 1);
      ch %= 16;

      const uint8_t numElements = 3;
      uint8_t midiData[numElements];
      midiData[0] = 0xE0 | ch; // send Bend
      midiData[1] = lsb;
      midiData[2] = msb;

      HandleMidiOut(midiData, numElements);
      break;
    }
    // not functional yet
    // case HV_HASH_MIDIOUT: // __hv_midiout
    // {
    //   if (midiOutCount == 0 ) {
    //     uint8_t midiOutData[3];
    //   }

    //   midiOutData[midiOutCount] = hv_msg_getFloat(m, 0);

    //   if (midiOutCount < 2) {
    //     midiOutCount++;
    //     break;
    //   }

    //   HandleMidiOut(midiOutData, 3);
    //   midiOutCount = 0;
    //   break;
    // }
    default:
      break;
  }
}


/** Receives messages from PD and writes them to the appropriate
 *  index in the `output_data` array, to be written later.
 */
static void sendHook(HeavyContextInterface *c, const char *receiverName, uint32_t receiverHash, const HvMessage * m)
{
  HandleMidiSend(receiverHash, m);
}


/** Sends signals from the Daisy hardware to the PD patch via the receive objects during the main loop
 *
 */
void LoopWriteIn(Heavy_Untitled* hv)
{
  ScopedIrqBlocker block; //< Disables interrupts while in scope

}

/** Sends signals from the Daisy hardware to the PD patch via the receive objects during the audio callback
 *
 */
void CallbackWriteIn(Heavy_Untitled* hv)
{
  if ((hardware.sw1.RisingEdge()?1.f:0.f))
    hv->sendBangToReceiver((uint32_t) HV_UNTITLED_PARAM_IN_BUTTON1);
}

/** Writes the values sent to PD's receive objects to the Daisy hardware during the main loop
 *
 */
void LoopWriteOut() {
  ScopedIrqBlocker block; //< Disables interrupts while in scope

}

/** Writes the values sent to PD's receive objects to the Daisy hardware during the audio callback
 *
 */
void CallbackWriteOut() {
}

/** Handles the display code if the hardware defines a display
 *
 */
void Display() {

}