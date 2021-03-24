/*********************************************************
   This is the code for a prototype inline MIDI recorder
   that sits between a MIDI-OUT device (like a controller)
   and a MIDI-IN device (like a computer) for saving all
   note, pitch, and CC data that's being played without
   having any device specifically set to "record".
   This is Public Domain code, with all the caveats that
   comes with for you, because public domain is not the
   same as open source.
   See https://opensource.org/node/878 for more details.
   The reason this code is in the public domain is
   because anyone with half a brain and a need to
   create this functionality will reasonably end up
   with code that's so similar as to effectively be
   the same as what has been written here.
   Having said that, there are countries that do not
   recognize the Public Domain. In those countries,
   this code is to be considered as being provided
   under an MIT license. See the end of this file
   for its full license text.

   original created by Pomax 2021
   changed by othmar52 2021
 ********************************************************/



// File and MIDI handling
#include <SD.h>

// Our Real Time Clock
#include <RTClib.h>
RTC_DS3231 RTC;
bool HAS_RTC = false;


// we use a 10 minute idling timeout (in millis)
#define RECORDING_TIMEOUT 600000

unsigned long lastLoopCounter = 0;
unsigned long loopCounter = 0;

unsigned long startTime = 0;
unsigned long lastTime = 0;

unsigned long currentMillis = 0;
bool isRecording = false;

bool sdCardError = false;

/**
   Set up our inline MIDI recorder
*/
void setup() {
  setupLcd();
  setupMidi();
  setupSD();
}


/**
   The program loop consists of flushing our file to disk,
   and then handling MIDI input, if there is any.
*/
void loop() {
  currentMillis = millis();
  updateFile();
  loopMidi();
  updateDisplay();
}


/**
   This calculates the number of ticks since the last MIDI event
*/
int getDelta() {
  if (startTime == 0) {
    // if this is the first event, even if the Arduino's been
    // powered on for hours, this should be delta zero.
    startTime = millis();
    isRecording = true;
    lastTime = startTime;
    // drawText(" start");
    return 0;
  }
  unsigned long now = millis();
  unsigned int delta = (now - lastTime);
  lastTime = now;
  return delta;
}


/*********************************************************
   If you live in a country that does not recognise the
   Public Domain, the following (MIT) license applies:
     Copyright 2020, Pomax
     Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
     The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ********************************************************/
