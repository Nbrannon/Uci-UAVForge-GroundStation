#ifndef TELEMETRYPOINT_H
#define TELEMETRYPOINT_H

#include <QPair>

/**
 * @brief The TelemetryPoint class was intended to store the data values
 * needed to describe a data point.
 * @deprecated Could not be sent over a Qt signal
 * @todo Repace with a struct used by the server and mapexecution(?)
 * internally after working out protocall details with Avionics and Radio/Comms
 * teams.
 * @author Jordan Dickson
 * @date 10-9-2015
 */

class TelemetryPoint
{
public:

    DEPRECATED_ATTRIBUTE
    TelemetryPoint(double lat, double lng, long time);

    DEPRECATED_ATTRIBUTE
    TelemetryPoint(QPair<double, double> coord, long time);

    DEPRECATED_ATTRIBUTE
    ~TelemetryPoint();


    DEPRECATED_ATTRIBUTE
    QPair<double,double> getCoords();
    DEPRECATED_ATTRIBUTE
    long getTime();

private:
    DEPRECATED_ATTRIBUTE
    QPair<double,double> coord;
    DEPRECATED_ATTRIBUTE
    long time;
};

#endif // TELEMETRYPOINT_H
