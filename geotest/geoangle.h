/*
 * geoangle.h
 *
 *  Created on: July 22, 2014
 *      Author: laye
 */

#ifndef GEOANGLE_H_
#define GEOANGLE_H_

#include "define.h"

/* Calculate the include angle between two angles,
 * which is below than 180.
 * */
int CalculateIncludeAngle(int angle1, int angle2)
{
    if(angle1 == -1 || angle2 == -1)
        return 0;

    int iTemp = abs(angle2 - angle1);
    return  iTemp <= 180 ? iTemp : 360 - iTemp;
}

/* Calculate the clockwise angle of the vector.
 * Zero degrees azimuth is set toward a heading of true north.
 * */
int CalculateAngle(double x1, double y1, double x2, double y2)
{
    double vertexX = x2 - x1;
    double vertexY = y2 - y1;

    if(fabs(vertexX) < 1e-6 && fabs(vertexY) < 1e-6)
        return -1;
    else if(fabs(vertexX) < 1e-6 && vertexY > 1e-6)
        return 0;
    else if(vertexX > 1e-6 && fabs(vertexY) < 1e-6)
        return 90;
    else if(fabs(vertexX) < 1e-6 && -1 * vertexY > 1e-6)
        return 180;
    else if(-1 * vertexX > 1e-6 && fabs(vertexY) < 1e-6)
        return 270;

    else
    {
        if(vertexX > 1e-6)
            return 90 - (int)(atan(vertexY / vertexX) * 180 / PI);
        else
            return 270 - (int)(atan(vertexY / vertexX) * 180 / PI);
    }
}

/* Calculate the perpendicular between the gps point and the the vertex.
 * */
bool CalculatePerpendicular(LonLat Vtx1, LonLat Vtx2, LonLat GPS, LonLat &MatchNodeOut)
{
    // Convert longitude and latitude to Long value.
    const long PARAM = 10000000000;

    double lVtx1Longitude = (Vtx1.lon * PARAM);
    double lVtx1Latitude = (Vtx1.lat * PARAM);

    double lVtx2Longitude = (Vtx2.lon * PARAM);
    double lVtx2Latitude = (Vtx2.lat * PARAM);

    double lGpsLongitude = (GPS.lon * PARAM);
    double lGpsLatitude = (GPS.lat * PARAM);

    // The line function involved pVtx1 and pVtx2 is: a * x + b * y + c = 0.
    double a = lVtx2Latitude - lVtx1Latitude;
    double b = lVtx1Longitude - lVtx2Longitude;
    double cc = Vtx2.lon * Vtx1.lat -
            Vtx1.lon * Vtx2.lat;
    double c = (cc * PARAM * PARAM);

    // The following formula is to calculate the perpendicular.
    double aa = a * a;
    double bb = b * b;
    double ab = a * b;
    double ac = a * c;
    double bc = b * c;

    double x1 = bb * lGpsLongitude - ab * lGpsLatitude - ac;
    double x2 = aa + bb;
    MatchNodeOut.lon = x1 / x2 / PARAM;

    double y1 = aa * lGpsLatitude - ab * lGpsLongitude - bc;
    double y2 = x2;
    MatchNodeOut.lat = y1 / y2 / PARAM;

    return true;
}

/**
 *  Line is (x1,y1) to (x2,y2), point is (x3,y3).
 *
 *  float check_value = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
 *
 *  return a negative check value ( < 0 ) if the point is "to the left" of the line,
 *  zero ( == 0 ) if the point is on the line,
 *  a positive value ( > 0 ) if it's on "the right".
 *
 *  http://wiki.processing.org/w/Find_which_side_of_a_line_a_point_is_on
 */
inline LinkPointPosition
linePointPosition2D(LonLat Vtx1, LonLat Vtx2, LonLat lonlat3)
{
    float value1 = (Vtx2.lon - Vtx1.lon) * (lonlat3.lat - Vtx1.lat)
                   - (Vtx2.lat - Vtx1.lat) * (lonlat3.lon - Vtx1.lon);

    if (value1 == 0) {
        return POINT_ON_LINK;
    }
    else if (value1 < 0) {
        return LINK_RIGHT_SIDE;
    }
    else {
        return LINK_LEFT_SIDE;
    }
}

#endif
