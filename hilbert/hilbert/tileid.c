//
//  tileid.c
//  hilbert
//
//  Created by Kotaimen on 7/18/14.
//  Copyright (c) 2014 Kotaimen. All rights reserved.
//


#include "tileid.h"
#include "hilbert.h"

unsigned tile_coordinate_to_serial(unsigned z, unsigned x, unsigned y) {
    unsigned s = hil_s_from_xy(x, y, z);
    unsigned serial = (z << 28) | s;
    return serial;
}


unsigned long long tile_coordinate_to_serial_64(unsigned z, unsigned x, unsigned y) {
    unsigned long long s = hil_s_from_xy_64(x, y, z);
    unsigned long long serial =  ( (unsigned long long)(z) << 58 ) | s;
    return serial;
}


void tile_serial_to_coordinate_64(unsigned long long serial, unsigned *pz, unsigned *px, unsigned *py) {

    unsigned n = (serial & 0x7c00000000000000ULL) >> 58;
    unsigned long long s = serial & 0x3ffffffffffffffULL;
    unsigned long long xp, yp;
    hil_xy_from_s_64(s, n, &xp, &yp);
    
    *pz = n;
    *px = (unsigned) xp;
    *py = (unsigned) yp;
}
