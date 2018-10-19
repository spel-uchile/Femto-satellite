/**
 * @brief Simple Radio Example
 */
 
/*Author: Matias Vidal*/

#include "radio.h"
#include "gps.h"
#include "femto_pins.h"

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

/*Object Definitions*/
Radio radio(RADIO_SLAVESELECTPIN, RADIO_INTERRUPT, RADIO_RST, CLIENT_ADDRESS, SERVER_ADDRESS);
GPS gps;

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

uint8_t base_cmd;

void setup() {
    // initialize serial communication
    Serial.begin(115200);
    delay(1800);
    // init radio
    Serial.println("Init femto-satellite");
    gps.init();
    radio.init();
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
	gps.updateData();
	base_cmd = radio.read_command(); 
	if (base_cmd == SEND_DATA) {
      	radio.updateBeacon(&gps.gpsData);
      	radio.send_data();
	}
    else if (base_cmd == SEND_PICTURE) {
        
    }
    else if (base_cmd == SEND_BEACON) {
        radio.updateBeacon(&gps.gpsData);
        radio.send_data();
    }
    else if (base_cmd == PING) {
        radio.ack_ping();
    }
    else if (base_cmd == ENABLE_LOW_POWER_MODE) {
        
    }
    else if (base_cmd == DISABLE_LOW_POWER_MODE) {
        
    }
	base_cmd = 0;
    delay(500);
}
