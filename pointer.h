#ifndef POINTER_H
#define POINTER_H

#include <QWidget>
#include <QGraphicsLineItem>
#include <QtMath>
#include <QPainter>
#include <QGraphicsScene>

class Pointer:public QGraphicsItem
{

public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    Pointer(qreal width, qreal height, qreal length);
    void advance(int phase);
private:
    void rotatePointer(qreal rotation);
    qreal width,length,height;
    qreal x1,x2,y1,y2;
    qreal ang;
    // QGraphicsItem interface

};

#endif // POINTER_H
