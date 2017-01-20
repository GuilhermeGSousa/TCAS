#include "pointer.h"


Pointer::Pointer(qreal width,qreal height,qreal length)
{
    this->width = width;
    this->length = length;
    this->height = height;
    x1=this->width/2;
    y1=this->height/2;

    ang=0;
}

void Pointer::advance(int phase){

    if(!phase) return;
    rotatePointer(ang*qreal(M_PI)/180.0);
    ang += 0.5;

}


void Pointer::rotatePointer(qreal rotation){
    qreal x_local= -length*qCos(-rotation);
    qreal y_local= length*qSin(-rotation);


    qreal center_x=width/2;
    qreal center_y=height/2;


    x2=center_x-x_local;
    y2=center_y-y_local;

    this->update();
}

QRectF Pointer::boundingRect() const
{
    return QRectF(0,0,(qreal)width,(qreal)height);
}

void Pointer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen whitePen(Qt::white);
    whitePen.setWidth(10);
    painter->setPen(whitePen);
    const QLineF* pointer = new QLineF(x1,y1,x2,y2);

    painter->drawLine(*pointer);

}
