/**
 * @brief Simple Radio Library
 */
 
/*Author: Matías Vidal*/

#include "radio.h"

#define DEBUG_LEVEL 1

//-------------------------- Public Methods --------------------------

void Radio::init(void) {
    delay(1500);
    if (!Radio::rf24.begin()) {
        SerialUSB.println(F("Initialization failed"));
    }
    Radio::rf24.setPALevel(RF24_PA_LOW);          // Used only during tests
    SerialUSB.println(F("Set TX power to LOW"));
    Radio::rf24.setPayloadSize(beacon_size);
    Radio::rf24.openWritingPipe(address[node]);
    SerialUSB.print(F("Set writing pipe address: "));
    SerialUSB.println(node);
    Radio::rf24.openReadingPipe(1, address[3]);
    SerialUSB.print(F("Set reading pipe address: "));
    SerialUSB.println(address[0][3]);
    Radio::rf24.stopListening();
}

bool Radio::available(uint8_t pipe) {
    return Radio::rf24.available(&pipe);
}

/**
 * This function sends a packet to the receiver and then it waits for a confirmation,
 * if there is no acknowledge, an error is printed on screen.
 * @param frame[] Data sent to the receiver..
 * @param frame_size Size of the packet to be sent.
 */
void Radio::sendFrame(frame_t *frame, uint32_t frame_size) {
    if (!Radio::rf24.write(&frame, frame_size))
        SerialUSB.println(F("Transmission failed or timed out"));
}

//void Radio::updateBeacon(AtmsData *atmsData, GpsData *gpsData, VectorInt16 *gyroData)
void Radio::updateBeacon(GpsData *gpsData) {
    beacon.node = Radio::node;
    beacon.index = Radio::index;
    beacon.date = gpsData->date;
    beacon.time = gpsData->time;
    beacon.latitude = (float) gpsData->latitude;
    beacon.longitude = (float) gpsData->longitude;
    beacon.altitude_km = (float) gpsData->altitude_km;
    beacon.speed_mps = (float) gpsData->speed_mps;
    beacon.num_sats = gpsData->num_sats;
    ++Radio::index;
}

void Radio::send_data() {
    Radio::sendFrame(&beacon, beacon_size);
}

void Radio::readFrame() {
    uint8_t cmd;
    uint8_t pipe;
    if (Radio::available(pipe)) {
	uint8_t bytes = Radio::rf24.getPayloadSize();
	Radio::rf24.read(&cmd, 1);
        SerialUSB.print(F("Received "));
        SerialUSB.print(bytes);
        SerialUSB.print(F(" bytes on pipe "));
        SerialUSB.println(pipe);
        SerialUSB.print(F("Command: "));
        SerialUSB.println(cmd);
    }
        //memcpy(&beacon, frame, sizeof(beacon));
}

void Radio::ping(uint8_t to) {
    Radio::rf24.openWritingPipe(address[to]);
    Radio::sendFrame(0, sizeof(uint8_t));
}

void Radio::lowPowerMode() {
    Radio::rf24.powerDown();
}

void Radio::normalMode() {
    Radio::rf24.powerUp();
    delay(5);
}
