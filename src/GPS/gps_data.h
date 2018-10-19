#ifndef GPSSDATA_H
#define GPSSDATA_H

struct GpsData{
    double latitude;
    double longitude;
    double altitude;
    double crse;
    double mps;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t satellites;
    uint8_t validity;
};

#endif