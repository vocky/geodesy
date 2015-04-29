Distance's Formula Performance
============================
Computer configuration:
----------------------
+ Processor: Intel core i5-2400 CPU @ 3.1GHz
+ Memory: 16GB
+ OS type: 64-bit

Test the cost time of five distance's formula:
----------------------------------------
```
Test the cost time of length calculation.
The test cases are: 500000
working time of Guass's formula: 227228 ms
working time of GreatCircle's formula: 132344 ms
working time of Haversine's formula: 74179 ms
working time of PbHypotenuse's formula: 80108 ms
working time of Vincenty's formula: 962341 ms
```

Test the error of five distance's formula:
-----------------------------------------
```
The table below shows distances calculated by each of the methods, 
which are Guass, GreatCirle, Haversine, PbHypotenuse.
Guass reference:http://en.wikipedia.org/wiki/Transverse_Mercator_projection
GreatCirle reference:http://en.wikipedia.org/wiki/Great_circle
Haversine reference:http://en.wikipedia.org/wiki/Haversine_formula
PbHypotenuse reference:http://en.wikipedia.org/wiki/Right_triangle
Vincenty reference:http://en.wikipedia.org/wiki/Vincenty%27s_formulae
Then you can get the best answer by compare to the standard Vincenty's formula.
The result of Vincenty's formula is showed in: 
www.ga.gov.au/geodesy/datums/vincenty_inverse.jsp

The sample points:
Point    Longitude        Latitude
A        139.7716903      35.6819149
B        139.7713094      35.6812936
C        139.0            35.0
D        150.0            30.0
E        150.0            -30.0
F        120.0            60.0
G        120.0            -60.0
H        20.0             -30.0
I        20.0             -20.0
J        20.0             -10.0
K        20.0             0.0
L        20.0             10.0

The result:
From    Method1      Method2         Method3      Method4         Method5
To      Guass        GreatCircle     Haversine    PbHypotenuse    Vincenty
AA      0.00         0.00            0.00         0.00            0.00       
AB      77.09        77.13           77.26        77.08           77.08      
AC      103204.85    103123.16       103308.30    102978.10       103175.15  
AD      1148442.60   1143727.91      1145781.21   1120155.75      1145259.45 
AE      7371153.97   7376465.29      7389708.07   7346197.50      7348883.95 
AF      3074018.49   3051383.25      3056861.32   3237833.25      3055158.79 
AG      10842895.17  10785559.87     10804922.90  10766025.00     10758841.72
AH      26187801.23  14430212.84     14456119.02  13063837.00     14438715.34
AI      30142727.89  13928341.62     13953346.80  12478911.00     13940147.93
AJ      33822066.08  13323278.65     13347197.58  11968490.00     13338544.42
AK      34826981.31  12643941.02     12666640.36  11542460.00     12662836.56
AL      31918452.20  11913435.08     11934822.95  11210447.00     11935869.81
Accumulative error:
Error   90710310.69  -27964.36       131275.59    -5890098.51     0.0
```
Test the accuracy of the angle's formula:
--------------------------------------
```
Test Included Angle.
Test case: 0, Angle1: 0, Angle2: 45, Expected: 45.
IncludeAngle: 45
Test case: 1, Angle1: -1, Angle2: -1, Expected: 0.
IncludeAngle: 0
Test case: 2, Angle1: 0, Angle2: 270, Expected: 90.
IncludeAngle: 90
```
```
Test the clockwise angle of the vector.
Test case: 0, PointA: 0.000,0.000, PointB: 0.000,0.000, Expected: -1.
Clockwise angle: -1
Test case: 1, PointA: 0.000,2.000, PointB: 0.000,5.000, Expected: 0.
Clockwise angle: 0
Test case: 2, PointA: 0.000,2.000, PointB: 2.000,2.000, Expected: 90.
Clockwise angle: 90
Test case: 3, PointA: 0.000,2.000, PointB: 0.000,-2.000, Expected: 180.
Clockwise angle: 180
Test case: 4, PointA: 0.000,0.000, PointB: -2.000,0.000, Expected: 270.
Clockwise angle: 270
Test case: 5, PointA: 0.000,0.000, PointB: 1.414,-1.414, Expected: 135.
Clockwise angle: 135
Test case: 6, PointA: 0.000,0.000, PointB: -1.414,1.414, Expected: 315.
Clockwise angle: 315
```
```
Calculate the perpendicular of the segment.
Test case: 1, Vertex1: 0.000,0.000, Vertex2: 2.000,0.000, GPSPoint:1.500, 2.000, Expected: 1.5,0.0.
The perpendicular is: 1.500, 0.000
```
```
Test the relation of point and link.
Test case: 1, Vertex1: 0.000,0.000, Vertex2: 2.000,0.000, GPSPoint:1.500, 0.000, Expected: 0
The result is: 0
Test case: 2, Vertex1: 0.000,0.000, Vertex2: 2.000,0.000, GPSPoint:-1.500, 1.000, Expected: 2
The result is: 2
Test case: 3, Vertex1: 0.000,0.000, Vertex2: 2.000,0.000, GPSPoint:3.000, -1.000, Expected: 1
The result is: 1
```