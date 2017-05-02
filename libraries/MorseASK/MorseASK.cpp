#include "MorseASK.h"

const bool MorseASK::DIGITAL_MODE = false;
const bool MorseASK::PWM_MODE     = true;
const bool MorseASK::ACTIVE_HIGH  = HIGH;
const bool MorseASK::ACTIVE_LOW   = LOW;

MorseASK::MorseASK(int encodePin) : MorseASK(encodePin, ACTIVE_LOW, PWM_MODE) {
}
                                 

MorseASK::MorseASK(int encodePin, bool activeHigh, bool usePWM)
    : morseEncoder(encodePin), _active(activeHigh), _pwm(usePWM),
      _currentChar(NULL) {
    // KLUDGE: Must call setup_signal() again because morseEncoder
    // calls it before we have an opportunity to set this class's
    // variables.  Stupid C++ constructor ordering :-(
    this->setup_signal();
}

bool MorseASK::available() {
    // ensure there isn't more to send
    return morseEncoder::available() && !_currentChar;
}

void MorseASK::write(const char *msg) {
    if(MorseASK::available()) {
        _currentChar = msg;
    }
}

void MorseASK::encode() {
    // If the encoder is done encoding the current char and we have
    // more to send, write the next character
    if(morseEncoder::available() && _currentChar) {
        if(!*_currentChar) {   // we're done
            _currentChar = NULL;
        } else {
            morseEncoder::write(*_currentChar);
            _currentChar++;
        }
    }
    morseEncoder::encode();
}

void MorseASK::start_pwm() {
    // PWM key down
    analogWrite(morseOutPin, 128);
}
    
void MorseASK::stop_pwm() {
    // PWM key up
    if(_active == LOW) {
        analogWrite(morseOutPin, 255);
    } else {
        analogWrite(morseOutPin, 0);
    }
}
        
void MorseASK::start_digital() {
    digitalWrite(morseOutPin, _active);
}

void MorseASK::stop_digital() {
    digitalWrite(morseOutPin, !_active);
}

void MorseASK::setup_signal() {
  pinMode(morseOutPin, OUTPUT);
  this->stop_signal(false, 'x');
}


void MorseASK::start_signal(bool startOfChar, char signalType) {
    if(_pwm) start_pwm();
    else    start_digital();
}


void MorseASK::stop_signal(bool endOfChar, char signalType) {
    if(_pwm) stop_pwm();
    else    stop_digital();
}


void MorseASK::active(bool activeHigh) {
    _active = activeHigh;
}
