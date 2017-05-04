#include <PikoFox.h>
#include <OS2017_Common.h>

#include <SoftwareSerial.h>

/*Include the VirtualWire library */
#include <VirtualWire.h>

const byte FOX_Id = OS2017_FOX0_ID;  // What fox do we want to listen to...
/* Digital IO pin that will be used for receiving data from the receiver */
const int RX_DIO_Pin = 1;
const int LED_Pin = 4;
const int RX_Pin = 0;
const int TX_Pin = 3;

unsigned long last_seq = 0;

#define too_many_loops_wo_msg 10000
unsigned long loops_since_msg = 0;

SoftwareSerial Serial(RX_Pin,TX_Pin);


PF_AnyMsg msg;
uint8_t msgLen = sizeof(msg);

void setup()
{
    pinMode(LED_Pin, OUTPUT);
    digitalWrite(LED_Pin, HIGH);
    delay(1000);
    digitalWrite(LED_Pin, LOW);

    /* Initialises the DIO pin used to receive data from the Rx module */
    vw_set_rx_pin(RX_DIO_Pin);

    /* bits per second */
    vw_setup(OS2017_FOX_BIT_RATE);

    /* Enable the receiver */
    vw_rx_start();

    Serial.begin(9600);
    Serial.println("Serial Works!");
}

/* Main program */
void loop()
{
    PF_Msg *baseMsg = NULL;
    msgLen = sizeof(msg);
    if(vw_get_message((uint8_t *) &msg, &msgLen)) { // Non-blocking
        Serial.println("Found a fox!");
        for(int i = 0; i < msgLen; ++i) {
          Serial.print(((char*)&msg)[i]);
        }
        Serial.print("\n");

        baseMsg = msg.toMsg(msgLen);
    }

    if(baseMsg && (baseMsg->foxId == FOX_Id || baseMsg->foxId == OS2017_TEST_FOX_ID)) {
      if(baseMsg->seq - last_seq > 100) {
        // missed 100 messages
        digitalWrite(LED_Pin, LOW);
      } else {
        digitalWrite(LED_Pin, HIGH);
      }
      last_seq = baseMsg->seq;
      loops_since_msg = 0;
    } else if(loops_since_msg++ > too_many_loops_wo_msg) {
      digitalWrite(LED_Pin, LOW);
      loops_since_msg = 0;
    }
}
