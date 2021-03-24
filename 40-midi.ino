

#include <MIDI.h>

#define NOTE_OFF_EVENT 0x80
#define NOTE_ON_EVENT 0x90
#define CONTROL_CHANGE_EVENT 0xB0
#define PITCH_BEND_EVENT 0xE0

MIDI_CREATE_DEFAULT_INSTANCE();


void setupMidi() {
  // set up MIDI handling
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandlePitchBend(handlePitchBend);
  MIDI.setHandleControlChange(handleControlChange);
}

void loopMidi() {
  
  MIDI.read();
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  writeToFile(NOTE_OFF_EVENT, pitch, velocity, getDelta());
  lastNoteSignal = currentMillis;
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
 // drawText("note on");
  writeToFile(NOTE_ON_EVENT, pitch, velocity, getDelta());
  lastNoteSignal = currentMillis;
  // if (play) tone(AUDIO, 440 * pow(2, (pitch - 69.0) / 12.0), 100);
}

void handleControlChange(byte channel, byte cc, byte value) {
  
  // drawText("cc");
  writeToFile(CONTROL_CHANGE_EVENT, cc, value, getDelta());
  lastCCSignal = currentMillis;
}

void handlePitchBend(byte channel, int bend) {
  bend += 0x2000; // MIDI bend uses the range 0x0000-0x3FFF, with 0x2000 as center.
  byte lsb = bend & 0x7F;
  byte msb = bend >> 7;
  writeToFile(PITCH_BEND_EVENT, lsb, msb, getDelta());
}
