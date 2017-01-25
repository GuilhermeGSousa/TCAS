#ifndef POINTER_H
#define POINTER_H

#include <QWidget>
#include <QGraphicsLineItem>
#include <QtMath>
#include <QPainter>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QVector3D>
#include <broadcaster.h>
#include <QPointF>

#define ACC_INCR 0.00508
#define NM2M 1852.0
#define MPS2FPM 196.850394
#define FT2M 0.3048
#define G 9.81
#define MAXRANGE 6.0 //In nm

typedef enum {TA,
              RA
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

private:
    bool RA_sense(Message* i, qreal v, qreal a, qreal t);
    qreal stopACC(qreal v, qreal a, qreal t, int sense);
    qreal ownAltAt(qreal v, qreal a, qreal t, int sense);

    void compute_TA_RA(Message intruder);
    void rotatePointer(qreal rotation);
    bool isIdInList(int id);
    void drawIntruders(QPainter *painter);
    void setupSelf();
    QVector3D ECEF2ENU(QVector3D);
    qreal getDistanceToSelf(Message intruder);
    qreal width,length,height;
    qreal x1,x2,y1,y2; //Line variables
    qreal acc_z, bearing;
    qreal ang,intruder_scale,plane_scale, indicator_scale;
    QPixmap intruder_image,plane_image, indicator_image;
    Message self;
    QList<Message> intruder_list;
};

#endif // POINTER_H
