/**
 * @brief Femto-satellite v3.211109
 */
 
/*Author: Matias Vidal*/

#include "FE.h"
#include "radio.h"
#include "cmds.h"
#include "gps.h"

uint8_t address[][9] = {"Femto 01", "Femto 02", "SUCHAI 2", "SUCHAI 3"};
uint8_t radioNumber = 0;

#define serial SerialUSB

/*Object Definitions*/
FE femto;
Radio radio(RADIO_CE_PIN, RADIO_CSN_PIN, address, radioNumber);
GPS gps;

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

uint8_t frame [100];
char buf[100];
char params[100];
int cmd = 0;

void setup() {
    // Initialize serial communication
    delay(1000);
    serial.begin(115200);
    serial.setTimeout(200);
    while (!serial) {
        ; // wait for serial port to connect.
    }
    // Initialize femto-satellite's systems
    serial.println("Initializing the femto-satellite");
    femto.init();
    gps.init();
    radio.init();
    serial.println("Done.");
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    //gps.updateData();
    //cmd = radio.read_command();
    /*
    if (radio.available()) {
        radio.readFrame(frame);
        //buf = (char *)frame;
        memcpy(buf, frame, sizeof(buf) / sizeof(buf[0]));
        sscanf(buf, "%d %[^\n]s", &cmd, &params);
        executeCommand(cmd, params);
    }
    else if (SerialUSB.available()) {
    */
    if (serial.available()) {
        int i = 0;
        while (serial.available()) {
            buf[i] = serial.read();
            i++;
        }
        buf[i] = 0;
        sscanf(buf, "%d %[^\n]s", &cmd, &params);
        executeCommand(cmd, params);
    }
    delay(10);
}

/**
 * Enables the low power mode of operation
 * of the femto-satellite.
 * Currently it only puts the radio to sleep,
 * but it is expected to also put the
 * microcontroller into this state.
 */
void lowPowerMode() {
    serial.println("Low power mode selected");
    radio.lowPowerMode();
}

/**
 * Enables the normal mode of operation.
 */
void normalMode() {
    serial.println("Normal mode selected");
    radio.normalMode();
}

/**
 * This function displays all the femto-satellite's
 * commands. These commands are the same for both
 * the user (via serial) and the OBC (via I2C).
 */
void help() {
    serial.println(F("Available commands:"));
    serial.print(UPDATE_DATA);
    serial.println(F(": UPDATE_DATA"));
    serial.print(SEND_BEACON);
    serial.println(F(": SEND_BEACON"));
    serial.print(DEPLOY_FEMTOSATS);
    serial.println(F(": DEPLOY_FEMTOSATS"));
    serial.print(FOD_GET_STATUS);
    serial.println(F(": FOD_GET_STATUS"));
    serial.print(SEND_FEMTOSAT_DATA);
    serial.println(F(": SEND_FEMTOSAT_DATA"));
    serial.print(GET_CONFIG);
    serial.println(F(": GET_CONFIG"));
    serial.print(GET_VERSION);
    serial.println(F(": GET_VERSION"));
    serial.print(ENABLE_LOW_POWER_MODE);
    serial.println(F(": ENABLE_LOW_POWER_MODE"));
    serial.print(DISABLE_LOW_POWER_MODE);
    serial.println(F(": DISABLE_LOW_POWER_MODE"));
    serial.print(HELP);
    serial.println(F(": HELP"));
}

void executeCommand(int cmd, char params[]) {
    serial.print("Received: ");
    serial.print(cmd);
    serial.print("   Command: ");
    if (cmd == UPDATE_DATA) {
        serial.print("UPDATE_DATA");
        serial.print("   Parameters: ");
        serial.println(params);
        femto.updateData(params);
        serial.print("Data: ");
        serial.print("Date");
        serial.print(femto.gpsData.date);
        serial.print("    HH:MM:SS:CC ");
        serial.print(femto.gpsData.time);
        serial.print("    Location: ");
        serial.print(femto.gpsData.latitude, 6);
        serial.print(",");
        serial.print(femto.gpsData.longitude, 6);
        serial.print("    Altitude (GPS): ");
        serial.println(femto.gpsData.altitude_km, 6);
        serial.print("    Sat: ");
        serial.print(femto.gpsData.num_sats);
    }
    else if (cmd == SEND_BEACON) {
        serial.println("SEND_BEACON");
        //radio.updateBeacon(&gps.gpsData);
        radio.send_data();
    }
    else if (cmd == SEND_FEMTOSAT_DATA) {
        serial.println("SEND_FEMTOSAT_DATA");
        //radio.updateBeacon(&gps.gpsData);
        radio.send_data();
    }
    else if (cmd == GET_VERSION) {
        serial.println("GET_VERSION");
        serial.print("Current version is: ");
        serial.println(femto.version);
    }
    else if (cmd == ENABLE_LOW_POWER_MODE) {
        serial.println("ENABLE_LOW_POWER_MODE");
        lowPowerMode();
    }
    else if (cmd == DISABLE_LOW_POWER_MODE) {
        serial.println("DISABLE_LOW_POWER_MODE");
        normalMode();
    }
    else if (cmd == HELP) {
        serial.println("HELP");
        help();
    }
    else if (cmd == 12) {
        serial.println("DONE");
    }
    else {
        serial.print(cmd);
        serial.println(" is not available yet");
    }
    //cmd = 0;
}
