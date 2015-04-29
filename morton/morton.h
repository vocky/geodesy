#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

/* XXX: only works in x64 system */

unsigned long long morton_encode(unsigned x, unsigned y);

void morton_decode(unsigned long long s, unsigned *xp, unsigned *yp);

#if defined(__cplusplus)
}
#endif

