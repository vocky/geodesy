/*
 * distance.h
 *
 *  Created on: July 19, 2014
 *      Author: laye
 */

#ifndef DISTANCE_H_
#define DISTANCE_H_

#include "define.h"
#include <stdlib.h>
#include <math.h>

#define EARTH_RADIUS            6378.137        //earch radius

#ifndef DSquare
#define DSquare(x) ((x)*(x))
#endif
#define NM_2_KM(value) ((value)*1852.0/1000.0)

// get the radian
double radian_1(double d)
{
    return d * PI / 180.0;
}

void SubGussFs(double X,double Y,double L0, double* B, double* L)
{
    double p=57.29577951472;
    const double a=6.378245000e+06;
    const double e2=0.00669342162297;
    const double e12=0.00673852541468;
    const double c0=0.157046064172e-06;
    const double c1=0.005051773759;
    const double c2=0.000029837302;
    const double c3=0.000000238189;

    double bf0 = c0*X;
    double bf0c = cos(bf0);
    double bf0s = sin(bf0);
    double bf = bf0 + bf0c*(c1*bf0s - c2*pow(bf0s,3) + c3* pow(bf0s,5));
    double bt = tan(bf);
    double bc = cos(bf);
    double bs = sin(bf);
    double bi = e12 * pow(bc,2);
    double v2 = 1.0e+0 +bi;
    double bn = a/sqrt(1.0-e2* pow(bs,2));
    double yn=Y/bn;

    double b1 = -v2*bt*pow(yn,2)/2.0;
    double b2 = -(5.0+3.0*pow(bt,2) + bi-9.0*bi* pow(bt,2)) *b1* pow(yn,2) /12.0;
    double b3 = (61.0+90.0* pow(bt,2) + 45.0* pow(bt,4))*b1* pow(yn,4)/360.0;
    *B = bf + b1 + b2 + b3;
    *B = *B * p;

    double l1=yn/bc;
    double l2=-(1.0+2.0* pow(bt,2)+bi)*l1*pow(yn,2)/6.0;
    double l3=(5.0+28.0*pow(bt,2)+24.0*pow(bt,4)+6.0*bi+8.0*bi*pow(bt,2))*l1*pow(yn,4)/120.0;
    *L = l1 + l2 + l3;
    *L = *L * p;

    *L += L0;
    if(*L > 360.0)
        *L -= 360.0;
}

void CalGuassToLB(double dX, double dY, double* dLongitude, double* dLatitude)
{
    double L0;
    int nZoonNum;

    nZoonNum = (int)(dY/(1.0E+6));
    L0 = nZoonNum * 6-3;

    dY = dY - nZoonNum*1.0E+6;
    SubGussFs(dX, dY-500000,L0, dLatitude, dLongitude);
    *dLongitude = *dLongitude + nZoonNum * 6 - 3 ;
}

void SubGussFs(double* X, double* Y,double B,double L,int nCenterLongi)
{
    int nzonenum;
    if(nCenterLongi==0)
    {
        nzonenum = (int)L/6+1;
        nCenterLongi = nzonenum*6-3;
    }
    else
        nzonenum = (int)nCenterLongi/6+1;

    double rB = B/180*PI;
    double rL = (L-nCenterLongi)/180*PI;

    const double a = 6378245.00;
    const double b = 6356863.50;

    double sqre1 = (a*a-b*b)/(a*a);

    double sinb = sin(rB);
    double cosb = cos(rB);
    double M = a*(1-sqre1)/(1-sqre1*sinb*sinb)/sqrt(1-sqre1*sinb*sinb);

    double N = a/sqrt(1-sqre1*sinb*sinb);
    double sqrita = N/M-1;

    double s = a * ( 1 - sqre1 ) * ( 1.00505117739 * rB - 0.00506237764/2 * sin(2*rB) + 0.0000106245/4 * sin(4*rB) - 0.00000002081/6 * sin(6*rB));

    double tanb = tan(rB);
    *X = s+rL*rL*N/2*sinb*cosb + rL*rL*rL*rL*N/24*sinb*cosb*cosb*cosb * (5-tanb*tanb + 9*sqrita*sqrita +4*sqrita);
    *Y = rL*N*cosb+rL*rL*rL*N/6*cosb*cosb*cosb*(1-tanb*tanb+sqrita) +
            rL*rL*rL*rL*rL*N/120*cosb*cosb*cosb*cosb*cosb*(5-18*tanb*tanb+tanb*tanb*tanb*tanb);
    *Y = *Y + 500000 + nzonenum * 1.0e+6;
}

void CalGuassFromLB(double dLongitude, double dLatitude, double* dX, double* dY, long nCenterL)
{
    int CenterL = (int)nCenterL;

    SubGussFs(dX, dY, dLatitude, dLongitude, CenterL);
    nCenterL = (long)CenterL;
}

void GreatCircle(double PointA_Latitude, double PointA_Longitude,
                 double PointB_Latitude, double PointB_Longitude,
                 double *GreatCircleDistance, double *GreatCircleBearing)
{
    double Cos_PointA_Latitude;             /* Cosine of point A latitude. */
    double Cos_PointB_Latitude;             /* Cosine of point B latitude. */
    double Sin_PointA_Latitude;               /* Sine of point A latitude. */
    double Sin_PointB_Latitude;               /* Sine of point B latitude. */
    double Delta_Longitude;        /* Longitude differance B - A, radians. */
    double Cos_Delta_Longitude;              /* Cosine of delta longitude. */
    double Sin_Delta_Longitude;                /* Sine of delta longitude. */
    double Sin_Distance;                 /* Sine of great circle distance. */
    double Cos_Distance;               /* Cosine of great circle distance. */
    double Sin_Bearing;                   /* Sine of great circle bearing. */
    double Cos_Bearing;                 /* Cosine of great circle bearing. */
    double intermeadiate;                 /* An intermeadiate calculation. */

    /* Calculate some sines and cosines now to save time later. */

    Cos_PointA_Latitude = cos(PointA_Latitude);
    Cos_PointB_Latitude = cos(PointB_Latitude);
    Sin_PointA_Latitude = sin(PointA_Latitude);
    Sin_PointB_Latitude = sin(PointB_Latitude);
    Delta_Longitude = PointB_Longitude - PointA_Longitude;
    Cos_Delta_Longitude = cos(Delta_Longitude);
    Sin_Delta_Longitude = sin(Delta_Longitude);

    /* Get the cosine of the great circle distance. */

    Cos_Distance = Sin_PointA_Latitude*Sin_PointB_Latitude
           + Cos_PointA_Latitude*Cos_PointB_Latitude*Cos_Delta_Longitude;

    /* Get the sine of the great circle distance. */

    intermeadiate = DSquare(Cos_PointB_Latitude*Sin_Delta_Longitude)
        + DSquare(Cos_PointA_Latitude*Sin_PointB_Latitude
        - Sin_PointA_Latitude*Cos_PointB_Latitude*Cos_Delta_Longitude);

    Sin_Distance = sqrt(fabs(intermeadiate));

    /* Get the the great circle distance. */

    if(fabs(Sin_Distance)>.0000001 || fabs(Cos_Distance)>.0000001)
        *GreatCircleDistance = atan2(Sin_Distance,Cos_Distance);
    else
        *GreatCircleDistance = 0.0;                         /* Same point. */

    /* Get the sine of the great circle bearing. */

    Sin_Bearing = Sin_Delta_Longitude*Cos_PointB_Latitude;

    /* Get the cosine of the great circle bearing. */

    Cos_Bearing = Cos_PointA_Latitude*Sin_PointB_Latitude
           - Sin_PointA_Latitude*Cos_PointB_Latitude*Cos_Delta_Longitude;

    /* Get the great circle bearing. */

    //   if(fabs(Sin_Bearing)>.0000001 || fabs(Cos_Bearing)>.0000001)
    //   {
    intermeadiate = atan2(Sin_Bearing,Cos_Bearing);
    while(intermeadiate<0.0)
        intermeadiate += 2.0*PI;
    *GreatCircleBearing = intermeadiate;
    //   }
    //   else
    //  *GreatCircleBearing = 0.0;            /* Due north or co-incident. */
}

void MyCalDistance(double a_lat,double a_log,double b_lat, double b_log, double angleDiff,
                 double *dDistance, double *dBearing)
{
    double a_dlat,a_dlog,b_dlat,b_dlog;
    double distance,bearing;
    a_dlat=a_lat/180.0*PI;
    a_dlog=a_log/180.0*PI;
    b_dlat=b_lat/180.0*PI;
    b_dlog=b_log/180.0*PI;

    GreatCircle(a_dlat,a_dlog,b_dlat,b_dlog,&distance,&bearing);
    *dDistance = NM_2_KM( distance*180.0*60.0/PI)*1000.0;
    *dBearing = bearing *180.0/PI+angleDiff;
}

/* Calculate distance use Haversine's formula.
 * */
double
CalculateLength_Haversine(double lng1, double lat1, double lng2, double lat2)
{
    double radLat1 = radian_1(lat1);
    double radLat2 = radian_1(lat2);
    double a = radLat1 - radLat2;
    double b = radian_1(lng1 - lng2);

    double dst = asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2) )));

    dst = dst * 2 * EARTH_RADIUS * 1000.0;
    return dst;
}

/* Calculate distance use Guass's formula.
 * */
double CalculateLength_Guass(double dX1,double dY1,double dX2,double dY2)
{
    double dLength = 0;
    double x1=0,x2=0,y1=0,y2=0;
    int nCenterL = ((int)dX1/6+1)*6-3;

    CalGuassFromLB(dX1, dY1, &x1, &y1, nCenterL);
    CalGuassFromLB(dX2, dY2, &x2, &y2, nCenterL);
    dLength += sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    return dLength;
}

/* Calculate distance use GreatCircle's formula.
 * */
double CalculateLength_GreatCircle(double lng1, double lat1, double lng2, double lat2) {
    double dbAzimuth = 0, dbDistance = 0, dbAngleDiff = 0;
    MyCalDistance(lat1, lng1, lat2, lng2, dbAngleDiff, &dbDistance, &dbAzimuth);
    return dbDistance;
}

#endif
