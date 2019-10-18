#ifndef GPSSDATA_H
#define GPSSDATA_H

struct GpsData{
    int hour;
    int minute;
    int second;
    int satellites;

    float latitude;
    float longitude;
    float altitude;
};

#endif
