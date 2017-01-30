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
    PT_image = QPixmap::fromImage(QImage(":/PT"));
    TA_image = QPixmap::fromImage(QImage(":/TA"));
    RA_image = QPixmap::fromImage(QImage(":/RA"));
    OT_image = QPixmap::fromImage(QImage(":/OT"));
    blue_arrow_down = QPixmap::fromImage(QImage(":/Images/blue_arrow_down.png"));
    blue_arrow_up = QPixmap::fromImage(QImage(":/Images/blue_arrow_up.png"));
    red_arrow_down = QPixmap::fromImage(QImage(":/Images/red_arrow_down.png"));
    red_arrow_up = QPixmap::fromImage(QImage(":/Images/red_arrow_up.png"));
    yellow_arrow_down = QPixmap::fromImage(QImage(":/Images/yellow_arrow_down.png"));
    yellow_arrow_up = QPixmap::fromImage(QImage(":/Images/yellow_arrow_up.png"));

    plane_image =  QPixmap::fromImage(QImage(":/plane"));

    intruder_scale=0.2;
    plane_scale = 0.15;
    indicator_scale=(this->height+150)/indicator_image.height();

}

void SceneItems::advance(int phase){

    //Main loop here
    //Por unidades certas
    if(!phase) return;
    v_U += acc_z*0.01;

    llh_pos = wgs2llh(self.X_pos, self.Y_pos, self.Z_pos);
    QVector3D wgs_spd = enu2wgs(v_E, v_N, v_U, llh_pos.x(), llh_pos.y());

    self.X_pos += wgs_spd.x()*0.01;
    self.Y_pos += wgs_spd.y()*0.01;
    self.Z_pos += wgs_spd.z()*0.01;


    if(v_U > MAXVSPD/MPS2FPM){
        v_U= MAXVSPD/MPS2FPM;
        acc_z=0;
    }else if(v_U < -MAXVSPD/MPS2FPM){
        v_U =-MAXVSPD/MPS2FPM;
        acc_z=0;
    }
    ang=v_U*qreal(M_PI)/(MAXVSPD/MPS2FPM);


    if(ang>qreal(M_PI))
        ang=qreal(M_PI);
    if(ang<-qreal(M_PI))
        ang=-qreal(M_PI);

    rotatePointer(ang);



}

void SceneItems::addIntruder(Message m)
{
    //Add new intruder to list or update intruder messages

    if(m.Ac_id != 0xF34C290F)
    {
        for(int i = 0; i < intruder_list.length(); i++){
            if(m.Ac_id == intruder_list[i].Ac_id){
                intruder_list.replace(i,m);
                return;
            }
        }
        intruder_list.append(m);
    }
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

    int plane_height = plane_image.height();
    qreal center_x = width/2;
    qreal center_y = height-plane_height*plane_scale*2.5;


    foreach(Message i, intruder_list){

        QVector3D me(self.X_pos,
                     self.Y_pos,
                     self.Z_pos);
        QVector3D intr(i.X_pos,
                       i.Y_pos,
                       i.Z_pos);


        me = wgs2enu(me.x(),me.y(),me.z(),llh_pos.x(),llh_pos.y());
        intr = wgs2enu(intr.x(),intr.y(),intr.z(),llh_pos.x(),llh_pos.y());
        QVector3D intr_spd = wgs2enu(i.X_spd,i.Y_spd,i.Z_spd,llh_pos.x(),llh_pos.y());

        QVector3D intr_rel=intr-me;
        qreal h_rel = intr_rel.z();
        qreal dist = intr_rel.distanceToPoint(QVector3D(0,0,0));
        //Change to our frame of ref
        //Rever matrizes de rotação!
        intr_rel.setX(intr_rel.x()*cos(bearing*qreal(M_PI)/180.0)-intr_rel.y()*sin(bearing*qreal(M_PI)/180.0));
        intr_rel.setY(intr_rel.x()*sin(bearing*qreal(M_PI)/180.0)+intr_rel.y()*cos(bearing*qreal(M_PI)/180.0));


        qreal x = intr_rel.x()/NM2M;
        qreal y = intr_rel.y()/NM2M;

        QPen* penHText;

        Advisory threat_level = issue_TA_RA(&i);
        advanceStatus(&i,threat_level);
        //qDebug()<<self.TCAS_status;
        if (threat_level==RA){
            //qDebug()<<self.Resolution;
            //qDebug()<<self.Resolution_val/FT2M*60;
        }
        switch (threat_level) {
        case TA:
            painter->drawPixmap(center_x+x*length/MAXRANGE-PT_image.width()*intruder_scale/2,
                                center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                PT_image.width()*intruder_scale,
                                PT_image.height()*intruder_scale,
                                TA_image);
            if (intr_spd.z()<0){
                painter->drawPixmap(center_x+x*length/MAXRANGE+PT_image.width()*intruder_scale/2,
                                    center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                    yellow_arrow_down.width()*intruder_scale*0.8,
                                    PT_image.height()*intruder_scale,
                                    yellow_arrow_down);
            }else if (intr_spd.z()>0){
                painter->drawPixmap(center_x+x*length/MAXRANGE+PT_image.width()*intruder_scale/2,
                                    center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                    yellow_arrow_up.width()*intruder_scale*0.8,
                                    PT_image.height()*intruder_scale,
                                    yellow_arrow_up);
            }
            penHText = new QPen(QColor(255,249,0,255));
            break;
        case RA:
            painter->drawPixmap(center_x+x*length/MAXRANGE-PT_image.width()*intruder_scale/2,
                                center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                PT_image.width()*intruder_scale,
                                PT_image.height()*intruder_scale,
                                RA_image);
            if (intr_spd.z()<0){
                painter->drawPixmap(center_x+x*length/MAXRANGE+PT_image.width()*intruder_scale/2,
                                    center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                    red_arrow_down.width()*intruder_scale*0.8,
                                    PT_image.height()*intruder_scale,
                                    red_arrow_down);
            }else if (intr_spd.z()>0){
                painter->drawPixmap(center_x+x*length/MAXRANGE+PT_image.width()*intruder_scale/2,
                                    center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                    red_arrow_up.width()*intruder_scale*0.8,
                                    PT_image.height()*intruder_scale,
                                    red_arrow_up);
            }
            penHText = new QPen(QColor(255,0,0,255));
            break;
        case PT:
            painter->drawPixmap(center_x+x*length/MAXRANGE-PT_image.width()*intruder_scale/2,
                                center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                PT_image.width()*intruder_scale,
                                PT_image.height()*intruder_scale,
                                PT_image);
            if (intr_spd.z()<0){
                painter->drawPixmap(center_x+x*length/MAXRANGE+PT_image.width()*intruder_scale/2,
                                    center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                    blue_arrow_down.width()*intruder_scale*0.8,
                                    PT_image.height()*intruder_scale,
                                    blue_arrow_down);
            }else if (intr_spd.z()>0){
                painter->drawPixmap(center_x+x*length/MAXRANGE+PT_image.width()*intruder_scale/2,
                                    center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                    blue_arrow_up.width()*intruder_scale*0.8,
                                    PT_image.height()*intruder_scale,
                                    blue_arrow_up);
            }
            penHText = new QPen(QColor(0,147,255,255));
            break;
        case OT:
            painter->drawPixmap(center_x+x*length/MAXRANGE-PT_image.width()*intruder_scale/2,
                                center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                PT_image.width()*intruder_scale,
                                PT_image.height()*intruder_scale,
                                OT_image);
            if (intr_spd.z()<0){
                painter->drawPixmap(center_x+x*length/MAXRANGE+PT_image.width()*intruder_scale/2,
                                    center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                    blue_arrow_down.width()*intruder_scale*0.8,
                                    PT_image.height()*intruder_scale,
                                    blue_arrow_down);
            }else if (intr_spd.z()>0){
                painter->drawPixmap(center_x+x*length/MAXRANGE+PT_image.width()*intruder_scale/2,
                                    center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale/2,
                                    blue_arrow_up.width()*intruder_scale*0.8,
                                    PT_image.height()*intruder_scale,
                                    blue_arrow_up);
            }
            penHText = new QPen(QColor(0,147,255,255));
            break;
        }
        painter->setPen(*penHText);
        char text[2];
        if (h_rel<0){
            sprintf(text, "-%02d", int(-h_rel/FT2M/100));
            painter->drawText(QRect(center_x+x*length/MAXRANGE-PT_image.width()*intruder_scale*4/5,
                                    center_y-y*length/MAXRANGE+PT_image.height()*intruder_scale/2,
                                    PT_image.width()*intruder_scale*3/2,
                                    15),
                                    Qt::AlignCenter, text);
        }else{
            sprintf(text, "+%02d", int(h_rel/FT2M/100));
            painter->drawText(QRect(center_x+x*length/MAXRANGE-PT_image.width()*intruder_scale*4/5,
                                    center_y-y*length/MAXRANGE-PT_image.height()*intruder_scale*5/4,
                                    PT_image.width()*intruder_scale*3/2,
                                    15),
                                    Qt::AlignCenter, text);
        }
    }
}

qreal SceneItems::converter(qreal v, qreal v0, qreal a0, qreal v1, qreal a1){
    qreal a;
    a=(v-v0)*a1/(v1-v0)-((v-v1)*a0/(v1-v0));
    return a;
}

void SceneItems::drawTarget(QPainter *painter, qreal v_min)
{
    QPen pen(Qt::red);
    pen.setWidth(25);
    painter->setPen(pen);

    QPen pen2(Qt::green);
    pen2.setWidth(25);


    const int startAngle = 0;
    int spanAngle;

    qreal a;


    if(v_min>0 && v_min<=1){
        a=converter(v_min,0,0,1,63);

        painter->drawArc(width/2-280,height/2-260,570,510,startAngle,-(180+a)*16);

        painter->setPen(pen2);

        //painter->drawArc(width/2-280,height/2-260,570,510,-50*16,--60*16);

    }else if (v_min<0 && v_min>=-1){
        a=converter(-v_min,0,0,1,63);

        painter->drawArc(width/2-280,height/2-260,570,510,startAngle,(180+a)*16);

    }else if(v_min>1 && v_min<=1.5){
        a=converter(v_min,1,63,1.5,88);

        painter->drawArc(width/2-280,height/2-260,570,510,startAngle,-(180+a)*16);

    }else if(v_min<-1 && v_min>=-1.5){
        a=converter(-v_min,1,63,1.5,88);

        painter->drawArc(width/2-280,height/2-260,570,510,startAngle,(180+a)*16);

    }else if(v_min>1.5 && v_min<=2){

        a=converter(v_min,1.5,88,2,103);
        //qDebug()<<a;
        painter->drawArc(width/2-280,height/2-260,570,510,startAngle,-(180+a)*16);

    }else if(v_min<-1.5 && v_min>=-2){

        a=converter(-v_min,1.5,88,2,103);
        //qDebug()<<a;
        painter->drawArc(width/2-280,height/2-260,570,510,startAngle,(180+a)*16);

    }
    else if(v_min>2 && v_min<=3){
        a=converter(v_min,2,103,3,128);
        painter->drawArc(width/2-280,height/2-260,570,510,startAngle,-(180+a)*16);

    }else if(v_min<-2 && v_min>=-3){
        a=converter(-v_min,2,103,3,128);
        painter->drawArc(width/2-280,height/2-260,570,510,startAngle,(180+a)*16);

    }
    else if(v_min>3 && v_min<=4){
    a=converter(v_min,3,128,4,145);
    painter->drawArc(width/2-280,height/2-260,570,510,startAngle,-(180+a)*16);

    }else if(v_min<-3 && v_min>=-4){
        a=converter(-v_min,3,128,4,145);
        painter->drawArc(width/2-280,height/2-260,570,510,startAngle,(180+a)*16);

        }
    else if(v_min>4 && v_min<=5){
        a=converter(v_min,4,145,5,159);
        painter->drawArc(width/2-390,height/2-350,780,700,startAngle,-(180+a)*16);

    }else if(v_min<-4 && v_min>=-5){
        a=converter(-v_min,4,145,5,159);
        painter->drawArc(width/2-390,height/2-350,780,700,startAngle,(180+a)*16);

    }
    else if(v_min>5 && v_min<=6){
        a=converter(v_min,5,159,6,171);
        painter->drawArc(width/2-280,height/2-260,570,510,startAngle,-(180+a)*16);
    }







    //painter->drawPixmap(width/2-umemeio_width+20,height-umemeio_height-285,
    //                umemeio_width*target_scale*4.7,umemeio_height*target_scale*4.6,umemeio_image);
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

void SceneItems::goLeft()
{
    bearing -= DEGINC;
    //Update vel
    v_N = vmax * cos(bearing*qreal(M_PI)/180.0);
    v_E = vmax * sin(bearing*qreal(M_PI)/180.0);
}

void SceneItems::goRight()
{
    bearing += DEGINC;
    //Update vel
    v_N = vmax * cos(bearing*qreal(M_PI)/180.0);
    v_E = vmax * sin(bearing*qreal(M_PI)/180.0);
}

void SceneItems::setStart(qreal X, qreal Y, qreal Z, qreal V)
{
//Iniciar self aqui
    bearing = 0;
    QVector3D wgs_pos = llh2wgs(X * qreal(M_PI/180.0),Y * qreal(M_PI/180.0),Z);
    qDebug()<<wgs_pos.x();
    qDebug()<<wgs_pos.y();
    qDebug()<<wgs_pos.z();

    v_E=0;
    vmax=V;
    v_N=V;
    v_U=0;
    QVector3D wgs_spd = enu2wgs(v_E,v_N,v_U,X * qreal(M_PI/180.0),Y * qreal(M_PI/180.0));


    self.X_pos = wgs_pos.x();
    self.Y_pos = wgs_pos.y();
    self.Z_pos = wgs_pos.z();


    //Speed here
    self.X_spd = wgs_spd.x();
    self.Y_spd = wgs_spd.y();
    self.Z_spd = wgs_spd.z();

    self.Ac_id = 0xF34C290F;

    strncpy(self.header,"ACIP TCAS   V01\0", 16);

    strncpy(self.TCAS_status,"CLEAR",16);
    strncpy(self.Resolution,"CLEAR",16);
    self.Resolution_val = 0;
    self.Intruder_hex = 0;



}

void SceneItems::setCRC(uint32_t crc)
{
    self.CRC_32 = crc;
}

//afonso stuff

int SceneItems::RA_sense(QVector3D *i, QVector3D *i_spd, qreal v, qreal a, qreal t)
{
    qreal h_up = ownAltAt(v,a,t,1);
    qreal h_down = ownAltAt(v,a,t,-1);
    qreal h_i = i->z() + t * i_spd->z();
    qreal u = h_up-h_i;
    qreal d = h_i-h_down;

    if(me.z()-i->z()>0 && u >= alim*FT2M){
        //qDebug()<<"Case 1";
        return 1;
    }else if(me.z()-i->z()<0 && d >= alim*FT2M){
        //qDebug()<<"Case 2";
        return -1;
    }else if(u >= d){
        //qDebug()<<"Case 3";
        return 1;
    }else {
        //qDebug()<<"Case 4";
        return -1;
    }
}

qreal SceneItems::stopAccel(qreal v, qreal a, qreal t, int sense)
{
    if(t<=0 || sense*v_U >= v)
        return 0;
    else
        return (sense*v-v_U)/(sense*a);
}

qreal SceneItems::ownAltAt(qreal v, qreal a, qreal t, int sense)
{
    qreal s = stopAccel(v,a,t,sense);
    qreal q = qMin(t,s);
    qreal l = qMax(0.0,(t-s));

    return sense*q*q*a/2+q*v_U+me.z()+sense*l*v;
}


void SceneItems::computeTCAStimes(QVector3D *intr, QVector3D *intr_spd, qreal *t2cpa, qreal *t2coa, QPointF* pos_rel, QPointF* vel_rel){

    *t2cpa = -(QPointF::dotProduct(*pos_rel, *vel_rel) / QPointF::dotProduct(*vel_rel, *vel_rel));
    *t2coa = -(me.z()-intr->z())/(v_U-intr_spd->z());
}

Advisory SceneItems::issue_TA_RA(Message *intruder)
{

    me.setX(self.X_pos);
    me.setY(self.Y_pos);
    me.setZ(self.Z_pos);

    QVector3D intr(intruder->X_pos,
                   intruder->Y_pos,
                   intruder->Z_pos);

    me = wgs2enu(me.x(),me.y(),me.z(),llh_pos.x(),llh_pos.y());
    intr = wgs2enu(intr.x(),intr.y(),intr.z(),llh_pos.x(),llh_pos.y());

    QVector3D intr_spd = wgs2enu(intruder->X_spd,intruder->Y_spd,intruder->Z_spd,llh_pos.x(),llh_pos.y());

    QPointF pos_rel(me.x()-intr.x(),me.y()-intr.y());
    QPointF vel_rel(v_E-intr_spd.x(),v_N-intr_spd.y());


    //Self e intruder em SI
    qreal time2cpa, time2coa;
    computeTCAStimes(&intr, &intr_spd, &time2cpa, &time2coa, &pos_rel, &vel_rel);

    //
    if(me.z()/FT2M<=1000){
        sl=2;
        tau_TA = 20;//s
        tau_RA = 0;
        dmod_TA = 0.3;//nm
        dmod_RA = 0;
        zthr_TA = 850;//ft
        zthr_RA = 0;
        alim= 0;

    }else if(me.z()/FT2M<=2350){
        sl=3;
        tau_TA = 25;//s
        tau_RA = 15;
        dmod_TA = 0.33;//nm
        dmod_RA = 0.20;
        zthr_TA = 850;//ft
        zthr_RA = 600;
        alim= 300;
    }else if(me.z()/FT2M<=5000){
        sl=4;
        tau_TA = 30;//s
        tau_RA = 20;
        dmod_TA = 0.48;//nm
        dmod_RA = 0.35;
        zthr_TA = 850;//ft
        zthr_RA = 600;
        alim= 300;
    }else if(me.z()/FT2M<=10000){
        sl=5;
        tau_TA = 40;//s
        tau_RA = 25;
        dmod_TA = 0.75;//nm
        dmod_RA = 0.55;
        zthr_TA = 850;//ft
        zthr_RA = 600;
        alim= 350;
    }else if(me.z()/FT2M<=20000){
        sl=6;
        tau_TA = 45;//s
        tau_RA = 30;
        dmod_TA = 1.0;//nm
        dmod_RA = 0.8;
        zthr_TA = 850;//ft
        zthr_RA = 600;
        alim= 400;
    }else if(me.z()/FT2M<=42000){
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

    bool horizontal_RA = (sqrt(QPointF::dotProduct(pos_rel,pos_rel)) <= dmod_RA*NM2M) || (QPointF::dotProduct(pos_rel,vel_rel)<0
                                                                                          && taumod_RA < tau_RA);

    bool vertical_RA = (qFabs(me.z()-intr.z()) <= zthr_RA*FT2M) || ((me.z()-intr.z()) * (v_U-intr_spd.z()) <0
                                                                               && time2coa < tau_RA);

    bool horizontal_TA = (sqrt(QPointF::dotProduct(pos_rel,pos_rel)) <= dmod_TA*NM2M) || (QPointF::dotProduct(pos_rel,vel_rel)<0
                                                                                          && taumod_TA < tau_TA);

    bool vertical_TA = (qFabs(me.z()-intr.z()) <= zthr_TA*FT2M) || ((me.z()-intr.z()) * (v_U-intr_spd.z()) <0
                                                                               && time2coa < tau_TA);

    if(horizontal_RA && vertical_RA) {
        return RA;
    }else if((horizontal_TA || horizontal_RA) && (vertical_TA || vertical_RA)) {
        return TA;
    }else if(sqrt(QPointF::dotProduct(pos_rel,pos_rel))<6*NM2M && qFabs(me.z()-intr.z())<1200*FT2M) {
        return PT;
    }else{
        return OT;
    }
}

void SceneItems::complementResolutions(Message *intruder){
    if (!strcmp(intruder->Resolution,"CLIMB")){
        strncpy(self.Resolution,"DESCEND",16);
    }else{
        strncpy(self.Resolution,"CLIMB",16);
    }
}

bool SceneItems::areResolutionsComplementary(Message* intruder){
    if (!strcmp(intruder->Resolution,self.Resolution)){
        return false;
    }else{
        return true;
    }
}

void SceneItems::computeResolutionStrength(QVector3D *intr, QVector3D *intr_spd){
    // We know the resolution sense by now
    // Assume intruder keeps his current path and loop to check
    // which vertical speed will allow us to obtain the separation ALIM at CPA
    int sense;
    int inc = 10;
    qreal target_diff=alim*FT2M;
    double target_v;
    if (!strcmp(self.Resolution,"CLIMB")){sense=1;
    }else{sense=-1;}
    if (sense*v_U>0){target_v=v_U-inc*sense;
    }else{target_v=-inc*sense;}
    qreal h_at_cpa, h_diff;
    do{
        target_v += inc*sense;
        h_at_cpa = ownAltAt(target_v,0.35*G,taumod_RA,sense);
        h_diff = h_at_cpa - (intr->z() + taumod_RA * intr_spd->z());
    }while(qFabs(h_diff)<target_diff && qFabs(target_v)<6000*FT2M/60);
    if (qFabs(target_v)>6000*FT2M/60){target_v=sense*6000*FT2M/60;}
    self.Resolution_val = target_v;
}


void SceneItems::advanceStatus(Message *intruder, Advisory result){


    QVector3D intr(intruder->X_pos,
                   intruder->Y_pos,
                   intruder->Z_pos);

    intr = wgs2enu(intr.x(),intr.y(),intr.z(),llh_pos.x(),llh_pos.y());

    QVector3D intr_spd = wgs2enu(intruder->X_spd,intruder->Y_spd,intruder->Z_spd,llh_pos.x(),llh_pos.y());

    //result=TA/RA/NT/PT
    switch(result){
        case TA:
            self.Intruder_hex = intruder->Ac_id;
            if (!strcmp(self.TCAS_status,"RESOLVING") || !strcmp(self.TCAS_status,"RETURNING")){
                strncpy(self.TCAS_status,"RETURNING",16);
            }else {
                strncpy(self.TCAS_status,"ADVISORY",16);
            }break;
        case OT:
        case PT:
            strncpy(self.TCAS_status,"CLEAR",16);
            break;
        case RA:
            self.Intruder_hex = intruder->Ac_id;
            if (!strcmp(self.TCAS_status,"CLEAR") || !strcmp(self.TCAS_status,"ADVISORY")){
                if (!strcmp(intruder->TCAS_status,"RESOLVING")){
                    complementResolutions(intruder);
                    computeResolutionStrength(&intr, &intr_spd);
                }else{
                    int sense = RA_sense(&intr, &intr_spd, 1500 * FT2M / 60.0, 0.35 * G, taumod_RA);
                    if (sense==1){
                        strncpy(self.Resolution,"CLIMB",16);
                    }else{
                        strncpy(self.Resolution,"DESCEND",16);}
                    computeResolutionStrength(&intr, &intr_spd);
                }
                strncpy(self.TCAS_status,"RESOLVING",16);
            }else if(!strcmp(self.TCAS_status,"RESOLVING")){
                if(!strcmp(intruder->TCAS_status,"RESOLVING")){
                    bool diff_resolutions = areResolutionsComplementary(intruder);
                    if (!diff_resolutions && self.Ac_id < intruder->Ac_id){
                        complementResolutions(intruder);}
                }
                computeResolutionStrength(&intr, &intr_spd);
            }

    }
    //if result is TA, several cases
    //if RESOLVING or RETURNING, status is set to RETURNING
    //else, set to ADVISORY
    //if result is NT or PT, status is set to CLEAR
    //if result is RA, we have several cases
    //if our status is CLEAR or ADVISORY, several cases
    //check if intruder is RESOLVING.
    //if he is, complement and set RESOLVING
    // if he is not, compute and set RESOLVING
    //if our status is RESOLVING and intruder also
    //check if actions are complementary
    //if they are not, check if we have lower id
    //if we do, complement the action
}

// no longer afonso stuff


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
    painter->drawPixmap(width/2-plane_width*(plane_scale/2),height-plane_height*plane_scale*2.5,
                        plane_width*plane_scale,plane_height*plane_scale,plane_image);


    // Loop to draw all intruders here
    drawTarget(painter,5);
    drawIntruders(painter);
}
