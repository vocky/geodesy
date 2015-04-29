//
//  hilbert.h
//
//  Created by Kotaimen on 7/16/14.
//  Copyright (c) 2014 Kotaimen. All rights reserved.
//

#pragma once

/**
 * Convert a coordiante pair (x, y) to order n hilbert curve distance s
 */
unsigned hil_s_from_xy(unsigned x, unsigned y, int n);

/**
 * Given hilvert curve distance s and order n, compute coorsponding coordiante xp, yp
 */
void hil_xy_from_s(unsigned s, int n, unsigned *xp,
                   unsigned *yp);


/**
 * Convert a coordiante pair (x, y) to order n hilbert curve distance s
 */
unsigned long long hil_s_from_xy_64(unsigned long long x, unsigned long long y, int n);

/**
 * Given hilvert curve distance s and order n, compute coorsponding coordiante xp, yp
 */
void hil_xy_from_s_64(unsigned long long s, int n, unsigned long long *xp, unsigned long long *yp);



