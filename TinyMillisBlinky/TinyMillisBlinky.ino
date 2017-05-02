/* TinyMillisBlinky - a simple sketch to test the TinyMillis timer.
 * A millis()-like timer that uses Timer1 on the ATTiny85
 */

#include <TinyMillis.h>

const byte LED_PIN = 0;
const uint16_t TIMEOUT = 1000; // 1s
boolean led_state = LOW;
int nextChange = 0;


void setup() {
  tinyMillisSetup();
  pinMode(LED_PIN, OUTPUT);
  nextChange = tinyMillis;
}

void loop() {
  if(tinyMillisTimerExpired(nextChange)) {
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
    nextChange = tinyMillis() + TIMEOUT;
  }
}
