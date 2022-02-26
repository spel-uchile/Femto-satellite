/**
 * @brief Simple Temperature Library
 */
 
/*Author: Matias Vidal*/

#include <Wire.h>
#include "temp.h"

TEMP::TEMP(void) {
    address = 0x18;
}

TEMP::TEMP(uint8_t addr) {
    address = addr;
}

void TEMP::init() {
    Wire.begin();
}

uint16_t TEMP::read16(uint8_t reg) {
    uint16_t byte1, byte2;
    Wire.beginTransmission(address);
    Wire.write(reg & ~0x80);
    Wire.endTransmission();
    Wire.requestFrom(address, (byte)2);
    byte1 = (uint16_t) Wire.read() << 8;
    byte2 = (uint16_t) Wire.read();// << 8;
    return byte1 + byte2;
}

float TEMP::updateData() {
    uint16_t read1, read2, ivalue, sign;
    uint8_t l_temp, h_temp;
    float temp;
    Wire.beginTransmission(address);
    Wire.write(0x05);
    Wire.endTransmission();
    Wire.requestFrom(address, (byte)2);
    read1 = Wire.read() << 8;
    read2 = Wire.read();// << 8;
    /*
    SerialUSB.print("Address: ");
    SerialUSB.print(address, HEX);
    SerialUSB.print("    read1: ");
    SerialUSB.print(read1, BIN);
    SerialUSB.print("    read2: ");
    SerialUSB.print(read2, BIN);
    */
    ivalue = read1 + read2;
    /*
    SerialUSB.print("ivalue: ");
    SerialUSB.print(ivalue, BIN);
    temp = (0x1FFF & ivalue);
    l_temp = (0x00FF & temp);
    SerialUSB.print("    l_temp: ");
    SerialUSB.print(l_temp, BIN);
    h_temp = (0xFF00 & temp);
    SerialUSB.print("    h_temp: ");
    SerialUSB.println(h_temp, BIN);
    sign = (0x1000 & temp);
    if (sign == 0)
        return (float) (h_temp >> 4) + (float) l_temp*two_el_to_minus4;
    else
        return 256.0 - (float) (h_temp >> 4) + (float) l_temp*two_el_to_minus4;
    */
    temp = ivalue & 0x0FFF;
    temp /= 16.0;
    if (ivalue & 0x1000)
        temp -= 256;
    return temp;
}

bool TEMP::checkValidity(void) {
    uint16_t value1 = read16(0x06);
    uint16_t value2 = read16(0x07);
    if ((value1 == 84) && (value2 == 1024))
        return true;
    return false;
}
