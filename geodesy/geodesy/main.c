//
//  main.c
//  geodesy
//
//  Created by ray on 7/17/14.
//  Copyright (c) 2014 ray. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "geodesic.h"
#include "distance.h"

typedef struct test_case {
    double lng1;
    double lat1;
    double lng2;
    double lat2;
    double expect;
} test_case_type;


const int test_count = 37;

test_case_type tests[test_count] = {
    
    // tests for origin
    {     0.,    0.,    0.,    0.,   0.}, // 0.
    
    // tests for singularity
    {     0.,   90.,    0.,   90.,   0.}, // north pole
    {   180.,   90.,  180.,   90.,   0.},
    {  -180.,   90., -180.,   90.,   0.},
    {     0.,  -90.,    0.,  -90.,   0.}, // sourth pole
    {   180.,  -90.,  180.,  -90.,   0.},
    {  -180.,  -90., -180.,  -90.,   0.},

    // tests for discontinuity
    {  -180.,    0.,  180.,    0.,   0.}, // +/- 180
    {   180.,    0., -180.,    0.,   0.}, // +/- 180
    {  -179.,    0.,  181.,    0.,   0.},
    {  -181.,    0.,  179.,    0.,   0.},
    
    {   179.,    0.,  181.,    0.,   2. / 180. * M_PI * MEAN_EARTH_RADIUS},
    {   179.,    0., -179.,    0.,   2. / 180. * M_PI * MEAN_EARTH_RADIUS},
    {  -179.,    0., -181.,    0.,   2. / 180. * M_PI * MEAN_EARTH_RADIUS},
    {  -179.,    0.,  179.,    0.,   2. / 180. * M_PI * MEAN_EARTH_RADIUS},
    
    {    45.,   89., -135.,   89.,   2. / 180. * M_PI * MEAN_EARTH_RADIUS},
    {    45.,   89.,   45.,   91.,   2. / 180. * M_PI * MEAN_EARTH_RADIUS},
    {    45.,  -89., -135.,  -89.,   2. / 180. * M_PI * MEAN_EARTH_RADIUS},
    {    45.,  -89.,   45.,  -91.,   2. / 180. * M_PI * MEAN_EARTH_RADIUS},
    
    // tests for medians and equator
    {     0.,    0.,  180.,    0.,   M_PI * MEAN_EARTH_RADIUS},
    {     0.,    0., -180.,    0.,   M_PI * MEAN_EARTH_RADIUS},
    {     0.,    0.,    0.,   90.,   M_PI_2 * MEAN_EARTH_RADIUS},
    {     0.,    0.,    0.,  -90.,   M_PI_2 * MEAN_EARTH_RADIUS},
    
    {   180.,    0.,    0.,    0.,   M_PI * MEAN_EARTH_RADIUS},
    {  -180.,    0.,    0.,    0.,   M_PI * MEAN_EARTH_RADIUS},
    {     0.,   90.,    0.,    0.,   M_PI_2 * MEAN_EARTH_RADIUS},
    {     0.,  -90.,    0.,    0.,   M_PI_2 * MEAN_EARTH_RADIUS},
    
    // tests for high latitude <= 90
    // tests for mid latitude  <= 60
    {-99.181963, 49.464914, -99.110037, 49.432055, 8320},     // 8.32km
    {-100.285230, 49.934079, -99.112440, 49.440149, 134490},  // 134.49km
    {108.861466, 34.405671, 116.398087, 40.001222, 1351370}, // 1351.37km
    {-108.288770, 38.327793, -62.937209, 30.944298, 5262950},  // 5262.95km
    // tests for low latitude  <= 30
    {-99.181963, 19.464914, -99.110037, 19.432055, 8320},     // 8.32km
    {-100.285230, 19.934079, -99.112440, 19.440149, 134490},  // 134.49km
    {-107.409864, 28.928336, -99.016309, 19.442739, 1351370}, // 1351.37km
    {-108.288770, 8.327793, -62.937209, -3.944298, 5262950},  // 5262.95km
    
    // From Farnham to Reigate
    {-0.799, 51.214, -0.188, 51.230}, //42.5e3},
    // From London to Sidney
    {-0.13, 51.51, 151.21, -33.86}, //16998.e3},
    
};

const int counts = 12;
typedef struct {
    double lon;
    double lat;
} Point;
Point testcases[counts] = {
    // tests for origin
    {139.7716903, 35.6819149},
    {139.7713094, 35.6812936},
    {139.0, 35.0},
    {150.0, 30.0},
    {150.0, -30.0},
    {120.0, 60.0},
    {120.0, -60.0},
    {20.0, -30.0},
    {20.0, -20.0},
    {20.0, -10.0},
    {20.0, 0.0},
    {20.0, 10.0},
};

double RandDB(double a, double b) {
    return (a + (rand() * 1.0) / RAND_MAX * (b - a));
}

const int testnum = 50000;
void test_lengthefficient() {
    printf("------------------------------------------------\n");
    printf("Test the cost time of length calculation.\n");
    printf("The test cases are: %d.\n", testnum);
    test_case_type testpoint[testnum];
    for(int i = 0; i < testnum; ++i) {
        testpoint[i].lng1 = RandDB(-180.0, 180.0);
        testpoint[i].lat1 = RandDB(-90.0, 90.0);
        testpoint[i].lng2 = RandDB(-180.0, 180.0);
        testpoint[i].lat2 = RandDB(-90.0, 90.0);
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        spheroid_distance(testpoint[i].lng1, testpoint[i].lat1, testpoint[i].lng2, testpoint[i].lat2);
    }
    gettimeofday(&end, NULL);
    int micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of Spheroid's formula: %d ms\n", micros);

    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        great_circle_distance(testpoint[i].lng1, testpoint[i].lat1, testpoint[i].lng2, testpoint[i].lat2);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of GreatCircle's formula: %d ms\n", micros);

    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        haversine_distance(testpoint[i].lng1, testpoint[i].lat1, testpoint[i].lng2, testpoint[i].lat2);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of Haversine's formula: %d ms\n", micros);

    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        pythagoras_distance(testpoint[i].lng1, testpoint[i].lat1, testpoint[i].lng2, testpoint[i].lat2);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of Pythagoras's formula: %d ms\n", micros);

    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        pbhypotenuse_distance(testpoint[i].lng1, testpoint[i].lat1, testpoint[i].lng2, testpoint[i].lat2);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of Pbhypotenuse's formula: %d ms\n", micros);

    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        vincenty_distance(testpoint[i].lng1, testpoint[i].lat1, testpoint[i].lng2, testpoint[i].lat2);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of Vincenty's formula: %d ms\n", micros);
}

int test_lengtherror() {
    printf("------------------------------------------------\n");
    printf("The table below shows distances calculated by each of the methods,\n");
    printf("which are Spheroid, GreatCirle, Haversine, pythagoras, PbHypotenuse, Vincenty.\n");
    printf("Spheroid, The special case of Vincenty formula (spheroid)\n");
    printf("GreatCirle reference:http://en.wikipedia.org/wiki/Great_circle\n");
    printf("Haversine reference:http://en.wikipedia.org/wiki/Haversine_formula\n");
    printf("Pythagoras reference:http://en.wikipedia.org/wiki/Geographical_distance#Spherical_Earth_projected_to_a_plane\n");
    printf("PbHypotenuse reference:http://en.wikipedia.org/wiki/Right_triangle\n");
    printf("Vincenty reference:http://en.wikipedia.org/wiki/Vincenty's_formulae\n");
    printf("Then you can get the ""best"" answer by compare to the standard Vincenty's formula.\n");
    printf("The result of Vincenty's formula is showed in: \n");
    printf("www.ga.gov.au/geodesy/datums/vincenty_inverse.jsp\n");
    printf("The sample points:\n");
    printf("Point    Longitude        Latitude\n");
    printf("A        139.7716903      35.6819149\n");
    printf("B        139.7713094      35.6812936\n");
    printf("C        139.0            35.0\n");
    printf("D        150.0            30.0\n");
    printf("E        150.0            -30.0\n");
    printf("F        120.0            60.0\n");
    printf("G        120.0            -60.0\n");
    printf("H        20.0             -30.0\n");
    printf("I        20.0             -20.0\n");
    printf("J        20.0             -10.0\n");
    printf("K        20.0             0.0\n");
    printf("L        20.0             10.0\n");
    printf("The result:\n");
    printf("From    Method1      Method2         Method3      Method4         Method5         Method6\n");
    printf("To      Spheroid     GreatCircle     Haversine    Pythagoras      pbhypotenuse    Vincenty\n");

    double d1, d2, d3, d4, d5, d6;
    double deviation1, deviation2, deviation3, deviation4, deviation5;
    deviation1 = deviation2 = deviation3 = deviation4 = deviation5 = 0.0;
    int i;
    for (i = 0; i < counts; ++i) {
        if (i == 0) {
            d1 = spheroid_distance(testcases[i].lon, testcases[i].lat, testcases[i].lon, testcases[i].lat);
            d2 = great_circle_distance(testcases[i].lon, testcases[i].lat, testcases[i].lon, testcases[i].lat);
            d3 = haversine_distance(testcases[i].lon, testcases[i].lat, testcases[i].lon, testcases[i].lat);
            d4 = pythagoras_distance(testcases[i].lon, testcases[i].lat, testcases[i].lon, testcases[i].lat);
            d5 = pbhypotenuse_distance(testcases[i].lon, testcases[i].lat, testcases[i].lon, testcases[i].lat);
            d6 = vincenty_distance(testcases[i].lon, testcases[i].lat, testcases[i].lon, testcases[i].lat);
        }else {
            d1 = spheroid_distance(testcases[0].lon, testcases[0].lat, testcases[i].lon, testcases[i].lat);
            d2 = great_circle_distance(testcases[0].lon, testcases[0].lat, testcases[i].lon, testcases[i].lat);
            d3 = haversine_distance(testcases[0].lon, testcases[0].lat, testcases[i].lon, testcases[i].lat);
            d4 = pythagoras_distance(testcases[0].lon, testcases[0].lat, testcases[i].lon, testcases[i].lat);
            d5 = pbhypotenuse_distance(testcases[0].lon, testcases[0].lat, testcases[i].lon, testcases[i].lat);
            d6 = vincenty_distance(testcases[0].lon, testcases[0].lat, testcases[i].lon, testcases[i].lat);
        }
        deviation1 += pow((d1 - d6), 2);
        deviation2 += pow((d2 - d6), 2);
        deviation3 += pow((d3 - d6), 2);
        deviation4 += pow((d4 - d6), 2);
        deviation5 += pow((d5 - d6), 2);

        printf("%c%c      %-11.2f  %-11.2f     %-11.2f  %-11.2f     %-11.2f     %-11.2f\n",
               65, i+65, d1, d2, d3, d4, d5, d6);
    }
    deviation1 = sqrt(deviation1);
    deviation2 = sqrt(deviation2);
    deviation3 = sqrt(deviation3);
    deviation4 = sqrt(deviation4);
    printf("Accumulative error:\n");
    printf("Error   %-11.2f  %-11.2f     %-11.2f  %-11.2f     %-11.2f   0.0\n",
           deviation1, deviation2, deviation3, deviation4, deviation5);
    return i;
}

int test_distance() {
    int i;
    for (i = 0; i < test_count; i++) {
        test_case_type test = tests[i];
        printf("Test Case: %d, ", i);
        printf("Point1: %.6f, %.6f, ", test.lng1, test.lat1);
        printf("Point2: %.6f, %.6f, ", test.lng2, test.lat2);
        printf("Expeted: %.2f. \n", test.expect);
        
        double d = 0;
        
        d = pythagoras_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        printf("Plane distance:     %.12f \n", d);
        
        d = spheroid_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        printf("Spheroid distance:  %.12f \n", d);
        
        d = haversine_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        printf("Haversine distance: %.12f \n", d);
        
        d = great_circle_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        printf("Arc distance:       %.12f \n", d);
        
        
        printf("\n");
    }
    return i;
}


double benchmark_plane_distance(int iteration) {
    int i, j;
    double d = 0.;
    for (i = 0; i < iteration; i++) {
        for (j = 0; j < test_count; j++) {
            test_case_type test = tests[j];
            d += pythagoras_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        }
    }
    return d;
}


double benchmark_spheroid_distance(int iteration) {
    int i, j;
    double d = 0;
    for (i = 0; i < iteration; i++) {
        for (j = 0; j < test_count; j++) {
            test_case_type test = tests[j];
            d += spheroid_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        }
    }
    return d;
}

double benchmark_haversine_distance(int iteration) {
    int i, j;
    double d = 0.;
    for (i = 0; i < iteration; i++) {
        for (j = 0; j < test_count; j++) {
            test_case_type test = tests[j];
            d += haversine_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        }
    }
    return d;
}

double benchmark_arc_distance(int iteration) {
    int i, j;
    double d = 0.;
    for (i = 0; i < iteration; i++) {
        for (j = 0; j < test_count; j++) {
            test_case_type test = tests[j];
            d += great_circle_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        }
    }
    return d;
}

void test_length() {
    int i;
    for (i = 0; i < test_count; i++) {
        test_case_type test = tests[i];
        printf("Test Case: %d, ", i);
        printf("Point1: %.6f, %.6f, ", test.lng1, test.lat1);
        printf("Point2: %.6f, %.6f, ", test.lng2, test.lat2);
        printf("Expeted: %.2f. \n", test.expect);

        double d = 0;

        d = common_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        printf("Common distance:     %.12f \n", d);

        d = optimal_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        printf("The most optimal distance:  %.12f \n", d);

        d = fastest_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        printf("the most fastest distance: %.12f \n", d);

        d = accurate_distance(test.lng1, test.lat1, test.lng2, test.lat2);
        printf("Accurate distance:       %.12f \n", d);


        printf("\n");
    }
    printf("------------------------------------------------\nTest the efficient.\n");
    printf("The test cases are: %d.\n", testnum);
    test_case_type testpoint[testnum];
    for(int i = 0; i < testnum; ++i) {
        testpoint[i].lng1 = RandDB(-180.0, 180.0);
        testpoint[i].lat1 = RandDB(-90.0, 90.0);
        testpoint[i].lng2 = RandDB(-180.0, 180.0);
        testpoint[i].lat2 = RandDB(-90.0, 90.0);
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        common_distance(testpoint[i].lng1, testpoint[i].lat1, testpoint[i].lng2, testpoint[i].lat2);
    }
    gettimeofday(&end, NULL);
    int micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of common distance: %d ms\n", micros);

    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        optimal_distance(testpoint[i].lng1, testpoint[i].lat1, testpoint[i].lng2, testpoint[i].lat2);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of optimal distance: %d ms\n", micros);

    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        fastest_distance(testpoint[i].lng1, testpoint[i].lat1, testpoint[i].lng2, testpoint[i].lat2);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of fastest distance: %d ms\n", micros);

    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        accurate_distance(testpoint[i].lng1, testpoint[i].lat1, testpoint[i].lng2, testpoint[i].lat2);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of accurate distance: %d ms\n", micros);
    printf("------------------------------------------------\n");
    printf("The table below shows errors calculated by each of the methods.\n");
    printf("The sample points:\n");
    printf("Point    Longitude        Latitude\n");
    printf("A        139.7716903      35.6819149\n");
    printf("B        139.7713094      35.6812936\n");
    printf("C        139.0            35.0\n");
    printf("D        150.0            30.0\n");
    printf("E        150.0            -30.0\n");
    printf("F        120.0            60.0\n");
    printf("G        120.0            -60.0\n");
    printf("H        20.0             -30.0\n");
    printf("I        20.0             -20.0\n");
    printf("J        20.0             -10.0\n");
    printf("K        20.0             0.0\n");
    printf("L        20.0             10.0\n");
    printf("The result:\n");
    printf("From    Method1      Method2         Method3      Method4\n");
    printf("To      common       optimal         fastest      accurate\n");

    double d1, d2, d3, d4;
    double deviation1, deviation2, deviation3;
    deviation1 = deviation2 = deviation3 = 0.0;
    for (i = 0; i < counts; ++i) {
        if (i == 0) {
            d1 = common_distance(testcases[i].lon, testcases[i].lat, testcases[i].lon, testcases[i].lat);
            d2 = optimal_distance(testcases[i].lon, testcases[i].lat, testcases[i].lon, testcases[i].lat);
            d3 = fastest_distance(testcases[i].lon, testcases[i].lat, testcases[i].lon, testcases[i].lat);
            d4 = accurate_distance(testcases[i].lon, testcases[i].lat, testcases[i].lon, testcases[i].lat);
        }else {
            d1 = common_distance(testcases[0].lon, testcases[0].lat, testcases[i].lon, testcases[i].lat);
            d2 = optimal_distance(testcases[0].lon, testcases[0].lat, testcases[i].lon, testcases[i].lat);
            d3 = fastest_distance(testcases[0].lon, testcases[0].lat, testcases[i].lon, testcases[i].lat);
            d4 = accurate_distance(testcases[0].lon, testcases[0].lat, testcases[i].lon, testcases[i].lat);
        }
        deviation1 += pow((d1 - d4), 2);
        deviation2 += pow((d2 - d4), 2);
        deviation3 += pow((d3 - d4), 2);
        printf("%c%c      %-11.2f  %-11.2f     %-11.2f  %-11.2f\n",
               65, i+65, d1, d2, d3, d4);
    }
    deviation1 = sqrt(deviation1);
    deviation2 = sqrt(deviation2);
    deviation3 = sqrt(deviation3);
    printf("Accumulative error:\n");
    printf("Error   %-11.2f  %-11.2f     %-11.2f  0.0\n",
           deviation1, deviation2, deviation3);
}

int main(int argc, const char * argv[])
{
    //int iteration = 10000000;
    //double elapsed = 0.;
    //time_t tic, tac;
    
    //test_distance();
    //test_lengtherror();
    //test_lengthefficient();
    test_length();
    
//    // ========================================================
//    tic = clock();
//    printf("%f\n", benchmark_plane_distance(iteration));
//    tac = clock();
//    
//    elapsed = (double)(tac - tic) / CLOCKS_PER_SEC;
//    
//    printf("Iteration %d, Elapsed: %f.\n", iteration, elapsed);
//    printf("%f call per second.\n", test_count * iteration / elapsed);
//    
//    
//    // ========================================================
//    tic = clock();
//    printf("%f\n", benchmark_spheroid_distance(iteration));
//    tac = clock();
//    
//    elapsed = (double)(tac - tic) / CLOCKS_PER_SEC;
//    
//    printf("Iteration %d, Elapsed: %f.\n", iteration, elapsed);
//    printf("%f iteration per second.\n", iteration/elapsed);
//    
//    // ========================================================
//    
//    tic = clock();
//    printf("%f\n", benchmark_haversine_distance(iteration));
//    tac = clock();
//
//    elapsed = (double)(tac - tic) / CLOCKS_PER_SEC;
//    
//    printf("Iteration %d, Elapsed: %f.\n", iteration, elapsed);
//    printf("%f iteration per second.\n", iteration/elapsed);
//
//   
//    // ========================================================
//    
//    tic = clock();
//    printf("%f\n", benchmark_arc_distance(iteration));
//    tac = clock();
//    
//    elapsed = (double)(tac - tic) / CLOCKS_PER_SEC;
//    
//    printf("Iteration %d, Elapsed: %f.\n", iteration, elapsed);
//    printf("%f iteration per second.\n", iteration/elapsed);
//    
//    
//    
    
    return 0;
}

