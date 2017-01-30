#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <QtMath>
#include <QVector3D>


#define NM2M 1852.0
#define MPS2FPM 196.850394
#define FT2M 0.3048
#define RT 6378137.0 // Raio da terra metros

typedef struct{
    float lat, lon;
} LatLon;

typedef struct{
    float N, E,U;
} NEU;

LatLon xyz2llh(float x, float y, float z);
NEU xyz2neu(float x, float y, float z,
            float x_intruso, float y_intruso, float z_intruso, float lat, float lon);

NEU xyz2neu(float x, float y, float z,
            float x_intruso, float y_intruso, float z_intruso);

QVector3D llh2wgs(qreal lat, qreal lon, qreal alt);

QVector3D enu2wgs(qreal e, qreal n, qreal u, qreal lat, qreal lon);//Para a velocidade

QVector3D wgs2enu(qreal x, qreal y, qreal z);

QVector3D wgs2enu(qreal x, qreal y, qreal z, qreal lat, qreal lon);

QVector3D wgs2llh(qreal x, qreal y, qreal z);
#endif // CONVERSIONS_H
