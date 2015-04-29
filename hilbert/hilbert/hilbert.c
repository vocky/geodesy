//
//  hilbert.c
//
//  Created by Kotaimen on 7/16/14.
//  Copyright (c) 2014 Kotaimen. All rights reserved.
//

#include "hilbert.h"
#include "assert.h"

/**
 * Hilbert curve algorithms are taken from (and modified):
 *   Henry S. Warren, Jr., Hakers's Delight Second Edition, 2012.
 *   Chapter 16: Hilbert's Curve
 */


/*
 Given the "order" n of a Hilbert curve and coordinates x and y, this
 program computes the length s of the curve from the origin to (x, y).
 The square that the Hilbert curve traverses is of size 2**n by 2**n.
 The method is that given in [Lam&Shap], described by the following
 table.  Here i = n-1 for the most significant bit of x and y, and i = 0
 for the least significant bits.
 
 x[i]  y[i] | s[2i+1:2i]   x   y
 -----------|-------------------
  0     0   |     00       y   x
  0     1   |     01       x   y
  1     0   |     11      ~y  ~x
  1     1   |     10       x   y
 
 To use this table, start at the most significant bits of x and y
 (i = n - 1).  If they are both 0 (first row), set the most significant
 two bits of s to 00 and interchange x and y.  (Actually, it is only
 necessary to interchange the remaining bits of x and y.)  If the most
 significant bits of x and y are 10 (third row), output 11, interchange x
 and y, and complement x and y.
 Then, consider the next most significant bits of x and y (which may
 have been changed by this process), and select the appropriate row of
 the table to determine the next two bits of s, and how to change x and
 y.  Continue until the least significant bits of x and y have been
 processed. 
 */

unsigned hil_s_from_xy(unsigned x, unsigned y, int n) {
    
    int i, xi, yi;
    unsigned s;
    
    s = 0;                         // Initialize.
    for (i = n - 1; i >= 0; i--) {
        xi = (x >> i) & 1;          // Get bit i of x.
        yi = (y >> i) & 1;          // Get bit i of y.
        s = 4*s + 2*xi + (xi^yi);   // Append two bits to s.
        
        x = x^y;                    // These 3 lines swap
        y = y^(x & (yi - 1));       // x and y if yi = 0.
        x = x^y;
        x = x^(-xi & (yi - 1));     // Complement x and y if
        y = y^(-xi & (yi - 1));     // xi = 1 and yi = 0.
    }
    return s;
}

unsigned long long hil_s_from_xy_64(unsigned long long x, unsigned long long y, int n) {
    
    int i, xi, yi;
    unsigned long long s;
    
    s = 0;
    for (i = n - 1; i >= 0; i--) {
        xi = (x >> i) & 1;          // Get bit i of x.
        yi = (y >> i) & 1;          // Get bit i of y.
        s = 4*s + 2*xi + (xi^yi);   // Append two bits to s.
        
        x = x^y;                    // These 3 lines swap
        y = y^(x & (yi - 1));       // x and y if yi = 0.
        x = x^y;
        
        x = x^(-xi & (yi - 1));     // Complement x and y if
        y = y^(-xi & (yi - 1));     // xi = 1 and yi = 0.
    }
    return s;
}

/*
 Given the "order" n of a Hilbert curve and a distance s along the curve,
 this program computes the corresponding (x, y) coordinates.  The square
 that the Hilbert curve traverses is of size 2**n by 2**n.
 The method is that given in [Lam&Shap], described by the following
 table.  Here i = n-1 for the most significant bit of x and y, and i = 0
 for the least significant bits.
 
 s[2i+1:2i]   x[i]  y[i]  x[i-1:0]  y[i-1:0]
 -----------|-------------------------------
      00    |  0     0    y[i-1:0]  x[i-1:0]
      01    |  0     1    x[i-1:0]  y[i-1:0]
      10    |  1     1    x[i-1:0]  y[i-1:0]
      11    |  1     0   ~y[i-1:0] ~x[i-1:0]
 
 To use this table, start at the least significant two bits of s (i = 0).
 If they are both 0 (first row), set the least significant bits of x and
 y to 0 and 0 respectively, and interchange x and y.  The last two
 columns designate an interchange of the bits of x and y to the right of
 bit i, but on this first iteration they are null, so there is no
 interchange to do.  If the least significant two bits of s are 10 (third
 row), set the least significant bits of x and y to 1, and similarly for
 the other rows.
 Then, consider the next least significant two bits of s, and select
 the appropriate row of the table to determine the next bits of x and y,
 and how to change the bits of x and y to the right of i.  Continue until
 the most significant bits of x and y have been processed. */


void hil_xy_from_s(unsigned s, int n, unsigned *xp,
                   unsigned *yp) {
    
    int i, sa, sb;
    unsigned x, y, swap, cmpl;
    
    x = 0;
    y = 0;
    
    for (i = 0; i < 2*n; i += 2) {
        sa = (s >> (i+1)) & 1;      // Get bit i+1 of s.
        sb = (s >> i) & 1;          // Get bit i of s.
        
        swap = (sa ^ sb) - 1;  // -1 if should swap, else 0.
        cmpl = -(sa & sb);     // -1 if should compl't, else 0.
        x = x ^ y;
        y = y ^ (x & swap) ^ cmpl;
        x = x ^ y;
        
        x = (x >> 1) | (sa << 31);  // Prepend sa to x and
        y = (y >> 1) | ((sa ^ sb) << 31); // (sa^sb) to y.
    }
    *xp = x >> (32 - n);           // Right-adjust x and y
    *yp = y >> (32 - n);           // and return them to
                                   // the caller.
}


void hil_xy_from_s_64(unsigned long long s, int n,
                   unsigned long long *xp, unsigned long long *yp) {
    
    int i;
    unsigned long long sa, sb;
    unsigned long long x, y, swap, cmpl;
   
    x = 0;
    y = 0;
    
    for (i = 0; i < 2*n; i += 2) {
        sa = (s >> (i+1)) & 1;      // Get bit i+1 of s.
        sb = (s >> i) & 1;          // Get bit i of s.
        
        swap = (sa ^ sb) - 1;  // -1 if should swap, else 0.
        cmpl = -(sa & sb);     // -1 if should compl't, else 0.
        x = x ^ y;
        y = y ^ (x & swap) ^ cmpl;
        x = x ^ y;
        
        x = (x >> 1) | (sa << 63);  // Prepend sa to x and
        y = (y >> 1) | ((sa ^ sb) << 63); // (sa^sb) to y.
    }
    *xp = x >> (64 - n);           // Right-adjust x and y
    *yp = y >> (64 - n);           // and return them to
                                   // the caller.
}

