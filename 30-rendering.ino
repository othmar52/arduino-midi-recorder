uint8_t itemRectWidth = 0;
uint8_t itemRectHeight = 0;
uint8_t itemRectOffsetLeft = 0;
uint8_t itemRectOffsetTop = 0;

unsigned long lastNoteSignal = 0;
bool noteIndicatorVisible = false;


String previousFilename;

unsigned long lastCCSignal = 0;
bool ccIndicatorVisible = false;


unsigned long lastRenderedSeconds = 1000;

bool displayShowsWaiting = false;
bool sdErrorVisible = false;

void updateDisplay() {
  if (sdCardError == true && sdErrorVisible == false) {
    sdErrorVisible = true;
    drawSdCardError();
    return;
  }
  if (sdCardError == true && sdErrorVisible == true) {
    delay(1000);
    initSd();
    return;
  }
  if (sdCardError == false && sdErrorVisible == true) {
    tft.fillRect(0, 65, lcdWidth, lcdHeight, BLACK);
    sdErrorVisible = false;
    displayShowsWaiting == false;
    isRecording == false;
    startTime = 0;
  }
  updateNoteIndicator();
  updateCCIndicator();
  updateFilename();
  updateRecTime();
}

void drawSdCardError() {
  itemRectWidth = 155;
  itemRectHeight = 42;
  itemRectOffsetLeft = lcdWidth/2 - itemRectWidth/2;
  itemRectOffsetTop = 90;
  tft.fillRect(0, 65, lcdWidth, lcdHeight, DARK_GRAY);
  tft.fillRect(itemRectOffsetLeft, itemRectOffsetTop, itemRectWidth, itemRectHeight, RED);
  tft.fillRect(itemRectOffsetLeft, itemRectOffsetTop, itemRectWidth, itemRectHeight, RED);
  tft.setCursor(itemRectOffsetLeft + 6, itemRectOffsetTop + 11);
  tft.setTextColor(WHITE );
  tft.setTextSize(3);
  tft.println("SD Error");
}

void hello() {
  
  tft.fillRect(0, 0, lcdWidth, 65, DARK_GRAY);
  tft.setCursor(35, 20);
  tft.setTextColor(WHITE );
  tft.setTextSize(3);
  tft.println("MIDI RECORDER");
  tft.setTextSize(2);
  tft.setTextColor(GRAY);
  tft.println("         by Pomax");
}

String zeroPadLeft(int number) {
  return String((number >= 10) ? number : "0" + String(number));
}

void updateFilename() {
  if(previousFilename == filename) {
    return;
  }
  itemRectWidth = 230;
  itemRectHeight = 28;
  itemRectOffsetLeft = lcdWidth/2 -itemRectWidth/2;
  itemRectOffsetTop = 166;
  tft.fillRect(itemRectOffsetLeft, itemRectOffsetTop, itemRectWidth, itemRectHeight, BLACK);
  tft.setCursor(itemRectOffsetLeft + 5, itemRectOffsetTop + 3);
  tft.setTextColor(GRAY);
  tft.setTextSize(3);
  tft.println(filename);
  previousFilename = filename;
  
}



void drawText(String text) {
  tft.fillRect(0, 0, lcdWidth, lcdHeight, DARK_GRAY);
  tft.setCursor(0, 100);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println(text);  
}



void updateRecTime() {
  itemRectWidth = 155;
  itemRectHeight = 42;
  itemRectOffsetLeft = lcdWidth/2 - itemRectWidth/2;
  itemRectOffsetTop = 90;
  if (isRecording == false && displayShowsWaiting == false) {
    //tft.setCursor(0,0);
    tft.fillRect(itemRectOffsetLeft, itemRectOffsetTop, itemRectWidth, itemRectHeight, DARK_GRAY);
    tft.setCursor(itemRectOffsetLeft + 12, itemRectOffsetTop + 3);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    displayShowsWaiting = true;
    tft.println("waiting for");
    tft.setCursor(itemRectOffsetLeft + 60, itemRectOffsetTop + 23);
    tft.println("MIDI");
    return;
  }
  if (isRecording == false && displayShowsWaiting == true) {
    return;
  }
  displayShowsWaiting = false;
  unsigned long deltaSecondsSinceStart = (currentMillis - startTime)/1000;
  if(isRecording == false) {
    deltaSecondsSinceStart = 0;
  }
  if(lastRenderedSeconds == deltaSecondsSinceStart) {
    return;
  }
  tft.fillRect(itemRectOffsetLeft, itemRectOffsetTop, itemRectWidth, itemRectHeight, RED);
  tft.setCursor(itemRectOffsetLeft + 6, itemRectOffsetTop + 11);
  tft.setTextColor(WHITE );
  tft.setTextSize(3);
  uint16_t h;
  uint8_t m;
  uint8_t s;
  secondsToHMS(deltaSecondsSinceStart, h, m, s);
  tft.println(zeroPadLeft(h) + ":" + zeroPadLeft(m) + ":" + zeroPadLeft(s));
  lastRenderedSeconds = deltaSecondsSinceStart;
  
}


void updateNoteIndicator() {

  itemRectWidth = 48;
  itemRectHeight = 14;
  itemRectOffsetLeft = 10;
  itemRectOffsetTop = 104;

  if (currentMillis - lastNoteSignal > 300 && noteIndicatorVisible == true) {
    // remove note indicator
    tft.fillRect(itemRectOffsetLeft, itemRectOffsetTop, itemRectWidth, itemRectHeight, BLACK);
    noteIndicatorVisible = false;
    return;
  }

  if (currentMillis - lastNoteSignal < 300 && noteIndicatorVisible == false) {
    // show note indicator
    noteIndicatorVisible = true;
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.fillRect(itemRectOffsetLeft, itemRectOffsetTop, itemRectWidth, itemRectHeight, BLACK);
    tft.setCursor(itemRectOffsetLeft, itemRectOffsetTop);
    tft.println("NOTE");
    
  }
}

void updateCCIndicator() {

  itemRectWidth = 23;
  itemRectHeight = 14;
  //itemRectOffsetLeft = lcdWidth - 10 - itemRectWidth;
  itemRectOffsetTop = 104;

  if (currentMillis - lastCCSignal > 300 && ccIndicatorVisible == true) {
    // remove note indicator
    tft.fillRect(lcdWidth - 10 - itemRectWidth, itemRectOffsetTop, itemRectWidth, itemRectHeight, BLACK);
    ccIndicatorVisible = false;
    return;
  }

  if (currentMillis - lastCCSignal < 300 && ccIndicatorVisible == false) {
    // show note indicator
    ccIndicatorVisible = true;
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.fillRect(lcdWidth - 10 - itemRectWidth, itemRectOffsetTop, itemRectWidth, itemRectHeight, BLACK);
    tft.setCursor(lcdWidth - 10 - itemRectWidth, itemRectOffsetTop);
    tft.println("CC");
    
  }
}




void secondsToHMS( const uint32_t seconds, uint16_t &h, uint8_t &m, uint8_t &s )
{
    uint32_t t = seconds;

    s = t % 60;

    t = (t - s)/60;
    m = t % 60;

    t = (t - m)/60;
    h = t;
}
