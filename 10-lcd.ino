/*
   AZDelivery 2.4TFT LCD Touch Display Parent
   https://www.amazon.de/dp/B086W6D9Z2
                        ________________________
                       |J3                    J2|
                       |_                      _|
                       |•|                    |•|
to Arduino UNO RESET <-|•|LCD_RST             |•|
   to Arduino UNO A3 <-|•|LCD_CS        LCD_02|•|-> to Arduino UNO D2
   to Arduino UNO A2 <-|•|LCD_RS        LCD_03|•|-> to Arduino UNO D3
   to Arduino UNO A1 <-|•|LCD_WR        LCD_04|•|-> to Arduino UNO D4
   to Arduino UNO A0 <-|•|LCD_RD        LCD_05|•|-> to Arduino UNO D5
                       |¯               LCD_06|•|-> to Arduino UNO D6
                       |_               LCD_07|•|-> to Arduino UNO D7
                       |•|                     ¯|
  to Arduino UNO GND <-|•|GND                  _|
                       |•|              LCD_D0|•|-> to Arduino UNO D8
   to Arduino UNO 5V <-|•|5V            LCD_01|•|-> to Arduino UNO D9
                       |•|3V3            SD_SS|•|-> to Arduino UNO D10
                       |•|               SD_DI|•|-> to Arduino UNO D11
                       |¯                SD_D0|•|-> to Arduino UNO D12
                       |J4              SD_SCK|•|-> to Arduino UNO D13
                       |                       ¯|
                       |         PINS of      J1|
                       |       AZ-Delivery      |
                       |       2.4TFT LCD       |
                       |________________________|
*/

#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPI.h>   // TODO: check if we really need this

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

//#define LCD_RESET HIGH // Arduino UNO's reset pin
//#define LCD_RESET 24 // Arduino UNO's reset pin


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, A6);

uint16_t lcdWidth = 0;
uint16_t lcdHeight = 0;



#define BLACK   0x0000
#define GRAY    0x94B2
#define WHITE   0xFFFF
#define DARK_GRAY 0x2946
#define PINK    0xF81B
#define RED     0xF800

void setupLcd(void) {
  //Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1); // landscape
  lcdWidth = tft.width() - 1;
  lcdHeight = tft.height() - 1;
  //delay(1000);
  tft.fillScreen(BLACK);
  hello();
}
