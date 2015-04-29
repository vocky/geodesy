/*
 * define.h
 *
 *  Created on: July 19, 2014
 *      Author: laye
 */

#ifndef DEFINE_H_
#define DEFINE_H_

#define PI                      3.14159265358979323846

typedef struct {
    double lon;
    double lat;
}LonLat;

typedef struct {
    double lon;
    double lat;
}Point;

enum LinkPointPosition {
    POINT_ON_LINK = 0, LINK_RIGHT_SIDE, LINK_LEFT_SIDE
};

typedef struct {double x,y;}				MAP_DBPOINT;
typedef struct {int x0,y0,x1,y1;}			MAP_RECT;

#endif
