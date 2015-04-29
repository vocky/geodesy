#include "morton.h"


unsigned long long morton_encode(unsigned x, unsigned y) {
  static const unsigned long long B[] = {
    0x5555555555555555ull, 0x3333333333333333ull, 0x0f0f0f0f0f0f0f0full,
    0x00ff00ff00ff00ffull, 0x0000ffff0000ffffull};
  static const unsigned S[] = {1, 2, 4, 8, 16};
  unsigned long long ullx = x;
  unsigned long long ully = y;
  int i;
  for(i = 4; i >= 0; --i) {
    ullx = (ullx | (ullx << S[i])) & B[i];
    ully = (ully | (ully << S[i])) & B[i];
  }
  return ullx | (ully << 1);
}


void morton_decode(unsigned long long s, unsigned *xp, unsigned *yp) {
static const unsigned long long B[] = {
  0x3333333333333333ull, 0x0f0f0f0f0f0f0f0full,
  0x00ff00ff00ff00ffull, 0x0000ffff0000ffffull, 0x00000000ffffffffull};
static const unsigned S[] = {1, 2, 4, 8, 16};
  unsigned long long ullx = s & 0x5555555555555555ull;
  s >>= 1;
  unsigned long long ully = s & 0x5555555555555555ull;
  int i;
  for(i = 0; i < 5; ++i) {
    ullx = (ullx | (ullx >> S[i])) & B[i];
    ully = (ully | (ully >> S[i])) & B[i];
  }

  *xp = (unsigned)ullx;
  *yp = (unsigned)ully;
}

