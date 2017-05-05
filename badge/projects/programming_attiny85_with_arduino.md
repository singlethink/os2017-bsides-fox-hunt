# Programming the ATtiny85 with an Arduino Uno (w/ an ATmega328)

## Arduino Setup

* [Download the Arduino IDE](https://www.arduino.cc/en/Main/Software)

* Run the IDE and navigate the menus (File->Examples->11.ArduinoISP->ArduinoISP)

This brings up code that will turn the Arduino into an in-system programmer (ISP)

* Connect the Arduino via USB and select it via the menus (Tools->Port)

* Verify the USBasp programmer is selected (Tools->Programmer->USBasp)

* Upload the Code (Right Arrow Icon)

## ATtiny Setup

### Wiring the Programming Lights

* Arduino Pin 9 -> Resistor -> LED -> GND (Heartbeat)

* Arduino Pin 8 -> Resistor -> LED -> GND (Error)

* Arduino Pin 7 -> Resistor -> LED -> GND (Programming)

### Wiring the ATtiny

* ATtiny Pin 1 (RESET) -> Arduino Pin 10

* ATtiny Pin 4 (GND) -> GND

* ATtiny Pin 5 (MOSI) -> Arduino Pin 11

* ATtiny Pin 6 (MISO) -> Arduino Pin 12

* ATtiny Pin 7 (SCK) -> Arduino Pin 13

* ATtiny Pin 8 (VCC) -> Arduino 5V Pin

* Arduino Reset Pin -> 10 mircofarad capacitor -> GND

(Prevents the Arduino from resetting when programming) 

## Adding the ATtiny as a programmable board

* (Arduino->Preferences->Additional Boards Manager URLs)
 * https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json

* Download the ATtiny board files (Tools->Board->Boards Manager)

 * Search for "attiny" and download version 1.0.2

## Programming the ATtiny

* Open the Blink sketch from the examples menu. (File->Examples->01.Basics->Blink)

* Change "LED_BUILTIN" to 4.
 
 * Or add ```#define LED_BUILTIN 4``` to the top of the file.

* Select “ATtiny25/45/85” from the Board menu (Tools->Board->ATtiny25/45/85)

* Select "ATtiny85" from the processor menu (Tools->Processor->ATtiny85)

* Select "Internal 1 MHz" from the clock menu (Tools->Clock->Internal 1 MHz)

* Select "Arduino as ISP" from the programmer menu (Tools->Programmer->Arduino as ISP)

* Upload the sketch (Right Arrow Icon Above the editor)

You should see “Done uploading.” in the Arduino software and no error messages. If you then connect an LED between PORTB 4 (Pin 3) and ground, you should see it blink on and off. Note that you may need to disconnect the LED before uploading a new program.

(NOTE: The ERROR LED will stay lit following programming, if it bothers you, disconnect it.)

# References 

* http://highlowtech.org/?p=1706

* http://highlowtech.org/?p=1695
