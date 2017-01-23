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

#define SPD_INCR 1.0
#define NM2M 1852.0
#define MAXRANGE 6.0 //In nm
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
    void rotatePointer(qreal rotation);
    bool isIdInList(int id);
    void drawIntruders(QPainter *painter);
    void setupSelf();
    QVector3D ECEF2ENU(QVector3D);
    qreal getDistanceToSelf(Message intruder);
    qreal width,length,height;
    qreal x1,x2,y1,y2;
    qreal ang,intruder_scale,plane_scale;
    QPixmap intruder_image,plane_image;
    Message self;
    QList<Message> intruder_list;
};

#endif // POINTER_H