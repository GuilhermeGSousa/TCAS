#include "sceneitems.h"


SceneItems::SceneItems(qreal width,qreal height,qreal length)
{
    this->width = width;
    this->length = length;
    this->height = height;
    x1=this->width/2;
    y1=this->height/2;
    x2=x1-length;
    y2=y1;
    ang=0;
    intruder_image = QPixmap::fromImage(QImage(":/rhombus"));
    plane_image =  QPixmap::fromImage(QImage(":/plane"));
}

void SceneItems::advance(int phase){

    //Main loop here

    if(!phase) return;
    rotatePointer(ang*qreal(M_PI)/180.0);
    ang += 0.5;

}

void SceneItems::addIntruder(Message m)
{
    if(isIdInList(m.Ac_id))
        return;
    else
        intruder_list.append(m);
}

bool SceneItems::isIdInList(int id)
{
    //TODO check if this id exists in the intruder list
    foreach(Message m, intruder_list){
        if(m.Ac_id==id)
            return true;
    }
    return false;

}

void SceneItems::drawIntruders(QPainter *painter)
{

}



void SceneItems::rotatePointer(qreal rotation){
    qreal x_local= length*qCos(rotation);
    qreal y_local= length*qSin(rotation);


    qreal center_x=width/2;
    qreal center_y=height/2;


    x2=center_x-x_local;
    y2=center_y-y_local;

    this->update();
}

QRectF SceneItems::boundingRect() const
{
    return QRectF(0,0,(qreal)width,(qreal)height);
}

void SceneItems::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen whitePen(Qt::white);
    QBrush greyBrush(Qt::gray);
    whitePen.setWidth(10);
    painter->setPen(whitePen);
    double scale = 0.3;
    int plane_width = plane_image.width();
    int plane_height = plane_image.height();
    const QLineF* pointer = new QLineF(x1,y1,x2,y2);

    //Paint Items here
    painter->drawLine(*pointer);
    painter->drawEllipse(width/2-length,height/2-length,length*2,length*2);
    painter->drawPixmap(mapToParent(0,0).x(),mapToParent(0,0).y(),intruder_image.width(),intruder_image.height(),intruder_image);
    painter->drawPixmap(width/2-plane_width*(scale/2),height/2+plane_height*scale*1.5,
                        plane_width*scale,plane_height*scale,plane_image);

    // Loop to draw all intruders here
}
