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

#ifndef geodesy_geodesic_h
#define geodesy_geodesic_h

#include <math.h>

// WGS84 Datum
const double WGS84_SEMI_MAJOR = 6378137.0;
const double WGS84_SEMI_MINOR = 6356752.314245;
const double WGS84_FALTTENING = WGS84_SEMI_MAJOR / WGS84_SEMI_MINOR - 1;
const double MEAN_EARTH_RADIUS = (2 * WGS84_SEMI_MAJOR + WGS84_SEMI_MINOR) / 3;

float sine(float x) {
    if (x < -M_PI) {
        x += 6.28318531;
    }
    else if (x > M_PI) {
        x -= 6.28318531;
    }

    float result = 0.0;
    //compute sine
    if (x < 0) {
        result = 1.27323954 * x + .405284735 * x * x;

        if (result < 0) {
            result = .225 * (result *-result - result) + result;
        }
        else {
            result = .225 * (result * result - result) + result;
        }
    }
    else {
        result = 1.27323954 * x - 0.405284735 * x * x;

        if (result < 0) {
            result = .225 * (result *-result - result) + result;
        }
        else {
            result = .225 * (result * result - result) + result;
        }
    }
    return result;
}

float cose(float x) {
    x += 1.57079632;
    if (x >  M_PI)
        x -= 6.28318531;

    float result = 0.0;
    if (x < 0) {
        result = 1.27323954 * x + 0.405284735 * x * x;
        if (result < 0) {
            result = .225 * (result *-result - result) + result;
        }
        else {
            result = .225 * (result * result - result) + result;
        }
    }
    else {
        result = 1.27323954 * x - 0.405284735 * x * x;

        if (result < 0) {
            result = .225 * (result *-result - result) + result;
        }
        else {
            result = .225 * (result * result - result) + result;
        }
    }
    return result;
}

float squareroot(float x) {
    union {
        int i;
        float x;
    } u;
    u.x = x;
    u.i = (1<<29) + (u.i >> 1) - (1<<22);

    // Two Babylonian Steps (simplified from:)
    // u.x = 0.5f * (u.x + x/u.x);
    // u.x = 0.5f * (u.x + x/u.x);
    u.x =       u.x + x/u.x;
    u.x = 0.25f*u.x + x/u.x;

    return u.x;
}

inline double radian(double degree) {
    // convert degree to radian
    return degree * (M_PI / 180.);
}


inline double degree(double radian) {
    // convert radian to degree
    return radian * (180. / M_PI);
}


inline int is_valid_lnglat(double lng, double lat) {
    // validate longitude and latitude
    return fabs(lat) <= 90. && fabs(lng) <= 180.;
}


inline double normalize_lng(double lng) {
    // normalize longitude to 0~360 degree
    return remainder(lng, 360.);
}


inline double normalize_lat(double lat) {
    // normalize latitude to +/-90 degree
    return fmax(-90., fmin(90., lat));
}

/* Radians positions -> metres apart along geodesic */
double llh_rad_to_metres(double lng1, double lat1, double lng2, double lat2) {
    double U1 = atan((1.0 - WGS84_FALTTENING) * tan(lat1));
    double U2 = atan((1.0 - WGS84_FALTTENING) * tan(lat2));
    double L, lambda, lambda0;
    double cos_2sigma_m;
    double cos2_alpha;
    double sigma;
    L = lng2 - lng1;
    lambda = L;
    if (fabs(L) < 1.0e-12 && (fabs(lat1 - lat2) < 1.0e-12))
        return 0.0;
    do {
        lambda0 = lambda;
        double t1 = cos(U2)*sin(lambda);
        double t2 = cos(U1)*sin(U2) - sin(U1)*cos(U2)*cos(lambda);
        double sin_sigma = sqrt(t1*t1 + t2*t2);
        double cos_sigma = sin(U1)*sin(U2) + cos(U1)*cos(U2)*cos(lambda);
        sigma = atan2(sin_sigma, cos_sigma);
        /* is this meant to use sin_sigma or a freshly computed sin(sigma) ? */
        double sin_alpha = cos(U1)*cos(U2)*sin(lambda)/sin(sigma);
        double sin2_alpha = sin_alpha * sin_alpha;
        cos2_alpha = 1.0 - sin2_alpha;
        cos_2sigma_m = cos(sigma) - 2.0*sin(U1)*sin(U2)/cos2_alpha;
        double C = (1.0/16.0) * WGS84_FALTTENING * cos2_alpha * (4.0 + WGS84_FALTTENING * (4.0 - 3.0*cos2_alpha));
        double squares = cos_2sigma_m + C * cos(sigma) * (-1.0 + 2.0 * cos_2sigma_m * cos_2sigma_m);
        double braces = sigma + C * sin(sigma) * squares;
        lambda = L + (1.0 - C) * WGS84_FALTTENING * sin_alpha * braces;
    } while (fabs(lambda0 - lambda) > 1.0e-12);

    double u2 = cos2_alpha * (WGS84_SEMI_MAJOR - WGS84_SEMI_MINOR) * (WGS84_SEMI_MAJOR + WGS84_SEMI_MINOR) / (WGS84_SEMI_MINOR*WGS84_SEMI_MINOR);
    double A = 1.0 + (u2 / 16384.0) * (
      4096.0 + u2 * (-768.0 + u2*(320.0 - 175.0*u2))
      );
    double B = (u2 / 1024.0) * (
      256.0 + u2*(-128.0 + u2*(74.0 - 47.0*u2))
      );
    double z1 = cos(sigma)*(-1.0 + 2.0*cos_2sigma_m*cos_2sigma_m);
    double z2 = -3.0 + 4.0 * sin(sigma)*sin(sigma);
    double z3 = -3.0 + 4.0 * cos_2sigma_m * cos_2sigma_m;
    double z4 = (B/6.0) * cos_2sigma_m * z2 * z3;
    double DELTA_sigma = B * sin(sigma) * (
      cos_2sigma_m + (B/4.0) * (z1 - z4)
      );
    double s = WGS84_SEMI_MINOR * A * (sigma - DELTA_sigma);
    return s;
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
double common_distance(double lng1, double lat1, double lng2, double lat2) {
    
    double sdlat = sin(0.5 * radian(lat1 - lat2));
    double sdlng = sin(0.5 * radian(lng1 - lng2));
    
    double clat1 = cos(radian(lat1));
    double clat2 = cos(radian(lat2));
    
    double a = sdlat * sdlat + clat1 * clat2 * sdlng * sdlng;
    
    // use min() protects the roundoff errors
    return MEAN_EARTH_RADIUS * 2. * asin(fmin(1.0, sqrt(a)));
    
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
double optimal_distance(double lng1, double lat1, double lng2, double lat2) {
    
    double dlng = radian(lng1 - lng2);
    double dlat = radian(lat1 - lat2);
    
    double mlat = 0.5 * radian(lat1 + lat2);
    float tempdata = cos(mlat);
    double d = MEAN_EARTH_RADIUS * sqrt(
        dlat * dlat + (tempdata * dlng) * (tempdata * dlng)
    );
    
    return d;
}

double fastest_distance(double lng1, double lat1, double lng2, double lat2) {

    double dlng = radian(lng1 - lng2);
    double dlat = radian(lat1 - lat2);

    double mlat = 0.5 * radian(lat1 + lat2);
    float tempdata = cose(mlat);
    double d = MEAN_EARTH_RADIUS * sqrt(
        dlat * dlat + (tempdata * dlng) * (tempdata * dlng)
    );

    return d;
}

/* Calculate distance use Vincenty's formula.
 * Vincenty reference:http://en.wikipedia.org/wiki/Vincenty's_formulae
 * The result of Vincenty's formula is showed in:
 * www.ga.gov.au/geodesy/datums/vincenty_inverse.jsp
 * */
double accurate_distance(double lng1, double lat1, double lng2, double lat2) {
    double lat_1 = (M_PI/180.0) * lat1;
    double lon_1 = (M_PI/180.0) * lng1;
    double lat_2 = (M_PI/180.0) * lat2;
    double lon_2 = (M_PI/180.0) * lng2;
    return llh_rad_to_metres(lon_1, lat_1, lon_2, lat_2);
}
#endif
