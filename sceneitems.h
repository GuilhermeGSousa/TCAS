#ifndef POINTER_H
#define POINTER_H

#include <QWidget>
#include <QGraphicsLineItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QVector3D>
#include <broadcaster.h>
#include <QPointF>
#include <QString>
#include "conversions.h"

#define MAXRANGE 6.0 //In nm
#define MAXVSPD 5900.0 //Ft/min
#define G 9.81
#define ACC_INCR 0.35*G //1000Ft/min/s?
#define DEGINC 5.0 //Degrees

typedef enum {TA,
              RA,
              PT,
              OT
    }Advisory;
class SceneItems:public QGraphicsItem
{

public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    SceneItems(qreal width, qreal height, qreal length);
    void advance(int phase);
    void addIntruder(Message m);
    void updateIntruders();
    Message getSelf() const;
    void goUp();
    void goDown();
    void goLeft();
    void goRight();
    void setStart(qreal X, qreal Y, qreal Z, qreal V);
    void setCRC(uint32_t crc);

private:
    int RA_sense(QVector3D *i, QVector3D *i_spd, qreal v, qreal a, qreal t);
    qreal stopAccel(qreal v, qreal a, qreal t, int sense);
    qreal ownAltAt(qreal v, qreal a, qreal t, int sense);
    void computeTCAStimes(QVector3D *intr, QVector3D *intr_spd, qreal* t2cpa, qreal* t2coa, QPointF* pos_rel, QPointF* vel_rel);
    Advisory issue_TA_RA(Message* intruder);
    void complementResolutions(Message* intruder);
    bool areResolutionsComplementary(Message* intruder);
    void computeResolutionStrength(QVector3D *intr, QVector3D *intr_spd);
    void advanceStatus(Message* intruder, Advisory result);
    void rotatePointer(qreal rotation);
    bool isIdInList(int id);
    void drawIntruders(QPainter *painter);
    void drawTarget(QPainter *painter, qreal v_min);
    qreal converter(qreal v, qreal v0, qreal a0, qreal v1, qreal a1);
    QVector3D llh_pos;
    qreal getDistanceToSelf(Message intruder);
    qreal width,length,height;
    qreal x1,x2,y1,y2; //Line variables
    qreal acc_z, v_E, v_N, v_U, bearing; //Bearing in degrees, acc and speeds in enu for display, convert on send
    qreal vmax;
    int sl,tau_TA,tau_RA;
    qreal zthr_TA,zthr_RA,dmod_TA,dmod_RA,alim,taumod_TA, taumod_RA;
    qreal ang,intruder_scale,plane_scale, indicator_scale;
    QPixmap PT_image, TA_image, OT_image, RA_image, plane_image, indicator_image;
    QPixmap blue_arrow_down, blue_arrow_up, red_arrow_down, red_arrow_up, yellow_arrow_down, yellow_arrow_up;
    Message self;
    QList<Message> intruder_list;
    QVector3D me;
};

#endif // POINTER_H
