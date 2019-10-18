/**
 * @brief Simple Radio Library
 */

/*Author: Gustavo Diaz, Matias Vidal*/

/*Requiered Libraries*/
#include <Arduino.h>
#include <RHReliableDatagram.h>
#include <SPI.h>
#include <RH_RF69.h>
#include "gps_data.h"

/**
 * @class Radio
 * @brief Class for manage Radio data
 */

typedef struct {
    /*
    float Temp1;
    float Pressure;
    float Alt;
    float Temp2;
    float Humidity;
    float Temp3;
    float IMU1;
    float IMU2;
    float IMU3;
    */
    uint8_t GPS_HH;
    uint8_t GPS_MM;
    uint8_t GPS_SS;
    uint32_t GPS_Sat;
    float GPS_Lat;
    float GPS_Lng;
    float GPS_Alt;
} frame_t;

class Radio {
    /*Private Members*/
    frame_t beacon;
    frame_t beacon_tx_;

    uint8_t beacon_tx_size_ = sizeof(beacon_tx_);

    // Radio Object
    RH_RF69 driver;

    // Class to manage message delivery and receipt, using the driver declared above.
    RHReliableDatagram rf69;

    // Shutdown pin
    uint8_t rst_pin_;

    // addr
    uint8_t addr_;
    uint8_t addr2_;
    

    // Debug
    // HardwareSerial *debug_port_;

public:
    /*Public Members*/

    /*constructor de base (null)*/
    // Radio() {}

    // constructror parametrizado
    Radio(uint8_t radio_slaveselectpin, uint8_t radio_interrupt, uint8_t rst, uint8_t addr, uint8_t addr2):
        driver(radio_slaveselectpin, radio_interrupt),
        addr_(addr),
        addr2_(addr2),
        rf69(driver, addr),
        rst_pin_(rst)
        // debug_port_(debug_port)
    {
        // Config pins
        pinMode(rst_pin_, OUTPUT);
    }

    // methods
    void init(void);
    //void updateBeacon(AtmsData *atmsData, GpsData *gpsData, VectorInt16 *gyroData);
    bool available(void);
    void updateBeacon(GpsData * gpsData);
    void send_data();
    void ping(uint8_t to);
    void ack_ping();
    bool send_command(uint8_t cmd);
    uint8_t read_command(void);
    void sendFrame(uint8_t frame[], int frame_size);
    void displayData(double dataD[], float dataF[], uint8_t dataU8[], uint32_t dataU32);
    void readFrame(uint8_t* frame);
    bool lowPowerMode(void);
    void normalMode(void);

private:
    // methods
};
