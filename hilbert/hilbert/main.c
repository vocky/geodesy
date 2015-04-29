//
//  main.c
//  hilbert
//
//  Created by Kotaimen on 7/16/14.
//  Copyright (c) 2014 Kotaimen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hilbert.h"
#include "tileid.h"

static unsigned test_hil_32() {
    unsigned c = 0;
    unsigned z = 11;
    unsigned s, px, py;
    for (unsigned x=0; x < (1<<z); ++x) {
        for (unsigned y=0; y < (1<<z); ++y) {
            s = hil_s_from_xy(x, y, z);
            hil_xy_from_s(s, z, &px, &py);
            if ( px != x || py != y ) {
                printf("~~~~ failed! ~~~~\n");
                return 0;
            }
            ++ c;
        }
    }
    return c;

}

static unsigned test_hil_64_alt() {
    unsigned c = 0;
    unsigned z = 20;
    unsigned long long s, px, py;
    const unsigned stridex = 503; // random prime
    const unsigned stridey = 509;
    for (unsigned x=0; x < (1<<z); x += stridex) {
        for (unsigned y=0; y < (1<<z); y += stridey) {
            s = hil_s_from_xy_64(x, y, z);
            hil_xy_from_s_64(s, z, &px, &py);
            if ( px!=x || py!=y ) {
                printf("~~~~ failed! ~~~~\n");
                return 0;
            }
            ++ c;
        }
    }
    return c;
    
}

static unsigned test_hil_64() {
    unsigned c = 0;
    unsigned z = 11;
    unsigned long long s, px, py;
    for (unsigned x=0; x < (1<<z); ++x) {
        for (unsigned y=0; y < (1<<z); ++y) {
            s = hil_s_from_xy_64(x, y, z);
            hil_xy_from_s_64(s, z, &px, &py);
            if ( px!=x || py!=y ) {
                printf("~~~~ failed! ~~~~\n");
                return 0;
            }
            ++ c;
        }
    }
    return c;
    
}


int test_tile_serial() {
    unsigned c = 0;
    unsigned long long serial;
    unsigned pz, px, py;
    for (unsigned z=0; z < 22; ++z) {
        for (unsigned x=0; x < (1<<z); x += 1319) {
            for (unsigned y=0; y < (1<<z); y += 1421) {
                serial = tile_coordinate_to_serial_64(z, x, y);
                
//                printf("(%3d, %4d, %4d) => 0x%08llx\n", z, x, y, serial);
                
                tile_serial_to_coordinate_64(serial, &pz, &px, &py);
                if ( px!=x || py!=y || pz!=z ) {
                    printf("~~~~ failed! ~~~~\n");
                    return 0;
                }
                
                ++ c;

            }
        }
    }
    
    return c;
    
}


int main() {
    // See profile.md for numbers.
    clock_t tic;
    int i;
    
    for (i=1; i<=3;++i) {
        printf("Take %d\n", i);

        tic = clock();
        printf("1) Testing uint32 hilbert ...");
        printf("ok, %d samples in %.3fs.\n", test_hil_32(),
               1.0 * (clock() - tic) / CLOCKS_PER_SEC);
        
        tic = clock();
        printf("2) Testing uint64 hilbert ...");
        printf("ok, %d samples in %.3fs.\n", test_hil_64(),
               1.0 * (clock() - tic) / CLOCKS_PER_SEC);

        tic = clock();
        printf("3) Testing uint64 hilbert (z=20) ...");
        printf("ok, %d samples in %.3fs.\n", test_hil_64_alt(),
               1.0 * (clock() - tic) / CLOCKS_PER_SEC);

        tic = clock();
        printf("4) Testing tile coordinate to serial...");
        printf("ok, %d samples in %.3fs.\n", test_tile_serial(),
               1.0 * (clock() - tic) / CLOCKS_PER_SEC);

    
    }
    


}

