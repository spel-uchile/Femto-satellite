/*
  Test.h - Test library for Wiring - description
  Copyright (c) 2006 John Doe.  All right reserved.
*/
#ifndef __RM3100_H__
#define __RM3100_H__

#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>

/*!
    I2C ADDRESS/BITS/SETTINGS
*/
#define RM3100_ADDRESS (0x20) /**< The default I2C address for the sensor. */
#define RM3100_ADDRESS_ALT1 (0x21) /**< 1st Alternative I2C address for the sensor. */
#define RM3100_ADDRESS_ALT2 (0x22) /**< 2nd Alternative I2C address for the sensor. */
#define RM3100_ADDRESS_ALT3 (0x23) /**< 3th Alternative I2C address for the sensor. */

//  Forward declarations of Wire and SPI for board/variant combinations that
//  don't have a default 'Wire' or 'SPI'
extern TwoWire Wire; /**< Forward declaration of Wire object */
extern SPIClass SPI; /**< Forward declaration of SPI object */

/*!
   Registers available on the sensor.
*/
enum {
  // Count Cycles Write Registers
  RM3100_REGISTER_CMM = 0x01,
  RM3100_REGISTER_CMX = 0x04,
  RM3100_REGISTER_CLX = 0x05,
  RM3100_REGISTER_CMY = 0x06,
  RM3100_REGISTER_CLY = 0x07,
  RM3100_REGISTER_CMZ = 0x08,
  RM3100_REGISTER_CLZ = 0x09,
  RM3100_REGISTER_TMRC = 0x0B,


  // Count Cycles Read Registers
  RM3100_REGISTER_RCMX = 0x84,
  RM3100_REGISTER_RCLX = 0x85,
  RM3100_REGISTER_RCMY = 0x86,
  RM3100_REGISTER_RCLY = 0x87,
  RM3100_REGISTER_RCMZ = 0x88,
  RM3100_REGISTER_RCLZ = 0x89,
  RM3100_REGISTER_RTMRC = 0x8B,

  // Test Registers
  RM3100_REGISTER_TEST = 0xB6,

  // STATUS Register
  RM3100_REGISTER_STATUS = 0xB4,
};

/*!
    Struct to hold calibration data.
*/
typedef struct {
  float bias_x; /**< bias measured for x sensor measurement. */
  float slope_x;  /**<  slope measured for x sensor measurement. */
  float bias_y; /**< bias measured for y sensor measurement. */
  float slope_y;  /**<  slope measured for x sensor measurement. */
  float bias_z; /**< bias measured for z sensor measurement. */
  float slope_z;  /**<  slope measured for x sensor measurement. */
} RM3100_calib_data;

/*!
    Struct to hold the measurements.
*/
typedef struct {
  long mag_x; /**< x axis magnetic field measurement. */
  long mag_y;  /**<  y axis magnetic field measurement. */
  long mag_z; /**< z axis magnetic field measurement. */
} RM3100_measurements;

class PNI_RM3100
{
  public:
  
    /** Operating mode for the sensor. */
    enum sensor_mode 
    {
      /** Single measurement mode. */
      MODE_SINGLE = 0x78,
      /** Contunuous Measurment mode XYZ sensors. */
      MODE_CMMXYZ = 0x79,
      /** Contunuous Measurment mode X sensor */
      MODE_CMMX = 0x49,
      /** Contunuous Measurment mode Y sensor */
      MODE_CMMY = 0x51,
      /** Contunuous Measurment mode Z sensor */
      MODE_CMMZ = 0x61,
    };  
    
    /** Max Sampling Rate Continuous Measurement Mode. */
    enum max_sensor_sampling 
    {
      /** max 600 Hz. */
      TMRC_600HZ = 0x92,
      /** max 300 Hz. */
      TMRC_300HZ = 0x93,
      /** max 150 Hz. */
      TMRC_150HZ = 0x94,
      /** max 75 Hz. */
      TMRC_75HZ = 0x95,
      /** max 37 Hz (Default). */
      TMRC_37HZ = 0x96,
    };

    PNI_RM3100(TwoWire *theWire = &Wire);
    PNI_RM3100(int8_t cspin, SPIClass *theSPI = &SPI);
    PNI_RM3100(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin);

    bool begin(uint8_t addr = RM3100_ADDRESS);

    void setSampling(sensor_mode mode = MODE_SINGLE,
                     max_sensor_sampling maxsampling = TMRC_37HZ,
                     int ccx = 200,
                     int ccy = 200,
                     int ccz = 200);

    bool check_masurement(void);
    
    RM3100_measurements readMagneticField(void);

  private:
    TwoWire *_wire; /**< Wire object */
    SPIClass *_spi; /**< SPI object */

    uint8_t spixfer(uint8_t x);
    void write8(byte reg, byte value);
    void write16(byte reg, int value);
    uint8_t read8(byte reg);

    uint8_t _i2caddr;

    int8_t _cs, _mosi, _miso, _sck;

};
#endif
