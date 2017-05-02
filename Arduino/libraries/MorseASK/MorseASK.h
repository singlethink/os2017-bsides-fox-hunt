#ifndef MorseASK_H
#define MorseASK_H

/* Author: Matthew Van Gundy KI6KTE */

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <MorseEnDecoder.h>

class MorseASK : public morseEncoder {
 public:
    static const bool DIGITAL_MODE;
    static const bool PWM_MODE;
    static const bool ACTIVE_HIGH;
    static const bool ACTIVE_LOW;

    MorseASK(int encodePin);
    MorseASK(int encodePin, bool activeHigh, bool pwm);

    void active(bool activeHigh);
    bool available();
    void encode();

    // Send full msg in morse code.  Caller must not change the
    // contents of message until available() returns true.
    void write(const char *msg);
    using morseEncoder::write;     // unhide void write(char)

private:
    bool _active;
    bool _pwm;
    const char *_currentChar;

    void start_pwm();
    void stop_pwm();
    void start_digital();
    void stop_digital();
protected:
    virtual void setup_signal();
    virtual void start_signal(bool startOfChar, char signalType);
    virtual void stop_signal(bool endOfChar, char signalType);
};

#endif
