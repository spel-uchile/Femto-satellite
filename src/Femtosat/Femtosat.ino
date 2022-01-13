#include <SPI.h>
#include "wiring_private.h"
#include "printf.h"
#include "RF24.h"
#include "PNI_RM3100.h"

#define serial SerialUSB
#define RADIO_CE_PIN A4//A6
#define RADIO_CSN_PIN A3//A5
#define START_TRANSMISSION 1
#define MAG_SCK  13
#define MAG_MISO 12
#define MAG_MOSI 11
#define MAG_CS   2

SPIClass mySPI (&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);
// Instantiate an object for the nRF24L01 transceiver
RF24 radio(RADIO_CE_PIN, RADIO_CSN_PIN, 1000000);
// Instantiate an object for tje RM3100 Magnetometer
PNI_RM3100 mag(MAG_CS, &mySPI);

uint8_t address[4][8] = {"Femto 1", "Femto 2", "SUCHAI2", "SUCHAI3"};
uint8_t radioNumber = 0;

// Used to control whether this node is sending or receiving
bool role = false;  // true = TX role, false = RX role

uint32_t cmd = 0;

typedef struct{
    uint32_t node = 0;
    uint32_t index = 0;
    uint32_t date = 241121;
    uint32_t time = 16132231;
    int32_t latitude = -3345782;
    int32_t longitude = -7066167;
    int32_t altitude_km = 5055;
    uint32_t num_sats = 15;
} gnss_data_t;
gnss_data_t gnss_data;

typedef struct{
    uint32_t node = 0;
    uint32_t index = 0;
    int32_t fe_mag_x = 5782;
    int32_t fe_mag_y = 6167;
    int32_t fe_mag_z = 9055;
} mag_data_t;
mag_data_t mag_data;

int ccx = 800; /**< Count Cycles for x sensor measurement. It must be between 30 an 400 (default 200) */
int ccy = 800; /**< Count Cycles for y sensor measurement. It must be between 30 an 400 (default 200) */
int ccz = 800; /**< Count Cycles for z sensor measurement. It must be between 30 an 400 (default 200) */

void setup() {
    /*
    mySPI.begin();
    pinPeripheral(11, PIO_SERCOM_ALT);
    pinPeripheral(12, PIO_SERCOM_ALT);
    pinPeripheral(13, PIO_SERCOM);
    */
    pinMode(MAG_CS, OUTPUT);
    digitalWrite(MAG_CS, HIGH);
    pinMode(RADIO_CSN_PIN, OUTPUT);
    digitalWrite(RADIO_CSN_PIN, LOW); 
    serial.begin(115200);
    while (!serial) {
    }
delay(1000);
    if (!radio.begin(&mySPI, RADIO_CE_PIN, RADIO_CSN_PIN)) {
        serial.println(F("radio hardware is not responding!!"));
        while (1) {} // hold in infinite loop
    }
    serial.println(F("Initializing radio hardware..."));
    radio.setPALevel(RF24_PA_MAX);//LOW);  // RF24_PA_MAX is default.
    radio.setPayloadSize(sizeof(gnss_data));
    radio.openWritingPipe(address[radioNumber]);
    radio.openReadingPipe(1, address[3]);
    radio.setDataRate(RF24_250KBPS);
    radio.startListening(); // put radio in RX mode
    serial.println(F("Done."));
    // For debugging info
    //printf_begin();             // needed only once for printing details
    // radio.printDetails();       // (smaller) function that prints raw register values
    //radio.printPrettyDetails(); // (larger) function that prints human readable data
    delay(500);
    if (!mag.begin()) {
        serial.println(F("Could not find a valid RM3100 sensor, check wiring or "
                     "try a different address!"));
        while (1) delay(10);
    }
    serial.println(F("RM_3100 connected"));
    serial.println(F("Initializing the magnetometer..."));
    /* Default settings from datasheet. */
    mag.setSampling(PNI_RM3100::MODE_CMMXYZ,     /* Operating Mode. MODE_SINGLE MODE_CMMXYZ MODE_CMMX MODE_CMMY MODE_CMMZ*/
                    PNI_RM3100::TMRC_37HZ,     /* Max frequency */
                    ccx,    /**< Count Cycles for x sensor measurement. It must be between 30 an 400 (default 200) */
                    ccy,    /**< Count Cycles for y sensor measurement. It must be between 30 an 400 (default 200) */
                    ccz);   /**< Count Cycles for z sensor measurement. It must be between 30 an 400 (default 200) */
    serial.println(F("Done."));
} // setup

void loop() {
    if (role) {
        if (cmd == 0) {
            radio.setPayloadSize(sizeof(gnss_data));
            // This device is a TX node
            unsigned long start_timer = micros();                     // start the timer
            bool report = radio.write(&gnss_data, sizeof(gnss_data)); // transmit & save the report
            unsigned long end_timer = micros();                       // end the timer
            if (report) {
                serial.print(F("Transmission successful! "));          // payload was delivered
                serial.print(F("Time to transmit = "));
                serial.print(end_timer - start_timer);                 // print the timer result
                serial.println(F(" us. Sent: "));
                serial.print(F("Node: "));
                serial.print(gnss_data.node);
                serial.print(F(" Index: "));
                serial.print(gnss_data.index);
                serial.print(F(" Date: "));
                serial.print(gnss_data.date);
                serial.print(F(" Time: "));
                serial.print(gnss_data.time);
                serial.print(F(" Latitude: "));
                serial.print(gnss_data.latitude);
                serial.print(F(" Longitude: "));
                serial.print(gnss_data.longitude);
                serial.print(F(" Altitude: "));
                serial.print(gnss_data.altitude_km);
                serial.print(F(" Number of sats.: "));
                serial.print(gnss_data.num_sats);
                serial.print(F("\n"));
                gnss_data.index++;

                role = false;
                radio.startListening();
                }
                else {
                     serial.println(F("Transmission failed or timed out")); // payload was not delivered
                }
            }
        else {
            updateMagData();
            radio.setPayloadSize(sizeof(mag_data));
            // This device is a TX node
            unsigned long start_timer = micros();                     // start the timer
            bool report = radio.write(&mag_data, sizeof(mag_data));   // transmit & save the report
            unsigned long end_timer = micros();                       // end the timer
            if (report) {
                serial.print(F("Transmission successful! "));          // payload was delivered
                serial.print(F("Time to transmit = "));
                serial.print(end_timer - start_timer);                 // print the timer result
                serial.println(F(" us. Sent: "));
                serial.print(F("Node: "));
                serial.print(mag_data.node);
                serial.print(F(" Index: "));
                serial.print(mag_data.index);
                serial.print(F(" Magnetometer X: "));
                serial.print(mag_data.fe_mag_x);
                serial.print(F(" Magnetometer Y: "));
                serial.print(mag_data.fe_mag_y);
                serial.print(F(" Magnetometer Z: "));
                serial.print(mag_data.fe_mag_z);
                serial.print(F("\n"));
                mag_data.index++;

                role = false;
                //lowPowerMode();
                //delay(10);
                //normalMode();
                //delay(10);
                radio.startListening();
                }
                else {
                     serial.println(F("Transmission failed or timed out")); // payload was not delivered
                }
            }
    delay(10);
    }
    else {
    // This device is a RX node
    radio.setPayloadSize(sizeof(cmd));
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      //radio.setPayloadSize(sizeof(uint32_t));
      uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      radio.read(&cmd, bytes);            // fetch payload from FIFO
      serial.print(F("Received "));
      serial.print(bytes);                    // print the size of the payload
      serial.print(F(" bytes on pipe "));
      serial.print(pipe);                     // print the pipe number
      serial.print(F(": "));
      serial.print(cmd);                // print the payload's value
      serial.print(F("   Binary: "));
      serial.println(cmd, BIN);

      role = true;
      //serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      radio.stopListening();
    }
  } // role
} // loop

/**
 * Enables the low power mode of operation
 * of the femto-satellite.
 * Currently it only puts the radio to sleep,
 * but it is expected to also put the
 * microcontroller into this state.
 */
void lowPowerMode() {
    serial.println("Low power mode selected");
    radio.powerDown();
}

/**
 * Enables the normal mode of operation.
 */
void normalMode() {
    serial.println("Normal mode selected");
    radio.powerUp();
}

void updateMagData() {
    bool checking;
    RM3100_measurements data;
    checking = mag.check_masurement();
    if (checking) {
        data = mag.readMagneticField();
        mag_data.fe_mag_x = data.mag_x;
        mag_data.fe_mag_y = data.mag_y;
        mag_data.fe_mag_z = data.mag_z;
    }
}
