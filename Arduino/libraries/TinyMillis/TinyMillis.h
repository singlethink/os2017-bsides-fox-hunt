#ifndef TINYMILLIS_H
#define TINYMILLIS_H

/* TinyMillis - A millis()-like timer for the ATTiny85 using Timer1
 *
 * Author: Matthew Van Gundy, KI6KTE
 *
 * References:
 * - https://forum.arduino.cc/index.php?topic=163393.msg1221225#msg1221225
 * - ATTiny85 Datasheet - https://www.microchip.com/wwwproducts/en/ATtiny85
 */

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

// Must be called from setup()
// hijacks Timer/Counter1
extern void tinyMillisSetup();

// Equivalent of millis()
extern int32_t tinyMillis();

// Determine if the current time has proceeded past endTime
extern bool tinyMillisTimerExpired(int32_t endTime);
#endif
