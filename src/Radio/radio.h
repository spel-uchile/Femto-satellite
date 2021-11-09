/**
 * @brief Simple Radio Library
 */

/*Author: Matías Vidal*/

/*Requiered Libraries*/
#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"
#include "gps_data.h"

/**
 * @class Radio
 * @brief Class for manage Radio data
 */

typedef struct {
    uint32_t node;
    uint32_t index;
    uint32_t date;
    uint32_t time;
    float latitude;
    float longitude;
    float altitude_km;
    float speed_mps;
    uint32_t num_sats;
} frame_t;

class Radio {
    /*Private Members*/
    RF24 rf24;
    frame_t beacon;
    uint32_t beacon_size = sizeof(beacon);
    uint32_t index;
    uint8_t ce_pin;
    uint8_t csn_pin;
    uint8_t node;
    uint8_t address[][9];

public:
    /*Public Members*/

    /*constructor de base (null)*/
    // Radio() {}

    // constructor parametrizado
    Radio(uint8_t radio_ce_pin, uint8_t radio_csn_pin, uint8_t address[][9], uint8_t radioNumber):rf24(ce_pin, csn_pin)
    {
        ce_pin = radio_ce_pin;
        csn_pin = radio_csn_pin;
        index = 0;
        address = address;
        node = radioNumber;
        
        // Config pins
        pinMode(radio_ce_pin, OUTPUT);
	    pinMode(radio_csn_pin, OUTPUT);
    }
    // methods
    void init(void);
    bool available(uint8_t pipe);
    void updateBeacon(GpsData * gpsData);
    void send_data(void);
    void ping(uint8_t to);
    void sendFrame(frame_t *frame, uint32_t frame_size);
    void displayData(double dataD[], float dataF[], uint8_t dataU8[], uint32_t dataU32);
    void readFrame(void);
    void lowPowerMode(void);
    void normalMode(void);

private:
    // methods
};
