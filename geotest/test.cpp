/*
 * test.cpp
 *
 *  Created on: July 19, 2014
 *      Author: laye
 */

#include <cstring>
#include <sys/time.h>
#include <map>
#include <hash_map>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
//#include <google/sparse_hash_map>
#include <math.h>
#include "distance.h"
#include <vector>
#include "geoangle.h"
#include "triangle.h"
#include "../geodesy/geodesy/geodesic.h"
using namespace std;


extern int totalnum;
extern int testnum;

/*typedef struct tagLinkKey {
    int num_tileID;
    int num_linkID;
    int num_dir;//forward direction = 0, opposite direction = 1
    tagLinkKey () {
        num_tileID = num_linkID = num_dir = 0;
    }
    bool operator == (const tagLinkKey  & _A) const {
        if (num_tileID == _A.num_tileID && (num_linkID == _A.num_linkID) &&
                (num_dir == _A.num_dir)) return true;
        return false;
    }

    bool operator < (const tagLinkKey  & _A) const {
        if (num_tileID < _A.num_tileID) return true;
        if (num_tileID == _A.num_tileID && (num_linkID < _A.num_linkID)) return true;
        if (num_tileID == _A.num_tileID && (num_linkID == _A.num_linkID) &&
                (num_dir < _A.num_dir)) return true;
        return false;
    }

    tagLinkKey(const tagLinkKey& _A) {
        num_tileID = _A.num_tileID;
        num_linkID = _A.num_linkID;
        num_dir = _A.num_dir;
    }
    tagLinkKey& operator = (const tagLinkKey& _A) {
        if (this == &_A)
            return *this;
        num_tileID = _A.num_tileID;
        num_linkID = _A.num_linkID;
        num_dir = _A.num_dir;
        return *this;
    }
} LinkKey;

struct hash_func {
    size_t operator() (const LinkKey &linkkey) const {
        long tile = linkkey.num_tileID;
        long dir = linkkey.num_dir;
        long linkid = linkkey.num_linkID;
        return ((tile<<32) | (dir<<31) | linkid);
    }
};

struct cmp_fun {
    bool operator() (const LinkKey &linkkey1, const LinkKey &linkkey2) const {
        if (linkkey1.num_tileID == linkkey2.num_tileID && (linkkey1.num_linkID == linkkey2.num_linkID) &&
                (linkkey1.num_dir == linkkey2.num_dir)) return true;
        return false;
    }
};*/

/* Get a random list from [0,n], list size is m
 * */
void GetRandNumber(vector<int> &vec, int m, int n) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < n; ++i) {
        if ((rand() % (n - i)) < m) {
            m--;
            vec.push_back(i);
        }
    }
}

/* Test the efficient of std::map
 * */
/*void Map_Test() {
    std::cout<<"------------------------------------------------"<<std::endl;
    std::cout<<"Test the efficient of std::map."<<std::endl;
    if (testnum > totalnum) {
       std::cout<<"testnum must less than totalnum"<<std::endl;
       return;
    }

    map<LinkKey, long> testmap;
    map<LinkKey, long>::iterator iter;
    LinkKey linkkey;
    for (int i = 0; i < totalnum; i++) {
        linkkey.num_tileID = 1;
        linkkey.num_linkID = 1;
        linkkey.num_dir = i;
        testmap.insert(make_pair(linkkey, i));
    }

    std::cout<<"map size:"<<testmap.size()<<std::endl;

    vector<int> vec;
    GetRandNumber(vec, testnum, totalnum);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i=0; i < int(vec.size()); ++i) {
     linkkey.num_tileID = 1;
     linkkey.num_linkID = 1;
     linkkey.num_dir = vec[i];
     iter = testmap.find(linkkey);
     if (iter == testmap.end())
         std::cout<<"error"<<std::endl;
     iter->second = vec[i];
    }

    gettimeofday(&end, NULL);
    int micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    std::cout<<"map working time: "<<micros<<"ms"<<std::endl;

    testmap.clear();
    vec.clear();
}*/

/* Test the efficient of hash_map
 * */
/*void HashMap_Test() {
    std::cout<<"------------------------------------------------"<<std::endl;
    std::cout<<"Test the efficient of hash_map."<<std::endl;
    if (testnum > totalnum) {
       std::cout<<"testnum must less than totalnum"<<std::endl;
       return;
    }

    __gnu_cxx::hash_map<LinkKey, long, hash_func, cmp_fun> testmap;
    __gnu_cxx::hash_map<LinkKey, long, hash_func, cmp_fun>::iterator iter;
    LinkKey linkkey;
    for (int i = 0; i < totalnum; i++) {
        linkkey.num_tileID = 1;
        linkkey.num_linkID = 1;
        linkkey.num_dir = i;
        testmap.insert(make_pair(linkkey, i));
    }

    std::cout<<"map size:"<<testmap.size()<<std::endl;

    vector<int> vec;
    GetRandNumber(vec, testnum, totalnum);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i=0; i < int(vec.size()); ++i) {
     linkkey.num_tileID = 1;
     linkkey.num_linkID = 1;
     linkkey.num_dir = vec[i];
     iter = testmap.find(linkkey);
     if (iter == testmap.end())
         std::cout<<"error"<<std::endl;
     iter->second = vec[i];
    }

    gettimeofday(&end, NULL);
    int micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    std::cout<<"hash map working time: "<<micros<<"ms"<<std::endl;

    testmap.clear();
    vec.clear();
}*/

/* Test the efficient of sparse_hash_map
 * */
/*void SparseHashMap_Test() {
    std::cout<<"------------------------------------------------"<<std::endl;
    std::cout<<"Test the efficient of sparse_hash_map."<<std::endl;
    if (testnum > totalnum) {
       std::cout<<"testnum must less than totalnum"<<std::endl;
       return;
    }

    google::sparse_hash_map<LinkKey, long, hash_func, cmp_fun> testmap;
    google::sparse_hash_map<LinkKey, long, hash_func, cmp_fun>::iterator iter;
    LinkKey linkkey;
    for (int i = 0; i < totalnum; i++) {
        linkkey.num_tileID = 1;
        linkkey.num_linkID = 1;
        linkkey.num_dir = i;
        testmap.insert(make_pair(linkkey, i));
    }

    std::cout<<"map size:"<<testmap.size()<<std::endl;

    vector<int> vec;
    GetRandNumber(vec, testnum, totalnum);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i=0; i < int(vec.size()); ++i) {
     linkkey.num_tileID = 1;
     linkkey.num_linkID = 1;
     linkkey.num_dir = vec[i];
     iter = testmap.find(linkkey);
     if (iter == testmap.end())
         std::cout<<"error"<<std::endl;
     iter->second = vec[i];
    }

    gettimeofday(&end, NULL);
    int micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    std::cout<<"sparse hash map working time: "<<micros<<"ms"<<std::endl;

    testmap.clear();
    vec.clear();
}*/

/* Test the efficient of math.sin and math.cos
 * */
void SinCosError_Test() {
/*    std::cout<<"------------------------------------------------"<<std::endl;
    std::cout<<"Test the efficient of math.sin and math.cos."<<std::endl;
    double minerror = 1e10, maxerror = -1e10, meanerror = 0.0;
    double stdvalue = 0.0, samplevalue = 0.0, errorvalue = 0.0;

    //sin()
    for (int i = 0; i < testnum; ++i) {
        double testvalue = RandDB(-0.0009, 0.0009);
        stdvalue = sin(testvalue);
        samplevalue = pGisFunction->mysin(testvalue);
        errorvalue = fabs(stdvalue - samplevalue);
        if (errorvalue < minerror) {
            minerror = errorvalue;
        }
        if (errorvalue > maxerror) {
            maxerror = errorvalue;
        }
        if (i > 0)
            meanerror = (errorvalue + meanerror) / 2.0;
        else
            meanerror = errorvalue;
    }
    std::cout<<"sin() minimum deviation:"<<minerror<<std::endl;
    std::cout<<"sin() maxmum deviation:"<<maxerror<<std::endl;
    std::cout<<"sin() mean deviation:"<<meanerror<<std::endl;

    //cos()
    minerror = 1e10, maxerror = -1e10, meanerror = 0.0;
    stdvalue = 0.0, samplevalue = 0.0, errorvalue = 0.0;

    for (int i = 0; i < testnum; ++i) {
        double testvalue = RandDB(-0.0009, 0.0009);
        stdvalue = cos(testvalue);
        samplevalue = mycos(testvalue);
        errorvalue = fabs(stdvalue - samplevalue);
        if (errorvalue < minerror) {
            minerror = errorvalue;
        }
        if (errorvalue > maxerror) {
            maxerror = errorvalue;
        }
        if (i > 0)
            meanerror = (errorvalue + meanerror) / 2.0;
        else
            meanerror = errorvalue;
    }
    std::cout<<"cos() minimum deviation:"<<minerror<<std::endl;
    std::cout<<"cos() maxmum deviation:"<<maxerror<<std::endl;
    std::cout<<"cos() mean deviation:"<<meanerror<<std::endl;
*/
}

/* Test the deviation of length calculation
 * */
void LengthError_Test() {
    std::cout<<"------------------------------------------------"<<std::endl;
    std::cout<<"Test the deviation of length calculation."<<std::endl;
    double minerror = 1e10, maxerror = -1e10, meanerror = 0.0;
    double stdvalue = 0.0, samplevalue = 0.0, errorvalue = 0.0;
    for (int i = 0; i < testnum; ++i) {
        double dx1 = RandDB(138.0, 140.0);
        double dy1 = RandDB(33.0, 35.0);
        double dx2 = RandDB(138.0, 140.0);
        double dy2 = RandDB(33.0, 35.0);
        stdvalue = CalculateLength_Guass(dx1, dy1, dx2, dy2);
        samplevalue = CalculateLength_Guass(dx1, dy1, dx2, dy2);
        errorvalue = fabs(stdvalue - samplevalue);
        if (errorvalue < minerror) {
            minerror = errorvalue;
        }

        if (errorvalue > maxerror) {
            maxerror = errorvalue;
        }
        if (i > 0)
            meanerror = (errorvalue + meanerror) / 2.0;
        else
            meanerror = errorvalue;
    }

    std::cout<<"length calc minimum deviation:"<<minerror<<std::endl;
    std::cout<<"length calc maxmum deviation:"<<maxerror<<std::endl;
    std::cout<<"length calc mean deviation:"<<meanerror<<std::endl;
}

/* Test the cost time of length calculation
 * */
void LengthEfficient_Test() {
    std::cout<<"------------------------------------------------"<<std::endl;
    std::cout<<"Test the cost time of length calculation."<<std::endl;
    std::cout<<"The test cases are: "<<testnum<<std::endl;

    vector<double> vecx1, vecy1, vecx2, vecy2;
    for (int i = 0; i < testnum; ++i) {
        double dx1 = RandDB(135.0, 140.0);
        double dy1 = RandDB(30.0, 35.0);
        double dx2 = RandDB(135.0, 140.0);
        double dy2 = RandDB(30.0, 35.0);
        vecx1.push_back(dx1);
        vecy1.push_back(dy1);
        vecx2.push_back(dx2);
        vecy2.push_back(dy2);
    }

    //CalcLength_old
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        CalculateLength_Guass(vecx1[i], vecy1[i], vecx2[i], vecy2[i]);
    }
    gettimeofday(&end, NULL);
    int micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    std::cout<<"working time of Guass's formula: "<<micros<<" ms"<<std::endl;

    //CalcLength new
    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        CalculateLength_GreatCircle(vecx1[i], vecy1[i], vecx2[i], vecy2[i]);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    std::cout<<"working time of GreatCircle's formula: "<<micros<<" ms"<<std::endl;

    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        CalculateLength_Haversine(vecx1[i], vecy1[i], vecx2[i], vecy2[i]);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    std::cout<<"working time of Haversine's formula: "<<micros<<" ms"<<std::endl;

    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        pbhypotenuse_distance(vecx1[i], vecy1[i], vecx2[i], vecy2[i]);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    std::cout<<"working time of PbHypotenuse's formula: "<<micros<<" ms"<<std::endl;

    gettimeofday(&start, NULL);
    for (int i = 0; i < testnum; ++i) {
        vincenty_distance(vecx1[i], vecy1[i], vecx2[i], vecy2[i]);
    }
    gettimeofday(&end, NULL);
    micros = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    std::cout<<"working time of Vincenty's formula: "<<micros<<" ms\n"<<std::endl;

    vecx1.clear();
    vecy1.clear();
    vecx2.clear();
    vecy2.clear();
}

/* Test the relation about length and deviation of length calculation
 * */
void LengthErrorRange_Test() {
    std::cout<<"------------------------------------------------"<<std::endl;
    std::cout<<"The table below shows distances calculated by each of the methods, "<<std::endl;
    std::cout<<"which are Guass, GreatCirle, Haversine, PbHypotenuse."<<std::endl;
    std::cout<<"Guass reference:http://en.wikipedia.org/wiki/Transverse_Mercator_projection"<<std::endl;
    std::cout<<"GreatCirle reference:http://en.wikipedia.org/wiki/Great_circle"<<std::endl;
    std::cout<<"Haversine reference:http://en.wikipedia.org/wiki/Haversine_formula"<<std::endl;
    std::cout<<"PbHypotenuse reference:http://en.wikipedia.org/wiki/Right_triangle"<<std::endl;
    std::cout<<"Vincenty reference:http://en.wikipedia.org/wiki/Vincenty%27s_formulae"<<std::endl;
    std::cout<<"Then you can get the ""best"" answer by compare to the standard Vincenty's formula."<<std::endl;
    std::cout<<"The result of Vincenty's formula is showed in: "<<std::endl;
    std::cout<<"www.ga.gov.au/geodesy/datums/vincenty_inverse.jsp\n"<<std::endl;
    std::cout<<"The sample points:"<<std::endl;
    std::cout<<"Point    Longitude        Latitude"<<std::endl;
    std::cout<<"A        139.7716903      35.6819149"<<std::endl;
    std::cout<<"B        139.7713094      35.6812936"<<std::endl;
    std::cout<<"C        139.0            35.0"<<std::endl;
    std::cout<<"D        150.0            30.0"<<std::endl;
    std::cout<<"E        150.0            -30.0"<<std::endl;
    std::cout<<"F        120.0            60.0"<<std::endl;
    std::cout<<"G        120.0            -60.0"<<std::endl;
    std::cout<<"H        20.0             -30.0"<<std::endl;
    std::cout<<"I        20.0             -20.0"<<std::endl;
    std::cout<<"J        20.0             -10.0"<<std::endl;
    std::cout<<"K        20.0             0.0"<<std::endl;
    std::cout<<"L        20.0             10.0\n"<<std::endl;
    std::cout<<"The result:"<<std::endl;
    std::cout<<"From    Method1      Method2         Method3      Method4         Method5"<<std::endl;
    std::cout<<"To      Guass        GreatCircle     Haversine    PbHypotenuse    Vincenty"<<std::endl;

    double d1, d2, d3, d4, d5;
    double deviation1, deviation2, deviation3, deviation4;
    deviation1 = deviation2 = deviation3 = deviation4 = 0.0;
    char cTemp[1024];
    vector<Point> vecPoint;
    Point A,B,C,D,E,F,G,H,I,J,K,L;
    A.lon = 139.7716903, A.lat = 35.6819149;
    B.lon = 139.7713094, B.lat = 35.6812936;
    C.lon = 139.0, C.lat = 35.0;
    D.lon = 150.0, D.lat = 30.0;
    E.lon = 150.0, E.lat = -30.0;
    F.lon = 120.0, F.lat = 60.0;
    G.lon = 120.0, G.lat = -60.0;
    H.lon = 20.0, H.lat = -30.0;
    I.lon = 20.0, I.lat = -20.0;
    J.lon = 20.0, J.lat = -10.0;
    K.lon = 20.0, K.lat = 0.0;
    L.lon = 20.0, L.lat = 10.0;
    vecPoint.push_back(A);
    vecPoint.push_back(B);
    vecPoint.push_back(C);
    vecPoint.push_back(D);
    vecPoint.push_back(E);
    vecPoint.push_back(F);
    vecPoint.push_back(G);
    vecPoint.push_back(H);
    vecPoint.push_back(I);
    vecPoint.push_back(J);
    vecPoint.push_back(K);
    vecPoint.push_back(L);

    for (int i = 0; i < int(vecPoint.size()); ++i) {
        if (i == 0) {
            d1 = spheroid_distance(vecPoint[i].lon, vecPoint[i].lat, vecPoint[i].lon, vecPoint[i].lat);
            d2 = great_circle_distance(vecPoint[i].lon, vecPoint[i].lat, vecPoint[i].lon, vecPoint[i].lat);
            d3 = haversine_distance(vecPoint[i].lon, vecPoint[i].lat, vecPoint[i].lon, vecPoint[i].lat);
            d4 = pythagoras_distance(vecPoint[i].lon, vecPoint[i].lat, vecPoint[i].lon, vecPoint[i].lat);
            d5 = vincenty_distance(vecPoint[i].lon, vecPoint[i].lat, vecPoint[i].lon, vecPoint[i].lat);
        }else {
            d1 = spheroid_distance(vecPoint[0].lon, vecPoint[0].lat, vecPoint[i].lon, vecPoint[i].lat);
            d2 = great_circle_distance(vecPoint[0].lon, vecPoint[0].lat, vecPoint[i].lon, vecPoint[i].lat);
            d3 = haversine_distance(vecPoint[0].lon, vecPoint[0].lat, vecPoint[i].lon, vecPoint[i].lat);
            d4 = pythagoras_distance(vecPoint[0].lon, vecPoint[0].lat, vecPoint[i].lon, vecPoint[i].lat);
            d5 = vincenty_distance(vecPoint[0].lon, vecPoint[0].lat, vecPoint[i].lon, vecPoint[i].lat);
        }
        deviation1 += pow((d1 - d5), 2);
        deviation2 += pow((d2 - d5), 2);
        deviation3 += pow((d3 - d5), 2);
        deviation4 += pow((d4 - d5), 2);

        sprintf(cTemp, "%c%c      %-11.2f  %-11.2f     %-11.2f  %-11.2f     %-11.2f", 65, i+65, d1, d2, d3, d4, d5);
        std::cout<<cTemp<<std::endl;
    }
    deviation1 = sqrt(deviation1);
    deviation2 = sqrt(deviation2);
    deviation3 = sqrt(deviation3);
    deviation4 = sqrt(deviation4);
    std::cout<<"Accumulative error:"<<std::endl;
    sprintf(cTemp, "Error   %-11.2f  %-11.2f     %-11.2f  %-11.2f     0.0\n", deviation1, deviation2, deviation3, deviation4);
    std::cout<<cTemp<<std::endl;
}

typedef struct {
    int nAngle1;
    int nAngle2;
    int nResult;
}TestIncludeAngle;

const int test_count = 3;
TestIncludeAngle tests[test_count] = {
        {0,45,45},
        {-1,-1,0},
        {0,270,90},
};

typedef struct {
    double lon1;
    double lat1;
    double lon2;
    double lat2;
    int nResult;
}TestVectorAngle;
const int test_num = 7;
TestVectorAngle testangles[test_num] = {
        {0.0,0.0,0.0,0.0,-1},
        {0.0,2.0,0.0,5.0,0},
        {0.0,2.0,2.0,2.0,90},
        {0.0,2.0,0.0,-2.0,180},
        {0.0,0.0,-2.0,0.0,270},
        {0.0,0.0,1.414,-1.414,135},
        {0.0,0.0,-1.414,1.414,315},
};

void GeoAngle_Test() {
    std::cout<<"------------------------------------------------"<<std::endl;
    printf("Test Included Angle.\n");
    for (int i = 0; i < test_count; ++i) {
        TestIncludeAngle test = tests[i];
        printf("Test case: %d, ", i);
        printf("Angle1: %d, Angle2: %d, ", test.nAngle1, test.nAngle2);
        printf("Expected: %d.\n", test.nResult);
        printf("IncludeAngle: %d\n", CalculateIncludeAngle(test.nAngle1, test.nAngle2));
    }

    printf("Test the clockwise angle of the vector.\n");
    for (int i = 0; i < test_num; i++) {
        TestVectorAngle test = testangles[i];
        printf("Test case: %d, ", i);
        printf("PointA: %.3f,%.3f, PointB: %.3f,%.3f, ", test.lon1, test.lat1, test.lon2, test.lat2);
        printf("Expected: %d.\n", test.nResult);
        printf("Clockwise angle: %d\n", CalculateAngle(test.lon1, test.lat1, test.lon2, test.lat2));
    }

    printf("Calculate the perpendicular of the segment.\n");
    LonLat vertex1, vertex2, gps, matchout;
    vertex1.lon = 0.0f;
    vertex1.lat = 0.0f;
    vertex2.lon = 2.0f;
    vertex2.lat = 0.0f;
    gps.lon = 1.5f;
    gps.lat = 2.0f;
    printf("Test case: 1, ");
    printf("Vertex1: %.3f,%.3f, Vertex2: %.3f,%.3f, GPSPoint:%.3f, %.3f, ",
            vertex1.lon, vertex1.lat, vertex2.lon, vertex2.lat, gps.lon, gps.lat);
    printf("Expected: 1.5,0.0.\n");
    CalculatePerpendicular(vertex1, vertex2, gps, matchout);
    printf("The perpendicular is: %.3f, %.3f\n", matchout.lon, matchout.lat);

    printf("Test the relation of point and link.\n");
    vertex1.lon = 0.0f;
    vertex1.lat = 0.0f;
    vertex2.lon = 2.0f;
    vertex2.lat = 0.0f;
    gps.lon = 1.5f;
    gps.lat = 0.0f;
    printf("Test case: 1, ");
    printf("Vertex1: %.3f,%.3f, Vertex2: %.3f,%.3f, GPSPoint:%.3f, %.3f, ",
            vertex1.lon, vertex1.lat, vertex2.lon, vertex2.lat, gps.lon, gps.lat);
    printf("Expected: 0\n");
    printf("The result is: %d\n", linePointPosition2D(vertex1, vertex2, gps));
    gps.lon = -1.5f;
    gps.lat = 1.0f;
    printf("Test case: 2, ");
    printf("Vertex1: %.3f,%.3f, Vertex2: %.3f,%.3f, GPSPoint:%.3f, %.3f, ",
            vertex1.lon, vertex1.lat, vertex2.lon, vertex2.lat, gps.lon, gps.lat);
    printf("Expected: 2\n");
    printf("The result is: %d\n", linePointPosition2D(vertex1, vertex2, gps));
    gps.lon = 3.0f;
    gps.lat = -1.0f;
    printf("Test case: 3, ");
    printf("Vertex1: %.3f,%.3f, Vertex2: %.3f,%.3f, GPSPoint:%.3f, %.3f, ",
            vertex1.lon, vertex1.lat, vertex2.lon, vertex2.lat, gps.lon, gps.lat);
    printf("Expected: 1\n");
    printf("The result is: %d\n", linePointPosition2D(vertex1, vertex2, gps));
}

int main(int argc, const char* argv[]) {
    //SinCosError_Test();
    //LengthError_Test();
    //LengthEfficient_Test();
    //LengthErrorRange_Test();
    //GeoAngle_Test();
    SquareRoot_Test();
    //Map_Test();
    //HashMap_Test();
    //SparseHashMap_Test();
    return 0;
}
