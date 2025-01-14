# Arduino UNO based MIDI recorder
Original created by [Pomax](https://github.com/Pomax/arduino-midi-recorder)  

This fork uses a TFT Shield with SD-card slot. Further only USB-Midi is used (see [firmware](https://github.com/othmar52/arduino-midi-recorder/tree/master/firmware) to turn your Arduino UNO into a class compliant USB MIDI device).  

## Building

In case you want to use the combination of
 - Arduino UNO
 - AZDelivery 2.4TFT LCD Touch Display Parent
 - DS3231 Real Time Clock

### cut the pin `LCD_RST` of the TFT shield  
![AZDelivery 2.4TFT LCD Touch Display Parent](media/arduino-midi-recorder-01.jpg "AZDelivery 2.4TFT LCD Touch Display Parent")  

### add some wires
```
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

```
the cut pin of the TFT shild has to be wired to Arduino UNO's `RESET` pin (green wire).  
The 4 other wires are connected to the `DS3231`  
![wires](media/arduino-midi-recorder-02.jpg "wires")  

### connect all cables
![connected cables](media/arduino-midi-recorder-03.jpg "connected cables")  

### connect shield with the Arduino UNO and USB cable
![connected shields](media/arduino-midi-recorder-04.jpg "connected shields")  

## Display
After powering the device is armed. As soon as there is incoming MIDI the recordings starts automatically.  
![display of arduino midi recorder - ready to record](media/arduino-midi-recorder-05.jpg "display of arduino midi recorder - ready to record")  
During recording you can see the duration since start. Further a note- and cc-indicator is shown when signals are received and recorded (not visible in this screenshot).  
![display of arduino midi recorder - during recording](media/arduino-midi-recorder-06.jpg "display of arduino midi recorder - during recording")  
In case you remove the SD-card this screen is shown on the display  
![display of arduino midi recorder - sd card error](media/arduino-midi-recorder-07.jpg "display of arduino midi recorder - sd card error")