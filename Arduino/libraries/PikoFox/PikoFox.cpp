#include "PikoFox.h"

#include <VirtualWire.h>

const char PF_Fox::DEFAULT_MSG[] PROGMEM = "PF_Fox Beacon ";

const byte PF_Msg::INVALID_MSG = -1;
const byte PF_Msg::ID_MSG = 0;
const byte PF_Msg::BEACON_MSG = 1;


PF_Fox::PF_Fox(byte radioPin, byte id, byte nfoxes, const char *toCall, const char *fromCall,
               const char *cwMsg,
               uint32_t pktDelayMS, byte pktsPerRound, float courtesyDelayFactor,
               uint16_t bitRate, uint8_t cwWPM, bool activeLevel)
    : morseTx(radioPin, activeLevel, MorseASK::PWM_MODE), foxId(id),
      totalFoxes(nfoxes), toCall(toCall), fromCall(fromCall), pktDelay(pktDelayMS),
      pktsPerRound(pktsPerRound), courtesyDelayFactor(courtesyDelayFactor), cwMsg(cwMsg),
      seq(0), state(&init) {
    morseTx.setspeed(cwWPM);

    // Set up VirtualWire
    vw_set_tx_pin(radioPin);
    if(activeLevel == PF_ACTIVE_LOW)
        vw_set_ptt_inverted(true);
    vw_setup(bitRate);
}

bool PF_Fox::cwMessage(const char *cwMsg) {
    return false; //FIXME
}

bool PF_Fox::available() {
    return false; //FIXME
}

/* ****************************************************************************
 * State functions
 * **************************************************************************** */

void PF_Fox::run() { // FIXME
    (this->*state)();
}

void PF_Fox::init() {
    
    // sample start time
    // go to tx Pkt
}

void PF_Fox::txPkt() {
    //transmit packets
    // go to cw
}

void PF_Fox::txCW() {
    // tx cw
}

void PF_Fox::courtesyWait() {
    // Sample stop time
//  wait courtesy delay
    // goto init
}


PF_Msg* PF_Msg::init(PF_Msg *msg, byte tag) {
    msg->tag = tag;
    return msg;
}

PF_IdMsg::PF_IdMsg() : PF_Msg(ID_MSG) {
    PF_IdMsg::init(this);
}

PF_IdMsg* PF_IdMsg::init(PF_IdMsg *msg) {
    PF_Msg::init(msg, ID_MSG);
    const char empty[] = "";
    msg->setto(empty, sizeof(empty));
    msg->setfrom(empty, sizeof(empty));
    return msg;
}

PF_IdMsg* PF_IdMsg::init(PF_AnyMsg *anyMsg) {
    return PF_IdMsg::init( &(anyMsg->id) );
}

void PF_IdMsg::setto(const char *call, size_t callLen) {
    PF_Util::zeroCopy(to, sizeof(to), call, callLen);
}

void PF_IdMsg::setfrom(const char *call, size_t callLen) {
    PF_Util::zeroCopy(from, sizeof(from), call, callLen);
}

PF_BeaconMsg::PF_BeaconMsg() : PF_Msg(BEACON_MSG) {
    PF_BeaconMsg::init(this);
}

PF_BeaconMsg* PF_BeaconMsg::init(PF_BeaconMsg *msg) {
    PF_Msg::init(msg, BEACON_MSG);
    return msg;
}

PF_BeaconMsg* PF_BeaconMsg::init(PF_AnyMsg *anyMsg) {
    return PF_BeaconMsg::init( &(anyMsg->beacon) );
}

byte PF_AnyMsg::type() const {
    return base.type();
}

PF_Msg* PF_AnyMsg::toMsg(uint16_t len) {
    if(len >= sizeof(PF_Msg)) {
        return &base;
    } else {
        return NULL;
    }
}

PF_IdMsg* PF_AnyMsg::toIdMsg(uint16_t len) {
    if(len >= sizeof(PF_IdMsg) && base.tag == PF_Msg::ID_MSG) {
        return &id;
    } else {
        return NULL;
    }
}

PF_BeaconMsg* PF_AnyMsg::toBeaconMsg(uint16_t len) {
    if(len >= sizeof(PF_BeaconMsg) && base.tag == PF_Msg::BEACON_MSG) {
        return &beacon;
    } else {
        return NULL;
    }
}


void PF_Util::zeroCopy(byte *to, size_t toLen, byte *from, size_t fromLen) {
    memset(to, 0, toLen);
    size_t copyLen = fromLen > toLen ? toLen : fromLen;
    memcpy(to, from, copyLen);
};
