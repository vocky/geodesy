//
//  geodesic.h
//  geodesy
//
//  Created by ray on 7/17/14.
//  Copyright (c) 2014 ray. All rights reserved.
//

// The term "geodesic" comes from geodesy, the science of measuring the size
// and shape of Earth; in the original sense, a geodesic was the shortest route
// between two points on the Earth's surface, namely, a segment of a great
// circle.
//
// In geometric geodesy, two standard problems exist:
//
// First (direct) geodetic problem:
//
//      Given a point (in terms of its coordinates) and the direction (azimuth)
//      and distance from that point to a second point, determine
//      (the coordinates of) that second point.
//
// Second (inverse) geodetic problem:
//      Given two points, determine the azimuth and length of the line
//      (straight line, arc or geodesic) that connects them.
//
// http://en.wikipedia.org/wiki/Geodesy#Geodetic_problems
//
// The formulae in this source file calculate distances between points which
// are defined by geographical coordinates in terms of latitude and longitude.
// This distance is an element in solving the second (inverse) geodetic problem.
//
// All these formulae are for calculations on the basis of a spherical earth
// (ignoring ellipsoidal effects) – which is accurate enough for most purposes.
// If you want greater accuracy, you could use the Vincenty formula for
// calculating geodesic distances on ellipsoids, which gives results accurate
// to within 1mm.
// 
// The sphere model is based on WGS84 datum, which is a standard for use in
// cartography, geodesy, and navigation. It is the reference coordinate system
// used by the Global Positioning System.
// The WGS 84 datum surface is an oblate spheroid (ellipsoid) with major
// (equatorial) radius a = 6378137m at the equator and flattening
// f = 1/298.257223563. The polar semi-minor axis b then equals a times (1−f),
// or 6356752.314245m.
//
// http://en.wikipedia.org/wiki/WGS84
//

#ifndef geodesy_distance_h
#define geodesy_distance_h

#include "geodesic.h"
#include <math.h>

// WGS84 Datum
const double LOC_EARTH_ECC2 = 0.0066943799013;
const double HALF_CYCLE = 180.0;


float
pbHypotenuseF(float DeltaX, float DeltaY)
{
    int clac_cnt;
    float tmp_calc;
    float delta_x;
    float delta_y;

    delta_x = (float) fabs((double) DeltaX);
    delta_y = (float) fabs((double) DeltaY);

    if (delta_x < delta_y) {
        tmp_calc = delta_x;
        delta_x = delta_y;
        delta_y = tmp_calc;
    }

    if (delta_y == 0.0F) {
        return (delta_x);
    }

    for (clac_cnt = 0; clac_cnt < 2; clac_cnt++) {
        tmp_calc = delta_y / delta_x;
        tmp_calc *= tmp_calc;
        tmp_calc /= (4.0F + tmp_calc);
        delta_x += (2.0F * delta_x * tmp_calc);
        delta_y *= tmp_calc;
    }

    return (delta_x);
}

float CalcDistance(double latitude, float delta_lon, float delta_lat)
{
    double lat = 0.0;          // latitude [RAD]
    double r1 = 0.0;          // 2πA/3600/360
    double r2 = 0.0;          // sqrt(1-e^2*sinφ^2)
    double coslat = 0.0;          // Cosine value of latitude
    double sinlat = 0.0;          // Sine value of latitude
    float mps_lon = 0.0; // Convert parameter in longitude direction [m/sec]
    float mps_lat = 0.0;   // Convert parameter in latitude direcion [m/sec]

    // Get latitude value in [RAD]
    lat = latitude / 3600.0 * M_PI / HALF_CYCLE;

    // Calculate triangle functions value
    coslat = cos(lat);
    sinlat = sin(lat);

    // Calculate the convert parameter
    r1 = 2.0 * M_PI * WGS84_SEMI_MAJOR / 3600.0 / 360.0;
    r2 = sqrt(1 - LOC_EARTH_ECC2 * sinlat * sinlat);
    mps_lon = float(r1 * coslat / r2);
    mps_lat = float(r1 * (1 - LOC_EARTH_ECC2) / (r2 * r2 * r2));

    // Get the result distance
    return pbHypotenuseF(mps_lon * delta_lon, mps_lat * delta_lat);
}

// The Great Circle Distance
// http://en.wikipedia.org/wiki/Great-circle_distance
//
// Law of cosines:
//
// d = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lng1 - lng2))
//
double great_circle_distance(double lng1, double lat1, double lng2, double lat2) {
    
    double cdlng = cos(radian(lng1 - lng2));
    
    double rlat1 = radian(lat1);
    double rlat2 = radian(lat2);
    
    double a = sin(rlat1) * sin(rlat2) + cos(rlat1) * cos(rlat2) * cdlng;
    
    // use min() protects the roundoff errors
    return MEAN_EARTH_RADIUS * acos(fmin(1.0, a));
    
}

// The haversine formular
// http://en.wikipedia.org/wiki/Haversine_formula
//
// This is a mathematically equivalent formula to great_circle_distance, which
// is less subject to rounding error.
//
// d = 2 * arcsin(sqrt(
//        sin((lat1 - lat2) / 2) ^ 2 +
//        cos(lat1) * cos(lat2) * sin((lng1 - lng2) / 2) ^ 2
//     ))
//
double haversine_distance(double lng1, double lat1, double lng2, double lat2) {
    
    double sdlat = sin(0.5 * radian(lat1 - lat2));
    double sdlng = sin(0.5 * radian(lng1 - lng2));
    
    double clat1 = cos(radian(lat1));
    double clat2 = cos(radian(lat2));
    
    double a = sdlat * sdlat + clat1 * clat2 * sdlng * sdlng;
    
    // use min() protects the roundoff errors
    return MEAN_EARTH_RADIUS * 2. * asin(fmin(1.0, sqrt(a)));
    
}

// The special case of Vincenty formula (spheroid)
// A more complicated formula that is accurate for all distances.
//
// d = R * arctan(
//      sqrt(
//          (cos(lat2) * sin(lng1 - lng2)) ^ 2 +
//          (cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lng1 - lng2)) ^ 2
//      ) /
//      sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lng1 - lng2)
// )
//
double spheroid_distance(double lng1, double lat1, double lng2, double lat2) {
    
    double rlat1 = radian(lat1);
    double rlat2 = radian(lat2);
    double rdlng = radian(lng1 - lng2);
    
    double slat1 = sin(rlat1);
    double slat2 = sin(rlat2);
    double clat1 = cos(rlat1);
    double clat2 = cos(rlat2);
    double sdlng = sin(rdlng);
    double cdlng = cos(rdlng);
    
    double d = MEAN_EARTH_RADIUS * atan2(
        sqrt(
          (clat2 * sdlng) * (clat2 * sdlng) +
          (clat1 * slat2 - slat1 * clat2 * cdlng) *
          (clat1 * slat2 - slat1 * clat2 * cdlng)
          ),
        slat1 * slat2 + clat1 * clat2 * cdlng
    );
    
    return d;
}

// The Pythagoras’ theorem
// http://en.wikipedia.org/wiki/Geographical_distance#Spherical_Earth_projected_to_a_plane
//
// If performance is an issue and accuracy less important, for small distances
// Pythagoras’ theorem can be used on an equirectangular projection.
//
// d = R * sqrt(
//          (lat1 - lat2) ^ 2 +
//          (cos((lat1 + lat2) / 2) * (lng1 - lng2)) ^ 2
// )
//
double pythagoras_distance(double lng1, double lat1, double lng2, double lat2) {
    
    double dlng = radian(lng1 - lng2);
    double dlat = radian(lat1 - lat2);
    
    double mlat = 0.5 * radian(lat1 + lat2);
    float tempdata = cose(mlat);
    double d = MEAN_EARTH_RADIUS * sqrt(
        dlat * dlat + (tempdata * dlng) * (tempdata * dlng)
    );
    
    return d;
}

/* calculate distance use PbHypotenuse's formula.
 * */
double pbhypotenuse_distance(double lng1, double lat1, double lng2, double lat2)
{
    double latitude = lat1 * 3600;
    float delta_lon = (lng1 - lng2) * 3600;
    float delta_lat = (lat1 - lat2) * 3600;

    return CalcDistance(latitude, delta_lon, delta_lat);
}

/* Calculate distance use Vincenty's formula.
 * Vincenty reference:http://en.wikipedia.org/wiki/Vincenty's_formulae
 * The result of Vincenty's formula is showed in:
 * www.ga.gov.au/geodesy/datums/vincenty_inverse.jsp
 * */
double vincenty_distance(double lng1, double lat1, double lng2, double lat2) {
    double lat_1 = (M_PI/180.0) * lat1;
    double lon_1 = (M_PI/180.0) * lng1;
    double lat_2 = (M_PI/180.0) * lat2;
    double lon_2 = (M_PI/180.0) * lng2;
    return llh_rad_to_metres(lon_1, lat_1, lon_2, lat_2);
}
#endif
