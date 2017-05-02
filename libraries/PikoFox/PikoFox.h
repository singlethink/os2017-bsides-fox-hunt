#ifndef PIKOFOX_H
#define PIKOFOX_H

/* Author: Matthew Van Gundy */

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <MorseASK.h>

#define PF_MAX(a,b) ((a) > (b) ? (a) : (b))

// instantiate virtualwire and MorseASK
// Add ability to write entire message to MorseASK
//
// Data:
// - inter_packet_delay
// - packets_per_round
// - cw_message
// - to_call_sign
// - from_call_sign
// - courtesy_delay
// - radio_pin
// - bit_rate

#define PF_ACTIVE_HIGH MorseASK::ACTIVE_HIGH
#define PF_ACTIVE_LOW  MorseASK::ACTIVE_LOW

class PF_Fox {
  static const char DEFAULT_MSG[];
public:
  PF_Fox(byte radioPin, byte id, byte nfoxes, const char *toCall, const char *fromCall,
         const char *cwMsg = DEFAULT_MSG,
         uint32_t pktDelayMS = 1000, byte pktsPerRound = 30, float courtesyDelayFactor = 1.05,
         uint16_t bitRate = 500, uint8_t cwWPM = 10, bool activeLevel = PF_ACTIVE_LOW);
  bool cwMessage(const char *cwMsg);
  bool available();
  void run();

private:
  typedef void (PF_Fox::*StateFunc)();
  
  byte foxId;
  byte totalFoxes;
  const char *toCall;
  const char *fromCall;
  const char *cwMsg;
  uint32_t pktDelay; // in milliseconds
  byte pktsPerRound;
  float courtesyDelayFactor;
  MorseASK morseTx;
  uint32_t seq;
  StateFunc state;

  void init();
  void txPkt();
  void txCW();
  void courtesyWait();
};

// forward declarations :-/
class PF_AnyMsg;

struct PF_Msg {
    static const byte INVALID_MSG;
    static const byte ID_MSG;
    static const byte BEACON_MSG;

    byte foxId;
    uint32_t seq;

    byte type() { return tag; }

    // TODO: ntoh(), hton()
protected:
    byte tag;
    
    PF_Msg() : PF_Msg(INVALID_MSG, -1, 0) { };
    PF_Msg(byte tag) : PF_Msg(tag, -1, 0) { };
    PF_Msg(byte tag, byte id, uint32_t seq) : tag(tag), foxId(id), seq(seq) { }

    static PF_Msg* init(PF_Msg *msg, byte tag);

    friend class PF_AnyMsg;
};

struct PF_IdMsg : public PF_Msg {
    char to[9];
    char from[9];

    PF_IdMsg();
    static PF_IdMsg* init(PF_AnyMsg *msg);
    static PF_IdMsg* init(PF_IdMsg *msg);

    void setto(const char *call, size_t callLen);
    void setfrom(const char *call, size_t callLen);
};

struct PF_BeaconMsg : public PF_Msg {
    PF_BeaconMsg();
    static PF_BeaconMsg* init(PF_AnyMsg *msg);
    static PF_BeaconMsg* init(PF_BeaconMsg *msg);
};

class PF_AnyMsg {
    union {
        PF_Msg       base;
        PF_IdMsg     id;
        PF_BeaconMsg beacon;
    };

    friend class PF_IdMsg;
    friend class PF_BeaconMsg;
public:
    PF_AnyMsg() : base() { };
    PF_AnyMsg(const PF_AnyMsg &o) : base(o.base) { };

    // Return current message type
    inline byte type() const;

    // If PF_AnyMsg is of the requested type, these functions will
    // return a non-NULL pointer.  Returns NULL otherwise.
    PF_Msg* toMsg(uint16_t len);
    PF_IdMsg* toIdMsg(uint16_t len);
    PF_BeaconMsg* toBeaconMsg(uint16_t len);
};

struct PF_Util {
    static void zeroCopy(byte *to, size_t toLen, byte *from, size_t fromLen);
};

#define PF_MAX_MSG_LEN \
    PF_MAX(sizeof(PF_Msg), \
        PF_MAX(sizeof(PF_IdMsg), \
               sizeof(PF_BeaconMsg)))
#endif
