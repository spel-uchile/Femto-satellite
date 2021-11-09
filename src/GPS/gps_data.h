#ifndef GPSSDATA_H
#define GPSSDATA_H

struct GpsData{
    uint32_t date;
    uint32_t time;
    float latitude;
    float longitude;
    float altitude_km;
    float speed_mps;
    uint32_t num_sats;
};

#endif
