/**
 * @brief Simple GPS Library
 */

/*Author: Matias Vidal*/

/*Requiered Libraries*/
#include <Arduino.h>
#include <TinyGPSPlus.h>
#include "gps_data.h"

#define GPS_EN 4
#define GPS_RST 3

/**
 * @class GPS
 * @brief Class for manage GPS data
 */

 class GPS {
    /*Private Members*/
    
    // GPS object
    TinyGPSPlus gps;

    public:
        /*Public Members*/
        GpsData gpsData;
        
        // Default constructor
        GPS(void);
        
        // Overload constructor
        GPS(int);

        // Mutator functions
        void init(void);
        void updateData(void);
        uint8_t checkValidity(void);
        void powerUp(void);
        void powerDown(void);

    private:
        int gps_baudrate;
 };
