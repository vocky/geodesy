#include "triangle.h"
#include "squarerootmethods.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string.h>
#include <sys/time.h>
using namespace std;

int totalnum= 20000000;
int testnum = 50000;

/* Get a random double in [a,b]
 * */
double RandDB(double a, double b) {
    return (a + (rand() * 1.0) / RAND_MAX * (b - a));
}

/* Test the error of square root.
 *
 * Use Euclidean distance to get the deviation between calculated value and truth value.*/
void SquareRoot_Test() {
    printf("------------------------------------------------\n");
    double qrt0, qrt1, qrt2;
    double distance0, distance1, distance2;
    distance0 = distance1 = distance2 = 0.0;
    vector<double> vecTestValue, vecTestValue1, vecTestValue2, vecTestValue3;
    for (int i = 0; i < testnum; ++i) {
        double db = RandDB(0.1, 1.0e5);
        vecTestValue.push_back(db);
    }
    for (int i = 0; i < testnum; ++i) {
        double db = RandDB(1.0e5, 1.0e10);
        vecTestValue1.push_back(db);
    }
    for (int i = 0; i < testnum; ++i) {
        double db = RandDB(1.0e10, 1.0e15);
        vecTestValue2.push_back(db);
    }
    for (int i = 0; i < testnum; ++i) {
        double db = RandDB(1.0e15, 1.0e20);
        vecTestValue3.push_back(db);
    }

    for (int i = 0; i < int(vecTestValue.size()); ++i) {
        double db = vecTestValue[i];
        qrt0 = sqrt(db);
        distance0 = 0;

        qrt1 = squareroot(db);
        distance1 += pow((qrt1 - qrt0), 2);

        qrt2 = sqrt8(db);
        distance2 += pow((qrt2 - qrt0), 2);
    }
    distance1 = sqrt(distance1);
    distance2 = sqrt(distance2);

    printf("Use Euclidean distance to test sqrt function.\n");
    printf("range of value  sqrt        sqrt1       sqrt2\n");
    printf("(0,1.0e5)        %-.5f    %-.5f    %-.5f\n", distance0, distance1, distance2);
    distance0 = distance1 = distance2 = 0.0;
    for (int i = 0; i < int(vecTestValue.size()); ++i) {
        double db = vecTestValue1[i];
        qrt0 = sqrt(db);
        distance0 = 0;

        qrt1 = squareroot(db);
        distance1 += pow((qrt1 - qrt0), 2);

        qrt2 = sqrt8(db);
        distance2 += pow((qrt2 - qrt0), 2);
    }
    distance1 = sqrt(distance1);
    distance2 = sqrt(distance2);
    printf("(1.0e05,1.0e10)  %-.5f    %-.5f    %-.5f\n", distance0, distance1, distance2);
    distance0 = distance1 = distance2 = 0.0;
    for (int i = 0; i < int(vecTestValue.size()); ++i) {
        double db = vecTestValue2[i];
        qrt0 = sqrt(db);
        distance0 = 0;

        qrt1 = squareroot(db);
        distance1 += pow((qrt1 - qrt0), 2);

        qrt2 = sqrt8(db);
        distance2 += pow((qrt2 - qrt0), 2);
    }
    distance1 = sqrt(distance1);
    distance2 = sqrt(distance2);
    printf("(1.0e10,1.0e15)  %-.5f    %-.5f    %-.5f\n", distance0, distance1, distance2);
    distance0 = distance1 = distance2 = 0.0;
    for (int i = 0; i < int(vecTestValue.size()); ++i) {
        double db = vecTestValue3[i];
        qrt0 = sqrt(db);
        distance0 = 0;

        qrt1 = squareroot(db);
        distance1 += pow((qrt1 - qrt0), 2);

        qrt2 = sqrt8(db);
        distance2 += pow((qrt2 - qrt0), 2);
    }
    distance1 = sqrt(distance1);
    distance2 = sqrt(distance2);
    printf("(1.0e15,1.0e20)  %-.5f    %-.5f    %-.5f\n", distance0, distance1, distance2);

    printf("\nUse relative error to test sqrt function.\n");
    distance0 = distance1 = distance2 = 0.0;
    for (int i = 0; i < int(vecTestValue.size()); ++i) {
        double db = vecTestValue[i];
        qrt0 = sqrt(db);
        distance0 = 0;

        qrt1 = squareroot(db);
        distance1 += fabs(qrt1 - qrt0) / qrt0;

        qrt2 = sqrt8(db);
        distance2 += fabs(qrt2 - qrt0) / qrt0;
    }
    distance0 /= int(vecTestValue.size());
    distance1 /= int(vecTestValue.size());
    distance2 /= int(vecTestValue.size());
    printf("sqrt        sqrt1       sqrt2\n");
    printf("%-.5f    %-.5f    %-.5f\n", distance0, distance1, distance2);

    printf("\nTest the cost time of sqrt function.\n");
    printf("The test cases are: %d.\n", testnum);
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < int(vecTestValue.size()); ++i) {
        sqrt(vecTestValue[i]);
    }
    gettimeofday(&end, NULL);
    int micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of math.sqrt: %d.\n", micros);

    gettimeofday(&start, NULL);
    for (int i = 0; i < int(vecTestValue.size()); ++i) {
        squareroot(vecTestValue[i]);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of sqrt1: %d.\n", micros);

    gettimeofday(&start, NULL);
    for (int i = 0; i < int(vecTestValue.size()); ++i) {
        sqrt8(vecTestValue[i]);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("working time of sqrt2: %d.\n", micros);

    vecTestValue.clear();
    vecTestValue1.clear();
    vecTestValue2.clear();
    vecTestValue3.clear();
}
