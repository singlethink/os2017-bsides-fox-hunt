# Programming the ATtiny85 with a Tiny AVR Programmer (PGM-11801 w/ ATtiny84)

If you have not gone through using an Arduino to program an ATtiny85, make sure your dev environment
 is setup.

* [Download the Arduino IDE](https://www.arduino.cc/en/Main/Software)

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

* Select "USBtinyISP" from the programmer menu (Tools->Programmer->USBtinyISP)
 
* Plug in the Tiny AVR Programmer w/ the ATtiny85 in the socket. Make sure the chip is facing correctly (The small, etched circle on the IC should line up with the “notch” on the Programmer’s socket and silkscreen.)

![Proper Chip placement](https://cdn.sparkfun.com/r/600-600/assets/3/f/5/f/4/52713369757b7ff7668b4567.png)

We can ignore the Port selection menu (for MAC/Linux). Windows users will need additional drivers (See references).

* Upload the sketch (Right Arrow Icon Above the editor)

You should see “Done uploading.” in the Arduino software and no error messages. If you then connect an LED between PORTB 4 (pin 3) and ground, you should see it blink on and off. Note that you may need to disconnect the LED before uploading a new program.

# References

* https://www.sparkfun.com/products/11801
