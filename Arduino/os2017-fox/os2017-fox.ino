// OS 2017 Fox Hunt Beacon

#include <MorseASK.h>
#include <VirtualWire.h>
#include <PikoFox.h>
#include <OS2017_Common.h>
#include <TinyMillis.h>

#define WPM 10
const byte FOX_ID = 0;
const byte RADIO_PIN = 0;
const byte LED4_PIN = 4;
const byte LED2_PIN = 2;
const byte LED1_PIN = 1;
const byte NFOXES = 2;
const char TO_CALL[] = "BEACON";
const char FROM_CALL[] = "KI6KTE-2";
const char BEACON_BODY[] = "OS2017 Fox Hunt: http://bit.ly/2p2Dsjm";
const char* BEACON_PARTS[] = { TO_CALL, FROM_CALL, BEACON_BODY };
const byte BEACON_PARTS_LEN = sizeof(BEACON_PARTS) / sizeof(BEACON_PARTS[0]);
const uint8_t PKTS_PER_ROUND = 30;
const uint32_t INTER_PKT_DELAY_MS = 1000;
const uint16_t BIT_RATE = OS2017_FOX_BIT_RATE;
const uint32_t  INTER_MSG_DELAY = 30 * 1000; // 30 s

enum state {
  ST_INIT,
  ST_PKT,
  ST_PRE_ID_WAIT,
  ST_ID,
  ST_WAIT
};
byte state;
uint32_t seq;
uint8_t pktsSent;
int32_t sleepUntil;
byte idPart;
boolean spaceNeeded;

PF_AnyMsg msg;
MorseASK morseTx(RADIO_PIN, MorseASK::ACTIVE_HIGH, MorseASK::DIGITAL_MODE);

void gotoInit() {
  // Nothing!
}

void doInit() {
  gotoPkt();
}

void gotoPkt() {
  state = ST_PKT;
  pktsSent = 0;
  sleepUntil = tinyMillis();
  displayState(state);
}

void doPkt() {
  size_t txLen;
  
  if(vw_tx_active()) {
    // we can't do anything while the transmitter is active
    // just go around the loop again
    return;
  }
  if(pktsSent == PKTS_PER_ROUND) {
    // We're done sending packets go to ID state
    gotoPreIdWait();
    return;
  }
  if(!tinyMillisTimerExpired(sleepUntil)) {
    // We're still waiting for our inter packet delay to complete
    return;
  }
  // We can initiate sending a packet...
  if(pktsSent == 0 || pktsSent == PKTS_PER_ROUND - 1) {
    // send id
    PF_IdMsg *idMsg = PF_IdMsg::init(&msg);
    idMsg->foxId = FOX_ID;
    idMsg->seq = seq++;
    idMsg->setto(TO_CALL, sizeof(TO_CALL));
    idMsg->setfrom(FROM_CALL, sizeof(FROM_CALL));
    txLen = sizeof(*idMsg);
    vw_send((byte*)idMsg, txLen);
    vw_wait_tx();
  } else {
    // send beacon
    PF_BeaconMsg *beaconMsg = PF_BeaconMsg::init(&msg);
    beaconMsg->foxId = FOX_ID;
    beaconMsg->seq = seq++;
    txLen = sizeof(*beaconMsg);
    vw_send((byte*)beaconMsg, txLen);
    vw_wait_tx();
  }
  pktsSent++;
  sleepUntil = tinyMillis() + INTER_PKT_DELAY_MS;
}

void gotoPreIdWait() {
  state = ST_PRE_ID_WAIT;
  // Wait another inter-pkt delay
  sleepUntil = tinyMillis() + INTER_PKT_DELAY_MS;
  displayState(state);
}

void doPreIdWait() {
  if(tinyMillisTimerExpired(sleepUntil)) {
    gotoId();
  }
  // keep waiting
}

void gotoId() {
  state = ST_ID;
  idPart = 0;
  spaceNeeded = false;
  displayState(state);
}

void doId() {
  if(morseTx.available()) {
    if(idPart == BEACON_PARTS_LEN) {
      // We're done
      gotoWait();
    } else if(!spaceNeeded) {
      morseTx.write(BEACON_PARTS[idPart++]);
      spaceNeeded = true;
    } else if(spaceNeeded) {
      morseTx.write(' ');
      spaceNeeded = false;
    }
  }
  morseTx.encode();
}
void gotoWait() {
  state = ST_WAIT;
  sleepUntil = tinyMillis() + INTER_MSG_DELAY;
  displayState(state);
}

void doWait() {
  if(!tinyMillisTimerExpired(sleepUntil)) {
    // keep waiting
    return;
  } else {
    gotoPkt();
  }
}

void displayState(byte val) {
  if(val > 7) val = 7;
  digitalWrite(LED1_PIN, val & 0x01);
  digitalWrite(LED2_PIN, val & 0x02);
  digitalWrite(LED4_PIN, val & 0x04);
}

void setup() {
  tinyMillisSetup();

  pinMode(RADIO_PIN, OUTPUT);
  
  morseTx.setspeed(WPM);
  morseTx.setmillis(tinyMillis);
  // setup VirtualWire
  vw_set_tx_pin(RADIO_PIN);
  vw_set_ptt_inverted(true);
  vw_setup(BIT_RATE);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  displayState(7);  // start off with leds lit

  seq = 0;
  gotoInit();
}

void loop() {
  switch(state) {
    case ST_INIT: doInit(); break;
    case ST_PKT:  doPkt();  break;
    case ST_PRE_ID_WAIT: doPreIdWait(); break;
    case ST_ID:   doId();   break;
    case ST_WAIT: doWait(); break;
    default: gotoInit();    break;
  }
}
