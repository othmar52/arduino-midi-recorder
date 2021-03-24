

/*
   DS3231 Real Time Clock
   https://www.amazon.de/dp/B01M2B7HQB
                        ____________________________
                       |                            |
                       |_                           |
                       |•|32K                       |
                       |•|SQW                       |
   to Arduino UNO A5 <-|•|SCL         PINS of       |
   to Arduino UNO A4 <-|•|SDA         DS3231        |
   to Arduino UNO 5V <-|•|VCC                       |
  to Arduino UNO GND <-|•|GND                       |
                       |¯                           |
                       |____________________________|
*/



#define CHIP_SELECT 10
#define FILE_FLUSH_INTERVAL 400
#define HAS_MORE_BYTES 0x80

String filename;
File file;

void setupSD() {

  // set up RTC interfacing
  if (RTC.begin()) {
    // uncomment this line to set the current date/time on the RTC
    // RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // if the RTC works, we can tell the SD library
    // how it can check for the current time when it
    // needs timestamping for file creation/writing.
    SdFile::dateTimeCallback(dateTime);
    HAS_RTC = true;
    //drawText("RTC yes");
  } else {
    //drawText("RTC no");
  }

  // set up SD card functionality and allocate a file
  pinMode(CHIP_SELECT, OUTPUT);
  initSd();

}

void initSd() {
  sdCardError = true;
  if (SD.begin(CHIP_SELECT)) {
    
    // drawText("sd begin");
    createNextFile();
    if (file) {
      writeMidiPreamble();
      // tone(AUDIO, 1760, 100);
    }
  }
}
 
/**
    We could use the EEPROM to store this number,
    but since we're not going to get timestamped
    files anyway, just looping is also fine.
*/
void createNextFile() {
  for (int i = 1; i < 1000; i++) {
    filename = "file-";
    if (i < 10) filename += "0";
    if (i < 100) filename += "0";
    filename += String(i);
    filename += String(".mid");
    if (!SD.exists(filename)) {
      file = SD.open(filename, FILE_WRITE);
      
      //drawText("new file");
      sdCardError = false;
      if (file == false) {
         sdCardError = true;
      }
      return;
    }
  }
  sdCardError = true;
}

/**
   Set up a new MIDI file with some boiler plate byte code
*/
void writeMidiPreamble() {
  byte header[] = {
    0x4D, 0x54, 0x68, 0x64,   // "MThd" chunk
    0x00, 0x00, 0x00, 0x06,   // chunk length (from this point on)
    0x00, 0x00,               // format 0
    0x00, 0x01,               // one track
    0x01, 0xD4                // data rate = 458 ticks per quarter note
  };
  file.write(header, 14);

  byte track[] = {
    0x4D, 0x54, 0x72, 0x6B,   // "MTrk" chunk
    0x00, 0x00, 0x00, 0x00    // chunk length placeholder (MSB)
  };
  file.write(track, 8);

  byte tempo[] = {
    0x00,                     // time delta (of zero)
    0xFF, 0x51, 0x03,         // tempo op code
    //0x06, 0xFD, 0x1F          // real rate = 458,015μs per quarter note (= 134681 BPM)
    0x07, 0xA1, 0x20          // test = 458,015μs per quarter note (= 120 BPM)
  };
  file.write(tempo, 7);
}



/**
   We flush the file's in-memory content to disk
   every 400ms, allowing. That way if we take the
   SD card out, it's basically impossible for any
   data to have been lost.
*/
void updateFile() {
  loopCounter = currentMillis;
  if (loopCounter - lastLoopCounter > FILE_FLUSH_INTERVAL) {
    checkReset();
    lastLoopCounter = loopCounter;
    file.flush();
  }
}



/**
   Write "common" MIDI events to file, where common MIDI events
   all use the following data format:
     <delta> <event code> <byte> <byte>
   See the "Standard MIDI-File Format" for more information.
*/
void writeToFile(byte eventType, byte b1, byte b2, int delta) {
  if (!file) return;
  writeVarLen(file, delta);
  uint16_t writtenBytes = 0;
  writtenBytes += file.write(eventType);
  writtenBytes +=file.write(b1);
  writtenBytes +=file.write(b2);
  
  sdCardError = false;
  if (writtenBytes == 0) {
    sdCardError = true;
  }
}

/**
   Encode a unsigned 32 bit integer as variable-length byte sequence
   of, at most, 4 7-bit-with-has-more bytes. This function is supplied
   as part of the MIDI file format specification.
*/
void writeVarLen(File file, unsigned long value) {
  // capture the first 7 bit block
  unsigned long buffer = value & 0x7f;

  // shift in 7 bit blocks with "has-more" bit from the
  // right for as long as `value` has more bits to encode.
  while ((value >>= 7) > 0) {
    buffer <<= 8;
    buffer |= HAS_MORE_BYTES;
    buffer |= value & 0x7f;
  }

  // Then unshift bytes one at a time for as long as the has-more bit is high.
  while (true) {
    file.write((byte)(buffer & 0xff));
    if (buffer & HAS_MORE_BYTES) {
      buffer >>= 8;
    } else {
      break;
    }
  }
}



void dateTime(uint16_t* date, uint16_t* time) {
  DateTime d = RTC.now();
  *date = FAT_DATE(d.year(), d.month(), d.day());
  *time = FAT_TIME(d.hour(), d.minute(), d.second());
}
