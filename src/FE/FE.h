/**
 * @class FE
 * @brief Class that manages the femto-satellite
 * @author: Matías Vidal
 */

/*Requiered Libraries*/
#include <Arduino.h>
#include "femto_pins.h"
#include "gps_data.h"

class FE {
    /*Private Members*/

    // FOD Objects

    // Internal Variables
public:
    /*Public Members*/
    GpsData gpsData;

    unsigned long t0, dt;
    float version;
    char lat[12], lng[12], alt[12];
    int hour, min, sec, sats;

    /*Base contructor (null)*/
    FE() {}

    // methods
    void init(void);
    void updateData(char data[]);

// private:
    // methods
};
