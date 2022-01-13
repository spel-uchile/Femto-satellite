/*
  Test.h - Test library for Wiring - implementation
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// include core Wiring API
// include this library's description file
#include "PNI_RM3100.h"
#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include "wiring_private.h"


// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

/*!
   @brief  RM3100 constructor using i2c
   @param  *theWire
           optional wire
*/
PNI_RM3100::PNI_RM3100(TwoWire *theWire)
  : _cs(-1), _mosi(-1), _miso(-1), _sck(-1)
{
  _wire = theWire;
}

/*!
   @brief  RM3100 constructor using hardware SPI
   @param  cspin
           cs pin number
   @param  theSPI
           optional SPI object
*/
PNI_RM3100::PNI_RM3100(int8_t cspin, SPIClass * theSPI)
  : _cs(cspin), _mosi(-1), _miso(-1), _sck(-1)
{
  _spi = theSPI;
}

/*!
   @brief  RM3100 constructor using bitbang SPI
   @param  cspin
           The pin to use for CS/SSEL.
   @param  mosipin
           The pin to use for MOSI.
   @param  misopin
           The pin to use for MISO.
   @param  sckpin
           The pin to use for SCK.
*/
PNI_RM3100::PNI_RM3100(int8_t cspin, int8_t mosipin, int8_t misopin,
                       int8_t sckpin)
  : _cs(cspin), _mosi(mosipin), _miso(misopin), _sck(sckpin)
{

}

/*!
    Initialises the sensor.
    @param addr
           The I2C address to use (default = 0x77)
    @param chipid
           The expected chip ID (used to validate connection).
    @return True if the init was successful, otherwise false.
*/
bool  PNI_RM3100::begin(uint8_t addr)
{
  _i2caddr = addr;

  if (_cs == -1)
  {
    // i2c
    _wire->begin();
  }
  else
  {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);

    if (_sck == -1)
    {
      // hardware SPI
      _spi->begin();

//==============================================
    pinPeripheral(11, PIO_SERCOM);
    pinPeripheral(12, PIO_SERCOM);
    pinPeripheral(13, PIO_SERCOM);
    /*
    digitalWrite(_cs, LOW);
    _spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    uint8_t x = RM3100_REGISTER_TEST; // read, bit 7 high
    _spi->transfer(x);
    uint8_t reply = 0;
    x = 0; // read, bit 7 high
    reply = _spi->transfer(x);
    _spi->endTransaction();
    SerialUSB.begin(115200);
    SerialUSB.println(reply);
    digitalWrite(_cs, HIGH);
    */
//==============================================
      
      
    }
    else
    {
      // software SPI
      pinMode(_sck, OUTPUT);
      pinMode(_mosi, OUTPUT);
      pinMode(_miso, INPUT);
    }
  }
  
  if (read8(RM3100_REGISTER_TEST) == 0)
    return false;

  setSampling();
  delay(100);
  return true;
}

void PNI_RM3100::setSampling(sensor_mode mode,
                             max_sensor_sampling maxsampling,
                             int ccx,
                             int ccy,
                             int ccz)
{
  write16(RM3100_REGISTER_CMX, ccx);
  write16(RM3100_REGISTER_CMY, ccy);
  write16(RM3100_REGISTER_CMZ, ccz);
  write8(RM3100_REGISTER_CMM, mode);
  write8(RM3100_REGISTER_TMRC, maxsampling);
}

bool PNI_RM3100::check_masurement(void)
{
  uint8_t value;

  value = read8(RM3100_REGISTER_STATUS);

  if (bitRead(value, 7))
  {
    return true;
  }
  else
  {
    return false;
  }
}


/*!
    @brief  Reads a
*/
RM3100_measurements PNI_RM3100::readMagneticField(void) {
  byte mx[4];
  byte my[4];
  byte mz[4];

  long mag_valuex = 0;
  long mag_valuey = 0;
  long mag_valuez = 0;

  RM3100_measurements measurements;

  if (_cs == -1)
  {
    _wire->beginTransmission((uint8_t)_i2caddr);
    _wire->write((uint8_t)0xA4& ~0x80);
    _wire->endTransmission();
    _wire->requestFrom((uint8_t)_i2caddr, (byte)9);

    mx[1] = _wire->read();
    mx[2] = _wire->read();
    mx[3] = _wire->read();
    my[1] = _wire->read();
    my[2] = _wire->read();
    my[3] = _wire->read();
    mz[1] = _wire->read();
    mz[2] = _wire->read();
    mz[3] = _wire->read();
  }

  else
  {
    if (_sck == -1)
      _spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    spixfer(0xA4); // read, bit 7 high

    mx[1] = spixfer(0);
    mx[2] = spixfer(0);
    mx[3] = spixfer(0);
    my[1] = spixfer(0);
    my[2] = spixfer(0);
    my[3] = spixfer(0);
    mz[1] = spixfer(0);
    mz[2] = spixfer(0);
    mz[3] = spixfer(0);

    digitalWrite(_cs, HIGH);
    if (_sck == -1)
      _spi->endTransaction(); // release the SPI bus
  }

  if (mx[1] >= B10000000)
  {
    mx[0] = 0xFF;
  }
  else
  {
    mx[0] = 0x00;
  }
  mag_valuex += (long)mx[0] << 24;
  mag_valuex += (long)mx[1] << 16;
  mag_valuex += (long)mx[2] << 8;
  mag_valuex += (long)mx[3];


  if (my[1] >= B10000000)
  {
    my[0] = 0xFF;
  }
  else
  {
    my[0] = 0x00;
  }
  mag_valuey += (long)my[0] << 24;
  mag_valuey += (long)my[1] << 16;
  mag_valuey += (long)my[2] << 8;
  mag_valuey += (long)my[3];

  if (mz[1] >= B10000000)
  {
    mz[0] = 0xFF;
  }
  else
  {
    mz[0] = 0x00;
  }
  mag_valuez += (long)mz[0] << 24;
  mag_valuez += (long)mz[1] << 16;
  mag_valuez += (long)mz[2] << 8;
  mag_valuez += (long)mz[3];

  measurements.mag_x = mag_valuex;
  measurements.mag_y = mag_valuey;
  measurements.mag_z = mag_valuez;

  return measurements;
}

uint8_t PNI_RM3100::spixfer(uint8_t x)
{
  if (_sck == -1)
    return _spi->transfer(x);

  // software spi
  //SerialUSB.println("Software SPI");
  //SerialUSB.println(_sck);
  uint8_t reply = 0;
  for (int i = 7; i >= 0; i--)
  {
    reply <<= 1;
    digitalWrite(_sck, LOW);
    digitalWrite(_mosi, x & (1 << i));
    digitalWrite(_sck, HIGH);
    if (digitalRead(_miso))
      reply |= 1;
  }

  return reply;
}


/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over I2C/SPI
*/
/**************************************************************************/
void PNI_RM3100::write8(byte reg, byte value) {
  if (_cs == -1) {
    _wire->beginTransmission((uint8_t)_i2caddr);
    _wire->write((uint8_t)reg);
    _wire->write((uint8_t)value);
    _wire->endTransmission();
  } else {
    if (_sck == -1)
      _spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    spixfer(reg & ~0x80); // write, bit 7 low
    spixfer(value);
    digitalWrite(_cs, HIGH);
    if (_sck == -1)
      _spi->endTransaction(); // release the SPI bus
  }
}


/**************************************************************************/
/*!
    @brief  Writes an 16 bit value over I2C/SPI
*/
/**************************************************************************/
void PNI_RM3100::write16(byte reg, int value) {
  if (_cs == -1) {
    _wire->beginTransmission((uint8_t)_i2caddr);
    _wire->write((uint8_t)reg);
    _wire->write(highByte(value));
    _wire->write(lowByte(value));
    _wire->endTransmission();
  } else {
    if (_sck == -1)
      _spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    spixfer(reg & ~0x80); // write, bit 7 low
    spixfer(highByte(value));
    spixfer(lowByte(value));
    digitalWrite(_cs, HIGH);
    if (_sck == -1)
      _spi->endTransaction(); // release the SPI bus
  }
}

/*!
    @brief  Reads an 8 bit value over I2C/SPI
    @param  reg
            selected register
    @return value from selected register
*/
uint8_t PNI_RM3100::read8(byte reg) {
  uint8_t value;

  if (_cs == -1) {
    _wire->beginTransmission((uint8_t)_i2caddr);
    _wire->write((uint8_t)reg & ~0x80);
    _wire->endTransmission();
    _wire->requestFrom((uint8_t)_i2caddr, (byte)1);
    value = _wire->read();

  } else {
    if (_sck == -1)
      _spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    spixfer(reg); // read, bit 7 high
    value = spixfer(0);
    digitalWrite(_cs, HIGH);
    if (_sck == -1)
      _spi->endTransaction(); // release the SPI bus
  }
  return value;
}
