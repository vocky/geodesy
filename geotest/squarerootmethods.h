#ifndef SQUAREROOTMETHODS_H
#define SQUAREROOTMETHODS_H
#define SQRT_MAGIC_F 0x5f3759df

#include <math.h>

#include "define.h"
#include <stdlib.h>
//http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
//For sqrt1,sqrt2,and sqrt3 all Credit goes to http://ilab.usc.edu/wiki/index.php/Fast_Square_Root
//For sqrt5 all credit goes to Sanchit Karve(A.K.A born2c0de), He can be contacted at born2c0de@hotmail.com
//Functions sqrt9,sqrt12 and sqrt11 only compute integer inputs 
//enjoy :)


float sine(float x)
{
    if (x < -3.14159265)
        x += 6.28318531;
    else
    if (x >  3.14159265)
        x -= 6.28318531;

    float result = 0.0;
    //compute sine
    if (x < 0)
    {
        result = 1.27323954 * x + .405284735 * x * x;
        
        if (result < 0)
            result = .225 * (result *-result - result) + result;
        else
            result = .225 * (result * result - result) + result;
    }
    else
    {
        result = 1.27323954 * x - 0.405284735 * x * x;
        
        if (result < 0)
            result = .225 * (result *-result - result) + result;
        else
            result = .225 * (result * result - result) + result;
    }
    return result;
}

float cose(float x)
{
    x += 1.57079632;
    if (x >  3.14159265)
        x -= 6.28318531;

    float result = 0.0;
    if (x < 0)
    {
        result = 1.27323954 * x + 0.405284735 * x * x;

        if (result < 0)
            result = .225 * (result *-result - result) + result;
        else
            result = .225 * (result * result - result) + result;
    }
    else
    {
        result = 1.27323954 * x - 0.405284735 * x * x;

        if (result < 0)
            result = .225 * (result *-result - result) + result;
        else
            result = .225 * (result * result - result) + result;
    }
    return result;
}

float squareroot(const float x)  
{
  union
  {
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

//-------------------------------------------------

 float  sqrt2(const float x)
{
  const float xhalf = 0.5f*x;
 
  union // get bits for floating value
  {
    float x;
    int i;
  } u;
  u.x = x;
  u.i = SQRT_MAGIC_F - (u.i >> 1);  // gives initial guess y0
  return x*u.x*(1.5f - xhalf*u.x*u.x);// Newton step, repeating increases accuracy

}

//-------------------------------------------------

float sqrt3(const float x)  
{
  union
  {
    int i;
    float x;
  } u;

  u.x = x;
  u.i = (1<<29) + (u.i >> 1) - (1<<22); 
  return u.x;
}

//-------------------------------------------------

//-------------------------------------------------
	
   float sqrt5(const float m)
{
   float i=0;
   float x1,x2;
   while( (i*i) <= m )
          i+=0.1f;
   x1=i;
   for (int j = 0; j < 10; j++)
   {
       x2=m;
      x2/=x1;
      x2+=x1;
      x2/=2;
      x1=x2;
   }
   return x2;
}
 
//-------------------------------------------------
   
//http://www.azillionmonkeys.com/qed/sqroot.html#calcmeth
  double sqrt6 (double y) {
    double x, z, tempf;
    unsigned long *tfptr = ((unsigned long *)&tempf) + 1;

	tempf = y;
	*tfptr = (0xbfcdd90a - *tfptr)>>1; 
	x =  tempf;
	z =  y*0.5;                       
	x = (1.5*x) - (x*x)*(x*z);      
	x = (1.5*x) - (x*x)*(x*z);      

	return x*y;
    
  }

//-------------------------------------------------

//http://forums.techarena.in/software-development/1290144.htm
 double sqrt8( const double fg)
{

	double n = fg / 2.0;
	double lstX = 0.0;

	while(n != lstX) 

	{

	lstX = n;
	n = (n + fg/n) / 2.0;

	}
 return n;
 }

//-------------------------------------------------

//http://www.functionx.com/cpp/examples/squareroot.htm
double Abs(double Nbr)
{	
	if( Nbr >= 0 )	
		return Nbr;	
	else
		return -Nbr;
}

double sqrt9(double Nbr)
{
	double Number = Nbr / 2;	
	const double Tolerance = 1.0e-7;	
	do
	{
		Number = (Number + Nbr / Number) / 2;
	}while( Abs(Number * Number - Nbr) > Tolerance);
	
	return Number;
}

//-------------------------------------------------

//http://www.cs.uni.edu/~jacobson/C++/newton.html
double sqrt10(const double number)
{
const double ACCURACY=0.001;
double lower, upper, guess;

	if (number < 1)
	{
		lower = number;
		upper = 1;
	}
	else
	{
		lower = 1;
		upper = number;
	}

	while ((upper-lower) > ACCURACY)
	{
		guess = (lower + upper)/2;
		if(guess*guess > number)
			upper =guess;
		else
			lower = guess;	
	}
	return (lower + upper)/2;

}

#endif
