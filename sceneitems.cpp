#include "sceneitems.h"
#include <QDebug>

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
    intruder_image = QPixmap::fromImage(QImage(":/PT"));
    intruder_scale=0.5;

    plane_image =  QPixmap::fromImage(QImage(":/plane"));
}

void SceneItems::advance(int phase){

    //Main loop here

    if(!phase) return;

    ang=self.Z_spd*qreal(M_PI)/100.0;
    if(ang>qreal(M_PI))
        ang=qreal(M_PI);
    if(ang<-qreal(M_PI))
        ang=-qreal(M_PI);

    rotatePointer(ang);


}

void SceneItems::addIntruder(Message m)
{
    //Add new intruder to list or update intruder messages
    for(int i = 0; i < intruder_list.length(); i++){
        if(m.Ac_id == intruder_list[i].Ac_id){
            intruder_list.replace(i,m);
            return;
        }
    }
    intruder_list.append(m);
}

void SceneItems::updateIntruders()
{
    //Remove intruders too far away
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

    int plane_width = plane_image.width();
    int plane_height = plane_image.height();
    qreal center_x = width/2-plane_width*(plane_scale/2);
    qreal center_y = height/2+plane_height*plane_scale*1.5;


    foreach(Message i, intruder_list){
        qreal x = i.X_pos/NM2M;
        qreal y = i.Y_pos/NM2M;

        //Limit=6nm

        painter->drawPixmap(mapToParent(center_x+x*length/MAXRANGE-intruder_image.width()*intruder_scale/2,
                                        center_y-y*length/MAXRANGE-intruder_image.height()*intruder_scale/2).x(),
                            mapToParent(center_x+x*length/MAXRANGE-intruder_image.width()*intruder_scale/2,
                                        center_y-length/MAXRANGE-intruder_image.height()*intruder_scale/2).y(),
                            intruder_image.width()*intruder_scale,
                            intruder_image.height()*intruder_scale,
                            intruder_image);
    }
}

void SceneItems::setupSelf()
{
    self.X_pos = 0;
    self.Y_pos = 0;
    self.Z_pos = 0;
}

QVector3D SceneItems::ECEF2ENU(QVector3D vec)
{

}

qreal SceneItems::getDistanceToSelf(Message intruder)
{
    sqrt(pow(self.X_pos-intruder.X_pos,2)+
         pow(self.Y_pos-intruder.Y_pos,2)+
         pow(self.Z_pos-intruder.Z_pos,2));
}

Message SceneItems::getSelf() const
{
    return self;
}

void SceneItems::goUp()
{
    self.Z_spd += SPD_INCR;
}

void SceneItems::goDown()
{
    self.Z_spd -= SPD_INCR;
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
    plane_scale = 0.15;
    int plane_width = plane_image.width();
    int plane_height = plane_image.height();
    const QLineF* pointer = new QLineF(x1,y1,x2,y2);

    //Paint Items here
    painter->drawLine(*pointer);
    painter->drawEllipse(width/2-length,height/2-length,length*2,length*2);
    painter->drawPixmap(width/2-plane_width*(plane_scale/2),height/2+plane_height*plane_scale*1.5,
                        plane_width*plane_scale,plane_height*plane_scale,plane_image);

    // Loop to draw all intruders here
    drawIntruders(painter);
}
