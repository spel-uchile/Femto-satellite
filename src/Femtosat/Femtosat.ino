#include <SPI.h>
#include <Wire.h>
//#include "printf.h"
#include "RF24.h"
#include "PNI_RM3100.h"
#include "gps.h"
#include "temp.h"

#define serial SerialUSB
#define STATUS_LED 9
#define RADIO_CE_PIN A4
#define RADIO_CSN_PIN A3
#define MAG_RDY 5
#define TRX_IRQ 8
#define TEMP_ALERT 2
#define START_TRANSMISSION 1

SPIClass mySPI (&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);
// Instantiate an object for the nRF24L01 transceiver
RF24 radio(RADIO_CE_PIN, RADIO_CSN_PIN, 1000000);
// Instantiate an object for the RM3100 Magnetometer
PNI_RM3100 mag;
// Instantiate an object for the JAVAD GNSS receiver
GPS javad(9600);
// Instantiate an object for the MCP9808 temperature sensor
TEMP temp(0x18);

uint8_t address[4][8] = {"Femto 1", "Femto 2", "SUCHAI2", "SUCHAI3"};
uint8_t radioNumber = 0;

// Used to control whether this node is sending or receiving
bool role = false;  // true = TX role, false = RX role

bool mag_debug = true;
bool temp_debug = true;
bool gps_debug = true;
bool trx_debug = true;
uint32_t cmd = 0;
const uint32_t max_samples = 10;
uint32_t transmit_index = 0;

typedef struct{
    uint32_t node = 0;
    uint32_t index = 0;
    uint32_t date = 241121;
    uint32_t time = 16132231;
    int32_t latitude = -3345782;
    int32_t longitude = -7066167;
    int32_t altitude = 505500;
    uint32_t num_sats = 15;
} gnss_data_t;
gnss_data_t gnss_data[max_samples];

typedef struct{
    uint32_t node = 0;
    uint32_t index = 0;
    int32_t fe_mag_x = 5782;
    int32_t fe_mag_y = 6167;
    int32_t fe_mag_z = 9055;
    float temp = 25.0;
} mag_data_t;
mag_data_t mag_data[max_samples];

int ccx = 3200; /**< Count Cycles for x sensor measurement. It must be between 30 an 400 (default 200) */
int ccy = 3200; /**< Count Cycles for y sensor measurement. It must be between 30 an 400 (default 200) */
int ccz = 3200; /**< Count Cycles for z sensor measurement. It must be between 30 an 400 (default 200) */

void setup() {
    delay(1000);
    Wire.begin();
    //pinMode(MAG_CS, OUTPUT);
    //digitalWrite(MAG_CS, HIGH);
    pinMode(RADIO_CSN_PIN, OUTPUT);
    digitalWrite(RADIO_CSN_PIN, LOW);
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(RADIO_CSN_PIN, HIGH);
    pinMode(MAG_RDY, INPUT);
    serial.begin(115200);
    javad.init();
    temp.init();
    if (!mag.begin()) {
        serial.println(F("Could not find a valid RM3100 sensor, check wiring or "
                     "try a different address!"));
    }
    serial.println(F("RM_3100 connected"));
    serial.println(F("Initializing the magnetometer..."));
    /* Default settings from datasheet. */
    mag.setSampling(PNI_RM3100::MODE_CMMXYZ,     /* Operating Mode. MODE_SINGLE MODE_CMMXYZ MODE_CMMX MODE_CMMY MODE_CMMZ*/
                    PNI_RM3100::TMRC_HZ,         /* Max frequency */
                    ccx,    /**< Count Cycles for x sensor measurement. It must be between 30 an 400 (default 200) */
                    ccy,    /**< Count Cycles for y sensor measurement. It must be between 30 an 400 (default 200) */
                    ccz,    /**< Count Cycles for z sensor measurement. It must be between 30 an 400 (default 200) */
                    1);
    serial.println(F("Done."));
    updateData(9);
    while (!radio.begin(&mySPI, RADIO_CE_PIN, RADIO_CSN_PIN)) {
        serial.println(F("Radio hardware is not responding!!"));
        delay(1000);
    }
    serial.println(F("Initializing radio hardware..."));
    radio.setPALevel(RF24_PA_MAX);//LOW);  // RF24_PA_MAX is default.
    radio.setPayloadSize(sizeof(gnss_data[max_samples]));
    radio.openWritingPipe(address[radioNumber]);
    radio.openReadingPipe(1, address[3]);
    radio.setDataRate(RF24_250KBPS);
    radio.startListening(); // put radio in RX mode
    serial.println(F("Done."));
    // For debugging info
    //printf_begin();             // needed only once for printing details
    // radio.printDetails();      // (smaller) function that prints raw register values
    //radio.printPrettyDetails(); // (larger) function that prints human readable data
} // setup

void loop() {
    if (role) {
        if (cmd == 0) {
            sendGPSData(transmit_index);
        }
        else if (cmd == 1){
            sendMagData(transmit_index);
        }
        else {
            updateData(max_samples);
        }
        delay(10);
        transmit_index++;
        if (transmit_index > max_samples)
            transmit_index = 0;
    }
    else {
        receiveCMD();
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
    serial.println(F("Low power mode selected"));
    radio.powerDown();
    digitalWrite(STATUS_LED, LOW);
}

/**
 * Enables the normal mode of operation.
 */
void normalMode() {
    serial.println(F("Normal mode selected"));
    radio.powerUp();
}

void sendGPSData(uint32_t index) {
    radio.setPayloadSize(sizeof(gnss_data[index]));
    //updateGPSData();
    unsigned long start_timer = micros();                     // start the timer
    bool report = radio.write(&gnss_data[index], sizeof(gnss_data[index])); // transmit & save the report
    unsigned long end_timer = micros();                       // end the timer
    if (report) {
        if (trx_debug) {
            serial.print(F("Transmission successful! Time to transmit = ")); // payload was delivered
            serial.print(end_timer - start_timer);                           // print the timer result
            serial.println(F(" us. Sent: Node: "));
            serial.print(gnss_data[index].node);
            serial.print(F(" Index: "));
            serial.print(gnss_data[index].index);
            serial.print(F(" Date: "));
            serial.print(gnss_data[index].date);
            serial.print(F(" Time: "));
            serial.print(gnss_data[index].time);
            serial.print(F(" Latitude: "));
            serial.print(gnss_data[index].latitude);
            serial.print(F(" Longitude: "));
            serial.print(gnss_data[index].longitude);
            serial.print(F(" Altitude: "));
            serial.print(gnss_data[index].altitude);
            serial.print(F(" Number of sats.: "));
            serial.print(gnss_data[index].num_sats);
            serial.print(F("\n"));
        }
        //gnss_data.index++;
        role = false;
        radio.startListening();
    }
    else {
        serial.println(F("Transmission failed or timed out")); // payload was not delivered
    }
}

void sendMagData(uint32_t index) {
    //updateMagData();
    //updateTempData();
    radio.setPayloadSize(sizeof(mag_data[index]));
    unsigned long start_timer = micros();                     // start the timer
    bool report = radio.write(&mag_data[index], sizeof(mag_data[index]));   // transmit & save the report
    unsigned long end_timer = micros();                       // end the timer
    if (report) {
        if (trx_debug) {
            serial.print(F("Transmission successful! Time to transmit = ")); // payload was delivered
            serial.print(end_timer - start_timer);                           // print the timer result
            serial.println(F(" us. Sent: Node: "));
            serial.print(mag_data[index].node);
            serial.print(F(" Index: "));
            serial.print(mag_data[index].index);
            serial.print(F(" Magnetometer X: "));
            serial.print(mag_data[index].fe_mag_x);
            serial.print(F(" Magnetometer Y: "));
            serial.print(mag_data[index].fe_mag_y);
            serial.print(F(" Magnetometer Z: "));
            serial.print(mag_data[index].fe_mag_z);
            serial.print(F("\n"));
        }
        //mag_data.index++;
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

void receiveCMD() {
    // This device is a RX node
    radio.setPayloadSize(sizeof(cmd));
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
        //radio.setPayloadSize(sizeof(uint32_t));
        uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
        radio.read(&cmd, bytes);            // fetch payload from FIFO
        if (trx_debug) {
            serial.print(F("Received "));
            serial.print(bytes);                    // print the size of the payload
            serial.print(F(" bytes on pipe "));
            serial.print(pipe);                     // print the pipe number
            serial.print(F(": "));
            serial.print(cmd);                // print the payload's value
            serial.print(F("   Binary: "));
            serial.println(cmd, BIN);
        }
        role = true;
        radio.stopListening();
    }
}

void updateGPSData(uint32_t index) {
    digitalWrite(STATUS_LED, HIGH);
    if (Serial1.available()) {
        javad.updateData();
        javad.checkValidity();
        gnss_data[index].index = index;
        gnss_data[index].date = javad.gpsData.date;
        gnss_data[index].time = javad.gpsData.time;
        gnss_data[index].latitude = javad.gpsData.latitude;
        gnss_data[index].longitude = javad.gpsData.longitude;
        gnss_data[index].altitude = javad.gpsData.altitude;
        gnss_data[index].num_sats = javad.gpsData.num_sats;
        if (gps_debug) {
            serial.print(F("Node: "));
            serial.print(gnss_data[index].node);
            serial.print(F("  Index: "));
            serial.print(gnss_data[index].index);
            serial.print(F("  Date: "));
            serial.print(gnss_data[index].date);
            serial.print(F("  Time: "));
            serial.print(gnss_data[index].time);
            serial.print(F(" Lat: "));
            serial.print(gnss_data[index].latitude);
            serial.print(F("° Lng: "));
            serial.print(gnss_data[index].longitude);
            serial.print(F("° Alt:"));
            serial.print(gnss_data[index].altitude);
            serial.print(F(" [m] Sats: "));
            serial.println(gnss_data[index].num_sats);
        }
    }
    digitalWrite(STATUS_LED, LOW);
}

void updateMagData(uint32_t index) {
    digitalWrite(STATUS_LED, HIGH);
    bool checking;
    RM3100_measurements data;
    checking = mag.check_masurement();
    if (checking) {
        data = mag.readMagneticField();
        mag_data[index].index = index;
        mag_data[index].fe_mag_x = data.mag_x;
        mag_data[index].fe_mag_y = data.mag_y;
        mag_data[index].fe_mag_z = data.mag_z;
        if (mag_debug) {
            serial.print(F("Node: "));
            serial.print(mag_data[index].node);
            serial.print(F("  Index: "));
            serial.print(mag_data[index].index);
            serial.print(F("  Mag X: "));
            serial.print(mag_data[index].fe_mag_x);
            serial.print(F("  Mag Y: "));
            serial.print(mag_data[index].fe_mag_y);
            serial.print(F("  Mag Z: "));
            serial.print(mag_data[index].fe_mag_z);
        }
    }
    digitalWrite(STATUS_LED, LOW);
}

void updateTempData(uint32_t index) {
    if (temp.checkValidity()) {
        digitalWrite(STATUS_LED, HIGH);
        mag_data[index].temp = temp.updateData();
        if (temp_debug) {
            serial.print(F("  Temp: "));
            serial.print(mag_data[index].temp);
            serial.println(F(" [°C]"));
        }
    }
    else
        serial.println(F("Data invalid!"));
    digitalWrite(STATUS_LED, LOW);
}

void updateData(uint32_t samples) {
    /*
    radio.stopListening();
    radio.powerDown();
    digitalWrite(RADIO_CE_PIN, LOW);
    delay(5);
    */
    javad.powerUp();
    digitalWrite(STATUS_LED, LOW);
    int ttff = 20000;
    int sampling_period = 100;
    uint32_t i;
    for (i=0; i < ((int) ttff/sampling_period); i++) {
        updateGPSData(0);
        delay(sampling_period);
    }
    i = 0;
    while (i < samples) {
        updateGPSData(i);
        if (digitalRead(MAG_RDY)) {
            updateMagData(i);
            updateTempData(i);
            i++;
        }
        delay(980);
    }
    javad.powerDown();
    radio.powerUp();
    radio.startListening();
}
