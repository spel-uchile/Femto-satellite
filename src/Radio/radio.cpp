/**
 * @brief Simple Radio Library
 */
 
/*Author: Gustavo Diaz, Matias Vidal*/

#include "radio.h"

#define DEBUG_LEVEL 1

//-------------------------- Public Methods --------------------------

void Radio::init(void) {
    //digitalWrite(rst_pin_, LOW);
    delay(1500);
    if (!rf69.init()) {
        Serial.println(F("Initialization failed"));
    }
    //driver.setCRCPolynomial(driver.CRC_CCITT);
    driver.setHeaderFlags(0x7E); 
    driver.setFrequency(430.00, 0.05); 
    driver.setTxPower(20);
    if (!driver.setModemConfig(driver.FSK_Rb2Fd5)) {
        Serial.println(F("Configuration error"));    
    }
    rf69.setRetries(3);
    Serial.println(F("Set Tx Power = 20 DBm"));
    Serial.println(F("Set configuration = FSK_Rb2Fd5"));  
}

/**
 * This function sends a packet to the receiver and then it waits for a confirmation,
 * if there is no acknowledge, an error is printed on screen.
 * @param data[] Data sent to the receiver..
 * @param data_size Size of the packet to be sent.
 */
void Radio::sendFrame(uint8_t frame[], int frame_size) {
    if (!rf69.sendtoWait(frame, frame_size, addr2_))
        Serial.println(F("sendtoWait failed"));
}

//void Radio::updateBeacon(AtmsData *atmsData, GpsData *gpsData, VectorInt16 *gyroData)
void Radio::updateBeacon(GpsData *gpsData) {
    /*	
    beacon_tx_.Temp1 = (float) atmsData->temperature1;
    beacon_tx_.Pressure = (float) atmsData->pressure;
    beacon_tx_.Alt = (float) atmsData->altitude;
    beacon_tx_.Temp2 = atmsData->temperature2;
    beacon_tx_.Humidity = atmsData->humidity;
    beacon_tx_.Temp3 = atmsData->temperatureDallas;
    beacon_tx_.IMU1 = gyroData->x;
    beacon_tx_.IMU2 = gyroData->y;
    beacon_tx_.IMU3 = gyroData->z;
    */
    beacon_tx_.GPS_Lat = (float) gpsData->latitude;
    beacon_tx_.GPS_Lng = (float) gpsData->longitude;
    beacon_tx_.GPS_Alt = (float) gpsData->altitude;
    beacon_tx_.GPS_Crse = (float) gpsData->crse;
    beacon_tx_.GPS_Speed = (float) gpsData->mps;
    beacon_tx_.GPS_HH = gpsData->hour;
    beacon_tx_.GPS_MM = gpsData->minute;
    beacon_tx_.GPS_SS = gpsData->second;
    beacon_tx_.GPS_validity = gpsData->validity;
    beacon_tx_.GPS_Sat = gpsData->satellites;
}

void Radio::send_data() {
    sendFrame((uint8_t *)&beacon_tx_, beacon_tx_size_);
}

void Radio::read_frame() {
    uint8_t frame[50];
    if (rf69.available()) {
        uint8_t len = sizeof(frame);
        uint8_t from;
        if (rf69.recvfromAck(frame, &len, &from)) {
            memcpy(&beacon, frame, sizeof(beacon));
            displayFrame();
        }
    }
}

void Radio::displayFrame() {
    /*
    Serial.print("RTC HH:MM:SS: ");
    Serial.print(beacon.RTC_HH);
    Serial.print(":");
    Serial.print(beacon.RTC_MM);
    Serial.print(":");
    Serial.print(beacon.RTC_SS);
    
    Serial.print("    Temp1: ");
    Serial.print(beacon.Temp1);
    Serial.print("    Pressure: ");
    Serial.print(beacon.Pressure);
    Serial.print("    Altitude: ");
    Serial.print(beacon.Alt);
    
    Serial.print("    Temp2: ");
    Serial.print(beacon.Temp2);
    Serial.print("    Humidity: ");
    Serial.print(beacon.Humidity);
    Serial.print("    Temp3: ");
    Serial.print(beacon.Temp3);
    Serial.print("    IMU1: ");
    Serial.print(beacon.IMU1);
    Serial.print("    IMU2: ");
    Serial.print(beacon.IMU2);
    Serial.print("    IMU3: ");
    Serial.println(beacon.IMU3);
    */
    Serial.print("HH:MM:SS: ");
    Serial.print(beacon.GPS_HH);
    Serial.print(":");
    Serial.print(beacon.GPS_MM);
    Serial.print(":");
    Serial.print(beacon.GPS_SS);
    Serial.print("    Validity: ");
    Serial.print(beacon.GPS_validity, BIN);
    Serial.print("    Sat: ");
    Serial.print(beacon.GPS_Sat);
    Serial.print("    Location: ");
    Serial.print(beacon.GPS_Lat, 6);
    Serial.print(",");
    Serial.print(beacon.GPS_Lng, 6);
    Serial.print("    Altitude (GPS): ");
    Serial.print(beacon.GPS_Alt);
    Serial.print("    Course: ");
    Serial.print(beacon.GPS_Crse);
    Serial.print("    Speed: ");
    Serial.println(beacon.GPS_Speed);
}

void Radio::ping(uint8_t to) {
    uint8_t frame[5] = "ping";
    rf69.sendtoWait(frame, sizeof(frame), to);
}

void Radio::ack_ping() {
    uint8_t frame[50];
    if (rf69.available()) {
        uint8_t len = sizeof(frame);
        uint8_t from;
        if (rf69.recvfromAck(frame, &len, &from)) {
            rf69.sendtoWait(frame, len, from);
        }
    }
}

bool Radio::send_command(uint8_t cmd) {
    return rf69.sendtoWait((uint8_t *)&cmd, 1, addr2_);
}

uint8_t Radio::read_command(void) {
    uint8_t cmd;
    if (rf69.available()) {
        uint8_t len;
        uint8_t from;
        rf69.recvfromAck((uint8_t *)&cmd, &len, &from);
        return cmd;
    }
}

bool Radio::lowPowerMode() {
	driver.sleep();
}
