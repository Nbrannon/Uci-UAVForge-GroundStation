#include "telemetrypoint.h"

/**
 * @deprecated
 * @brief TelemetryPoint::TelemetryPoint
 * @param lat
 * @param lng
 * @param time
 */
DEPRECATED_ATTRIBUTE
TelemetryPoint::TelemetryPoint(double lat, double lng, long time) {
    QPair<double,double> coordinate(lat,lng);
    this->time = time;
}

DEPRECATED_ATTRIBUTE
TelemetryPoint::TelemetryPoint(QPair<double, double> coord, long time) {
    this->coord = coord;
    this->time = time;
}

DEPRECATED_ATTRIBUTE
TelemetryPoint::~TelemetryPoint() {
}

DEPRECATED_ATTRIBUTE
QPair<double,double> TelemetryPoint::getCoords(){
    return coord;
}

DEPRECATED_ATTRIBUTE
long TelemetryPoint::getTime(){
    return time;
}
