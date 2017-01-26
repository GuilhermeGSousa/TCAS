#include "conversions.h"

LatLon xyz2llh(float x, float y, float z){
    LatLon ll;
    int a = 6378137;
    float f = 1/298.257223563;                        //WGS84
    float b = a*(1-f);

    float r   = sqrt(pow(x,2) + pow(y,2));
    float elinha=sqrt(pow(a,2)/pow(b,2)-1);
    float e = sqrt(1-pow(b,2)/pow(a,2));

    float F   = 54*pow(b,2)*pow(z,2);
    float g   = pow(r,2)+(1-pow(e,2))*pow(z,2)-pow(e,2)*(pow(a,2)-pow(b,2));
    float c   = pow(e,4)*F*pow(r,2)/pow(g,3);
    float s   = pow((1+c+sqrt(pow(c,2)+2*c)),(0.333334));
    float P   = F/(3*pow((s+(1/s)+1),2)*pow(g,2));
    float Q   = sqrt(1+2*pow(e,4)*P);
    float r0  = -P*pow(e,2)*r/(1+Q) + sqrt(pow(a,2)/2*(1+1/Q)-P*(1-pow(e,2))*pow(z,2)/(Q*(1+Q))-P*pow(r,2)/2);
    float  V   = sqrt(pow((r-pow(e,2)*r0),2)+(1-pow(e,2))*pow(z,2));
    float U   = sqrt(pow((r-pow(e,2)*r0),2)+pow(z,2));

    float z0  = pow(b,2)*z/(a*V);

    ll.lat = atan((z+pow(elinha,2)*z0)/r);

    if (y>=0) {
        ll.lon=(M_PI/2-2*atan(x/(r+y)));
    }else{
        ll.lon=(-M_PI/2+2*atan(x/(r-y)));
    }

    return ll;

}

NEU xyz2neu(float x, float y, float z,float x_intruso, float y_intruso, float z_intruso, float lat, float lon){
    NEU neu;
    float R[3][3]={
            {-sin(lat)*cos(lon),-sin(lon),-cos(lat)*cos(lon)},
            {-sin(lat)*sin(lon),cos(lon),-cos(lat)*sin(lon)},
            {cos(lat),0,-sin(lat)}
    };

    float RR[3][3]={
            {-sin(lat)*cos(lon),-sin(lat)*sin(lon),cos(lat)},
            {-sin(lon),cos(lon),0},
            {-cos(lat)*cos(lon),-cos(lat)*sin(lon),-sin(lat)}

    };

    float POS[3][1]={
        {x_intruso - x},
        {y_intruso - y},
        {z_intruso - z}
    };

    float Sum[3][1];
    int sum=0;

    for(int i=0;i<3;i++){
        for(int k=0;k<3;k++){
            sum+= RR[i][k] * POS[k][0];
        }
        Sum[i][0]=sum;
        sum=0;
    }

    neu.N=Sum[0][0];
    neu.E=Sum[1][0];
    neu.U=-Sum[2][0];
    return neu;
}

NEU xyz2neu(float x, float y, float z, float x_intruso, float y_intruso, float z_intruso)
{
    LatLon ll = xyz2llh(x,y,z);

    return xyz2neu(x,y,z,x_intruso,y_intruso,z_intruso,ll.lat,ll.lon);
}

QVector3D llh2wgs(qreal lat, qreal lon, qreal alt)
{
    QVector3D ans;

    ans.setX(cos(lon) * cos(lat) * (alt + RT));
    ans.setY(sin(lon) * cos(lat) * (alt + RT));
    ans.setZ(sin(lat) * (alt + RT));

    return ans;
}


QVector3D enu2wgs(qreal e, qreal n, qreal u,qreal lat, qreal lon)
{
    QVector3D ans;

    ans.setX(e * -sin(lon) + n * -cos(lon) * sin(lat) + u * cos(lon) * cos(lat));
    ans.setY(e * cos(lon) + n * -sin(lon) * sin(lat) + u * sin(lon) * cos(lat));
    ans.setZ(n * cos(lat) + u * sin(lat));
    return ans;

}


QVector3D wgs2enu(qreal x, qreal y, qreal z){
    QVector3D ans;

    QVector3D llh = wgs2llh(x,y,z); //LaltLonAlt

    ans.setX(x * -sin(llh.y()) + y * cos(llh.y()));
    ans.setY(x * -cos(llh.y()) * sin(llh.x()) + y * -sin(llh.y()) * sin(llh.x()) + z * cos(llh.x()));
    ans.setZ(x * cos(llh.y()) * cos(llh.x()) + y * sin(llh.y()) * cos(llh.x()) + z * sin(llh.x()));

    return ans;
}

QVector3D wgs2enu(qreal x, qreal y, qreal z, qreal lat, qreal lon){
    QVector3D ans;


    ans.setX(x * -sin(lon) + y * cos(lon));
    ans.setY(x * -cos(lon) * sin(lat) + y * -sin(lon) * sin(lat) + z * cos(lat));
    ans.setZ(x * cos(lon) * cos(lat) + y * sin(lon) * cos(lat) + z * sin(lat));

    return ans;
}

QVector3D wgs2llh(qreal x, qreal y, qreal z)
{
    QVector3D ans;


    ans.setY(atan2(y,x));
    ans.setZ(sqrt(x*x + y*y + z*z) - RT);
    ans.setX(asin(z/(ans.z() + RT)));

    return ans;
}
