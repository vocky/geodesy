Distance's Formula Performance
============================
Computer configuration:
----------------------
+ Processor: Intel core i5-2400 CPU @ 3.1GHz
+ Memory: 16GB
+ OS type: 64-bit

Test the cost time of six distance's formula:
----------------------------------------
```
Test the cost time of length calculation.
The test cases are: 50000.
working time of Spheroid's formula: 12788 ms
working time of GreatCircle's formula: 10615 ms
working time of Haversine's formula: 10073 ms
working time of Pythagoras's formula: 3454 ms
working time of Pbhypotenuse's formula: 7929 ms
working time of Vincenty's formula: 147550 ms
```

Test the error of six distance's formula:
-----------------------------------------
```
The table below shows distances calculated by each of the methods,
which are Spheroid, GreatCirle, Haversine, pythagoras, PbHypotenuse, Vincenty.
Spheroid, The special case of Vincenty formula (spheroid)
GreatCirle reference:http://en.wikipedia.org/wiki/Great_circle
Haversine reference:http://en.wikipedia.org/wiki/Haversine_formula
Pythagoras reference:http://en.wikipedia.org/wiki/Geographical_distance#Spherical_Earth_projected_to_a_plane
PbHypotenuse reference:http://en.wikipedia.org/wiki/Right_triangle
Vincenty reference:http://en.wikipedia.org/wiki/Vincenty's_formulae
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
From    Method1      Method2         Method3      Method4         Method5         Method6
To      Spheroid     GreatCircle     Haversine    Pythagoras      pbhypotenuse    Vincenty
AA      0.00         0.00            0.00         0.00            0.00            0.00       
AB      77.18        77.18           77.18        77.18           77.08           77.08      
AC      103192.84    103192.84       103192.84    103193.66       102978.10       103175.47  
AD      1144500.68   1144500.68      1144500.68   1145548.60      1120155.75      1145266.83 
AE      7381449.30   7381449.30      7381449.30   7391316.25      7346197.50      7348821.18 
AF      3053444.96   3053444.96      3053444.96   3080480.02      3237833.25      3055168.47 
AG      10792847.28  10792847.28     10792847.28  10854260.92     10766025.00     10758780.44
AH      14439962.81  14439962.81     14439962.81  15174821.37     13063837.00     14438799.98
AI      13937752.49  13937752.49     13937752.49  14574085.68     12478911.00     13940240.47
AJ      13332280.71  13332280.71     13332280.71  13943144.21     11968490.00     13338639.91
AK      12652484.07  12652484.07     12652484.07  13283937.12     11542460.00     12662932.41
AL      11921484.55  11921484.55     11921484.55  12601539.45     11210447.00     11935964.59
Accumulative error:
Error   50946.27     50946.27        50946.27     1465985.17      7719208403057.89   0.0
```
