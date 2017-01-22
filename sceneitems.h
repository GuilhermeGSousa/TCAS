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
#include <broadcaster.h>

class SceneItems:public QGraphicsItem
{

public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    SceneItems(qreal width, qreal height, qreal length);
    void advance(int phase);
    bool isIdInList(int id);
private:
    void rotatePointer(qreal rotation);
    qreal width,length,height;
    qreal x1,x2,y1,y2;
    qreal ang;
    QPixmap intruder_image,plane_image;
    QList<Message> intruder_list;
};

#endif // POINTER_H
