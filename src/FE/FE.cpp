#include "FE.h"

unsigned long t0, dt;
float version;
char lat[12], lng[12], alt[12], speed[12];
uint32_t date, time, sats;

//-------------------------- Public Methods --------------------------
/**
 * Initialize all the pins of the deployment
 * system.
 */
void FE::init(void) {
    version = 3.211109;
}

void FE::updateData(char data[]) {
    sscanf(data, "%d %d %s %s %s %s %d",
           &date, &time, &lat, &lng, &alt, &speed, &sats);
    gpsData.date = date;
    gpsData.time = time;
    gpsData.latitude = ((String) lat).toFloat();
    gpsData.longitude = ((String) lng).toFloat();
    gpsData.altitude_km = ((String) alt).toFloat();
    gpsData.speed_mps = ((String) speed).toFloat();
    gpsData.num_sats = sats;
}
