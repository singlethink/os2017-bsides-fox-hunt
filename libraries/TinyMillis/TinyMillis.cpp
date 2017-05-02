#include "TinyMillis.h"

#define RESOLUTION  1 /* ms */

static volatile int32_t tinyMillisCnt = 0;

// Compute counter value required to interrupt every
// durationMs milliseconds given the current CPU
// frequency and prescale factor
static int tinyMillisCounterValue(int prescale, int durationMs) {
  return durationMs * 0.001 * F_CPU/prescale - 1;
}

// Compute the prescale factor required to provide a durationMs
// interrupt period
static int tinyMillisComputePrescale(int durationMs) {
  int prescale = 1;
  for(prescale = 1; tinyMillisCounterValue(prescale, durationMs) > 255; prescale <<= 1) {
    // keep counting
  }
  return prescale;
}

// Compute the prescale bit vector required to configure
// TCCR1 to apply the chosen prescale factor to Timer1
static byte tinyMillisPrescaleBV(int prescale) {
  byte bv = 0;
  while(prescale > 0) {
    prescale >>= 1;
    bv++;
  }
  if(bv > 0x0f) {
    bv = 0x0f;
  }
  return (bv<<CS10);
}

void tinyMillisSetup() {
  // Try to create our own millisecond timer
  // by setting timer 1 to interrupt every RESOLUTION ms
  tinyMillisCnt = 0;
  int prescale = tinyMillisComputePrescale(RESOLUTION /* ms */);
  int counterMax = tinyMillisCounterValue(prescale, RESOLUTION /* ms */);
  if(counterMax > 0xff) {
    counterMax = 0xff;
  }
  // Enable Clear Timer/Counter on Compare Match
  // and prescale the timer with an appropriate prescale factor
  TCCR1 = (1<<CTC1) | tinyMillisPrescaleBV(prescale);
  TCNT1 = 0;                  // zero the counter
  GTCCR |= (1<<PSR1);         // reset the prescaler
  GTCCR &= ~(1<<PWM1B);       // disable PWM
  GTCCR &= ~( (1<<COM1B1) | (1<<COM1B0)); // Don't modify output line based on comparator
  OCR1A = (byte)counterMax;   // interrupt will trigger when counter reaches counterMax
  OCR1C = (byte)counterMax;   // and clear the counter when it reaches that value
  TIMSK |= bit(OCIE1A);       // trigger interrupt when counter == OCR1A
  sei();
}

SIGNAL(TIMER1_COMPA_vect) {
  tinyMillisCnt += RESOLUTION;
}

int32_t tinyMillis() {
  return tinyMillisCnt;
}

// Determine if the current time has proceeded past endTime
bool tinyMillisTimerExpired(int32_t endTime) {
  return tinyMillisCnt - endTime >= 0;
}


