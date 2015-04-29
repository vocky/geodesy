//
//  tileid.h
//
//  Created by Kotaimen on 7/18/14.
//  Copyright (c) 2014 Kotaimen. All rights reserved.
//

#pragma once

/**
 * Convert a tile coordinate to serial using hilbert curve
 * z: [0, 15]
 * x,y: [0,2^16-1]
 * serial: 32bit integer
 */
unsigned long long tile_coordinate_to_serial_64(unsigned z, unsigned x, unsigned y);

/**
 * Convert a tile serial back to tile coordinate
 */
void tile_serial_to_coordinate_64(unsigned long long serial, unsigned *pz, unsigned *px, unsigned *py);

