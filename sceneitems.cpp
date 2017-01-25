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
    indicator_image = QPixmap::fromImage(QImage(":/Indicator"));
    intruder_image = QPixmap::fromImage(QImage(":/PT"));
    plane_image =  QPixmap::fromImage(QImage(":/plane"));

    intruder_scale=0.2;
    plane_scale = 0.15;
    indicator_scale=(this->height+50)/indicator_image.height();

}

void SceneItems::advance(int phase){

    //Main loop here
    //Por unidades certas
    if(!phase) return;
    self.Z_spd += acc_z*0.01;


    self.X_pos += self.X_spd*0.01;
    self.Y_pos += self.Y_spd*0.01;
    self.Z_pos += self.Z_spd*0.01;


    if(self.Z_spd>6.0/MPS2FPM){
        self.Z_spd=6.0/MPS2FPM;
        acc_z=0;
    }else if(self.Z_spd<-6.0/MPS2FPM){
        self.Z_spd=-6.0/MPS2FPM;
        acc_z=0;
    }
    ang=self.Z_spd*qreal(M_PI)/(6.0/MPS2FPM);


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
    qreal center_y = height/2+plane_height*plane_scale*2.0;


    foreach(Message i, intruder_list){




//        qreal x = i.X_pos/NM2M;
//        qreal y = i.Y_pos/NM2M;

//        //Limit=6nm

//        painter->drawPixmap(mapToParent(center_x+x*length/MAXRANGE-intruder_image.width()*intruder_scale/2,
//                                        center_y-y*length/MAXRANGE-intruder_image.height()*intruder_scale/2).x(),
//                            mapToParent(center_x+x*length/MAXRANGE-intruder_image.width()*intruder_scale/2,
//                                        center_y-length/MAXRANGE-intruder_image.height()*intruder_scale/2).y(),
//                            intruder_image.width()*intruder_scale,
//                            intruder_image.height()*intruder_scale,
//                            intruder_image);
    }
}

void SceneItems::setupSelf()
{
    self.X_pos = 0;
    self.Y_pos = 0;
    self.Z_pos = 0;
    self.Z_spd = 0;

    acc_z = 0.0;
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
    acc_z += ACC_INCR;
}

void SceneItems::goDown()
{
    acc_z -= ACC_INCR;
}

bool SceneItems::RA_sense(Message* i, qreal v, qreal a, qreal t)
{
    qreal h_up = ownAltAt(v,a,t,1);
    qreal h_down = ownAltAt(v,a,t,-1);
    qreal h_i = i->Z_pos + t * i->Z_spd;
    qreal u = h_up-h_i;
    qreal d = h_i-h_down;

    if(self.Z_pos-i->Z_pos>0 && u >= alim*FT2M){
        return true;
    }else if(self.Z_pos-i->Z_pos<0 && d >= alim*FT2M){
        return false;
    }else if(u >= d){
        return true;
    }

    return false;
}

qreal SceneItems::stopAccel(qreal v, qreal a, qreal t, int sense)
{
    if(t<=0 || sense*self.Z_spd > v)
        return 0;
    else
        return (sense*v-self.Z_pos)/(sense*a);
}

qreal SceneItems::ownAltAt(qreal v, qreal a, qreal t, int sense)
{
    qreal s = stopAccel(v,a,t,sense);
    qreal q = qMin(t,s);
    qreal l = qMax(0.0,(t-s));


    return sense*q*q*a/2+q*self.Z_spd+self.Z_pos+sense*l*v;
}

bool SceneItems::correctiveRA(Message *intruder, bool sense)
{
    QPointF pos_rel(self.X_pos-intruder->X_pos,self.Y_pos-intruder->Y_pos);
    QPointF vel_rel(self.X_spd-intruder->X_spd,self.Y_spd-intruder->Y_spd);
    qreal h_rel = self.Z_pos - intruder->Z_pos;
    qreal vz_rel = self.Z_spd - intruder->Z_spd;
    int sign;
    if(sense)
        sign=1;
    else
        sign=-1;

    return (sqrt(QPointF::dotProduct(pos_rel,pos_rel)) < dmod_RA) || (QPointF::dotProduct(vel_rel,pos_rel)<0
                                                                      && sign*(h_rel+taumod_RA*vz_rel)<alim);
}

void SceneItems::compute_TA_RA(Message intruder)
{
    //Self e intruder em SI
    QPointF pos_rel(self.X_pos-intruder.X_pos,self.Y_pos-intruder.Y_pos);
    QPointF vel_rel(self.X_spd-intruder.X_spd,self.Y_spd-intruder.Y_spd);

    qreal time2cpa = -(QPointF::dotProduct(pos_rel,vel_rel) / QPointF::dotProduct(vel_rel,vel_rel));
    qreal time2coa = -(self.Z_pos-intruder.Z_pos)/(self.Z_spd-intruder.Z_spd);



    //
    if(self.Z_pos*FT2M<=1000){
        sl=2;
        tau_TA = 20;//s
        tau_RA = 0;
        dmod_TA = 0.3;//nm
        dmod_RA = 0;
        zthr_TA = 850;//ft
        zthr_RA = 0;
        alim= 0;

    }else if(self.Z_pos*FT2M<=2350){
        sl=3;
        tau_TA = 25;//s
        tau_RA = 15;
        dmod_TA = 0.33;//nm
        dmod_RA = 0.20;
        zthr_TA = 850;//ft
        zthr_RA = 600;
        alim= 300;
    }else if(self.Z_pos*FT2M<=5000){
        sl=4;
        tau_TA = 30;//s
        tau_RA = 20;
        dmod_TA = 0.48;//nm
        dmod_RA = 0.35;
        zthr_TA = 850;//ft
        zthr_RA = 600;
        alim= 300;
    }else if(self.Z_pos*FT2M<=10000){
        sl=5;
        tau_TA = 40;//s
        tau_RA = 25;
        dmod_TA = 0.75;//nm
        dmod_RA = 0.55;
        zthr_TA = 850;//ft
        zthr_RA = 600;
        alim= 350;
    }else if(self.Z_pos*FT2M<=20000){
        sl=6;
        tau_TA = 45;//s
        tau_RA = 30;
        dmod_TA = 1.0;//nm
        dmod_RA = 0.8;
        zthr_TA = 850;//ft
        zthr_RA = 600;
        alim= 400;
    }else if(self.Z_pos*FT2M<=42000){
        sl=7;
        tau_TA = 48;//s
        tau_RA = 35;
        dmod_TA = 1.3;//nm
        dmod_RA = 1.10;
        zthr_TA = 850;//ft
        zthr_RA = 700;
        alim= 600;
    }else{
        sl=7;
        tau_TA = 48;//s
        tau_RA = 35;
        dmod_TA = 1.3;//nm
        dmod_RA = 1.10;
        zthr_TA = 1200;//ft
        zthr_RA = 800;
        alim= 700;
    }

    taumod_TA = (pow(dmod_TA*NM2M,2) - QPointF::dotProduct(pos_rel,pos_rel))/ QPointF::dotProduct(pos_rel,vel_rel);
    taumod_RA = (pow(dmod_RA*NM2M,2) - QPointF::dotProduct(pos_rel,pos_rel))/ QPointF::dotProduct(pos_rel,vel_rel);

    bool horizontal_RA = (QPointF::dotProduct(pos_rel,pos_rel) <= dmod_RA) || (QPointF::dotProduct(pos_rel,vel_rel)<0
                                                                              && taumod_RA < tau_RA);

    bool vertical_RA = (qFabs(self.Z_pos-intruder.Z_pos) <= zthr_RA) || ((self.Z_pos-intruder.Z_pos) * (self.Z_spd-intruder.Z_spd) <0
                                                                              && time2coa < tau_RA);
    if(horizontal_RA && vertical_RA){
        bool sense = RA_sense(&intruder, 1500*FT2M/60.0, 0.25*G,taumod_RA);
        bool isCorrective = correctiveRA(&intruder,sense);
    }else if(){

    }
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

    int plane_width = plane_image.width();
    int plane_height = plane_image.height();
    int indicator_width = indicator_image.width();
    int indicator_height = indicator_image.height();

    const QLineF* pointer = new QLineF(x1,y1,x2,y2);

    //Paint Items here
    painter->drawPixmap(width/2-indicator_width*(indicator_scale/2),
                        height/2-indicator_height*(indicator_scale/2),
                        indicator_width*indicator_scale,indicator_height*indicator_scale,
                        indicator_image);
    painter->drawLine(*pointer);
    painter->drawPixmap(width/2-plane_width*(plane_scale/2),height/2+plane_height*plane_scale*2.0,
                        plane_width*plane_scale,plane_height*plane_scale,plane_image);

    // Loop to draw all intruders here
    drawIntruders(painter);
}
