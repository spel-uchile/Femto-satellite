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
    Serial.begin(gps_baudrate);
    //Serial2.begin(gps_baudrate);
}

void GPS::updateData() {
    while (Serial.available()) {
        if (gps.encode(Serial.read())) {
            checkValidity();

            gpsData.date = gps.date.value();
            gpsData.time = gps.time.value();
            gpsData.latitude = gps.location.lat();
            gpsData.longitude = gps.location.lng();
            gpsData.altitude_km = gps.altitude.meters();
	    gpsData.speed_mps = gps.speed.mps();
            gpsData.num_sats = gps.satellites.value();
        }
    }
    if (millis() > 5000 && gps.charsProcessed() < 10) {
        SerialUSB.println(F("No GPS detected: check wiring."));
    }
}

uint8_t GPS::checkValidity(void) {
    uint8_t validity_ = 0;
    validity_ = gps.location.isValid()*(B1000000);
    validity_ += gps.altitude.isValid()*(B0100000);
    validity_ += gps.time.isValid()*(B00100000);
    validity_ += gps.satellites.isValid()*(B00010000);
    return validity_;
}
