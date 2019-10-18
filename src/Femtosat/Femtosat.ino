/**
 * @brief Femto-satellite v1.0
 */
 
/*Author: Matias Vidal*/

#include "FE.h"
#include "radio.h"
#include "cmds.h"
#include "gps.h"

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

/*Object Definitions*/
FE femto;
Radio radio(RADIO_SLAVESELECTPIN, RADIO_INTERRUPT, RADIO_RST, CLIENT_ADDRESS, SERVER_ADDRESS);
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
    Serial.begin(115200);
    SerialUSB.setTimeout(200);
    while (!SerialUSB) {
        ; // wait for serial port to connect.
    }
    // Initialize femto-satellite's systems
    Serial.println("Initializing the femto-satellite");
    femto.init();
    gps.init();
    radio.init();
    SerialUSB.println("Done.");
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
    if (SerialUSB.available()) {
        int i = 0;
        while (SerialUSB.available()) {
            buf[i] = SerialUSB.read();
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
 * @return True if succesfull.
 */
bool lowPowerMode() {
    SerialUSB.println("Low power mode selected");
    return false;//radio.lowPowerMode();
}

/**
 * Enables the normal mode of operation.
 */
void normalMode() {
    SerialUSB.println("Normal mode selected");
    //radio.normalMode();
}

/**
 * This function displays all the femto-satellite's
 * commands. These commands are the same for both
 * the user (via serial) and the OBC (via I2C).
 */
void help() {
    SerialUSB.println(F("Available commands:"));
    SerialUSB.print(UPDATE_DATA);
    SerialUSB.println(F(": UPDATE_DATA"));
    SerialUSB.print(SEND_BEACON);
    SerialUSB.println(F(": SEND_BEACON"));
    SerialUSB.print(DEPLOY_FEMTOSATS);
    SerialUSB.println(F(": DEPLOY_FEMTOSATS"));
    SerialUSB.print(FOD_GET_STATUS);
    SerialUSB.println(F(": FOD_GET_STATUS"));
    SerialUSB.print(SEND_FEMTOSAT_DATA);
    SerialUSB.println(F(": SEND_FEMTOSAT_DATA"));
    SerialUSB.print(SET_ON_TIME);
    SerialUSB.println(F(": SET_ON_TIME"));
    SerialUSB.print(GET_CONFIG);
    SerialUSB.println(F(": GET_CONFIG"));
    SerialUSB.print(GET_VERSION);
    SerialUSB.println(F(": GET_VERSION"));
    SerialUSB.print(ENABLE_LOW_POWER_MODE);
    SerialUSB.println(F(": ENABLE_LOW_POWER_MODE"));
    SerialUSB.print(DISABLE_LOW_POWER_MODE);
    SerialUSB.println(F(": DISABLE_LOW_POWER_MODE"));
    SerialUSB.print(HELP);
    SerialUSB.println(F(": HELP"));
}

void executeCommand(int cmd, char params[]) {
    SerialUSB.print("Received: ");
    SerialUSB.print(cmd);
    SerialUSB.print("   Command: ");
    if (cmd == UPDATE_DATA) {
        SerialUSB.print("UPDATE_DATA");
        SerialUSB.print("   Parameters: ");
        SerialUSB.println(params);
        femto.updateData(params);
        SerialUSB.print("Data: ");
        SerialUSB.print("HH:MM:SS: ");
        SerialUSB.print(femto.gpsData.hour);
        SerialUSB.print(":");
        SerialUSB.print(femto.gpsData.minute);
        SerialUSB.print(":");
        SerialUSB.print(femto.gpsData.second);
        SerialUSB.print("    Sat: ");
        SerialUSB.print(femto.gpsData.satellites);
        SerialUSB.print("    Location: ");
        SerialUSB.print(femto.gpsData.latitude, 6);
        SerialUSB.print(",");
        SerialUSB.print(femto.gpsData.longitude, 6);
        SerialUSB.print("    Altitude (GPS): ");
        SerialUSB.println(femto.gpsData.altitude, 6);
    }
    else if (cmd == SEND_BEACON) {
        SerialUSB.println("SEND_BEACON");
        //radio.updateBeacon(&gps.gpsData);
        //radio.send_data();
    }
    else if (cmd == SEND_FEMTOSAT_DATA) {
        SerialUSB.println("SEND_FEMTOSAT_DATA");
        //radio.updateBeacon(&gps.gpsData);
        //radio.send_data();
    }
    else if (cmd == GET_VERSION) {
        SerialUSB.println("GET_VERSION");
        SerialUSB.print("Current version is: ");
        SerialUSB.println(femto.version, 1);
    }
    else if (cmd == ENABLE_LOW_POWER_MODE) {
        SerialUSB.println("ENABLE_LOW_POWER_MODE");
        //lowPowerMode();
    }
    else if (cmd == DISABLE_LOW_POWER_MODE) {
        SerialUSB.println("DISABLE_LOW_POWER_MODE");
        //normalMode();
    }
    else if (cmd == HELP) {
        SerialUSB.println("HELP");
        help();
    }
    else if (cmd == 12) {
        SerialUSB.println("DONE");
    }
    else {
        SerialUSB.print(cmd);
        SerialUSB.println(" is not available yet");
    }
    //cmd = 0;
}
