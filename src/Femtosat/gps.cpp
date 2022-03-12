/**
 * @brief Simple GPS Library
 */
 
/*Author: Matias Vidal*/

#include "gps.h"

GPS::GPS(void) {
    gps_baudrate = 9600;
}

GPS::GPS(int baudrate) {
    gps_baudrate = baudrate;
}

void GPS::init() {
    Serial1.begin(gps_baudrate);
    pinMode(GPS_EN, OUTPUT);
    digitalWrite(GPS_EN, LOW);
}

void GPS::updateData() {
    while (Serial1.available() > 0) {
        if (gps.encode(Serial1.read())) {
            //checkValidity();
            gpsData.date = gps.date.value();
            gpsData.time = gps.time.value();
            gpsData.latitude = gps.location.lat()*1000000;
            gpsData.longitude = gps.location.lng()*1000000;
            gpsData.altitude = gps.altitude.meters();
            gpsData.num_sats = gps.satellites.value();
        }
    }
    if (millis() > 5000 && gps.charsProcessed() < 10) {
        SerialUSB.println(F("No GPS detected: check wiring."));
    }
}

uint8_t GPS::checkValidity(void) {
    uint8_t validity = 0;
    validity = gps.location.isValid()*(0b1000000);
    validity += gps.altitude.isValid()*(0b0100000);
    validity += gps.time.isValid()*(0b00100000);
    validity += gps.satellites.isValid()*(0b00010000);
    return validity;
}

void GPS::powerUp(void) {
    digitalWrite(GPS_EN, HIGH);
}

void GPS::powerDown(void) {
    digitalWrite(GPS_EN, LOW);
}
