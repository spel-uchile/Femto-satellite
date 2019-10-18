#include "FE.h"

unsigned long t0, dt;
float version;
char lat[12], lng[12], alt[12];
int hour, min, sec, sats;

//-------------------------- Public Methods --------------------------
/**
 * Initialize all the pins of the deployment
 * system.
 */
void FE::init(void) {
    version = 2.0;
}

void FE::updateData(char data[]) {
    sscanf(data, "%d %d %d %d %s %s %s",
           &hour, &min, &sec, &sats, &lat, &lng, &alt);
    gpsData.hour = hour;
    gpsData.minute = min;
    gpsData.second = sec;
    gpsData.satellites = sats;
    gpsData.latitude = ((String) lat).toFloat();
    gpsData.longitude = ((String) lng).toFloat();
    gpsData.altitude = ((String) alt).toFloat();
}
