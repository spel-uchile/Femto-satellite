/**
 * @brief Simple GPS Library
 */

/*Author: Matias Vidal*/

/*Requiered Libraries*/
#include <Arduino.h>
#include <TinyGPS++.h>
#include "gps_data.h"

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

    private:
        int gps_baudrate;
 };
