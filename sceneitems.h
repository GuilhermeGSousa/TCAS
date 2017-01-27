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
#include "conversions.h"

#define MAXRANGE 6.0 //In nm
#define MAXVSPD 6000.0 //Ft/min
#define G 9.81
#define ACC_INCR 5.08 //1000Ft/min/s?
#define DEGINC 5.0 //Degrees

typedef enum {TA,
              RA,
              PT,
              NT
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

private:
    int RA_sense(Message* i, qreal v, qreal a, qreal t);
    qreal stopAccel(qreal v, qreal a, qreal t, int sense);
    qreal ownAltAt(qreal v, qreal a, qreal t, int sense);
    bool correctiveRA(Message* intruder, int sense);
    void computeTCAStimes(Message* intruder, qreal* t2cpa, qreal* t2coa);
    Advisory issue_TA_RA(Message* intruder);
    void complementResolutions(Message* intruder);
    void areResolutionsComplementary(Message* intruder);
    void computeResolutionStrength(Message* intruder);
    void advanceStatus(Message* intruder, Advisory result);
    void rotatePointer(qreal rotation);
    bool isIdInList(int id);
    void drawIntruders(QPainter *painter);
    QVector3D llh_pos;
    qreal getDistanceToSelf(Message intruder);
    qreal width,length,height;
    qreal x1,x2,y1,y2; //Line variables
    qreal acc_z, v_E, v_N, v_U, bearing; //Bearing in degrees, acc and speeds in enu for display, convert on send
    qreal vmax;
    int sl,tau_TA,tau_RA;
    qreal zthr_TA,zthr_RA,dmod_TA,dmod_RA,alim,taumod_TA, taumod_RA;
    qreal ang,intruder_scale,plane_scale, indicator_scale;
    QPixmap intruder_image,plane_image, indicator_image;
    Message self;
    QList<Message> intruder_list;
};

#endif // POINTER_H
