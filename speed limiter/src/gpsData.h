#ifndef _gpsData
#define _gpsData
struct GpsData {
  bool isValid;
  double lat;
  double lon;
  float speed;
  int numberOfSats;
};
typedef struct GpsData GpsData;
#endif