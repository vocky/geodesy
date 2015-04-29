/* tests */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "morton.h"
#include "base32.h"

int tests_run = 0;
char outstr[256];
#define _mu_test_name(test) #test
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do {\
                            clock_t tic = clock();\
                            printf("Testing " _mu_test_name(test) " ...");\
                            char *message = test(); tests_run++;\
                            if (message) printf(message);\
                            else printf("OK, clocks: %f\n", 1.0 * (clock() - tic) / CLOCKS_PER_SEC);\
                            } while (0)

char* test_morton1() {
  unsigned x = 104395303;
  unsigned y = 122949829;
  const unsigned stridex = 503;
  const unsigned stridey = 509;
  unsigned loop = 10000000;
  unsigned long long s;
  unsigned rx, ry;
  char outstr[256];
  for (; loop > 0; --loop) {
    s = morton_encode(x, y);
    morton_decode(s, &rx, &ry);
    mu_assert("result unequal\n", ((x == rx) && (y == ry)));
    x += stridex;
    y += stridey;
  }
  return 0;
}

char* test_morton2() {
  unsigned long long s = 122949829ULL;
  const unsigned strides = 104395303ULL;
  unsigned loop = 10000000;
  unsigned x, y;
  unsigned long long rs;


  for (; loop > 0; --loop) {
    morton_decode(s, &x, &y);
    rs = morton_encode(x, y);
    if (s != rs) {
      sprintf(outstr, "result unequal: %llx, %llx\n", s, rs);
      return outstr;
    }
    s += strides;
  }
  return 0;
}

char* test_base32() {
    unsigned long long content;
    char result[64];
    memset(result, 0, 64);
    strcpy((char*)(&content), "abcdefg");
    printf("content is: 0x%llx\n", content);
    base32_encode(content, 10, result);
    printf("result is: %s\n", result);
    unsigned long long dec_result;
    base32_decode(result, &dec_result);
    printf("dec_result is: 0x%llx\n", dec_result);
    return 0;
}


int main() {
  mu_run_test(test_morton1);
  mu_run_test(test_morton2);
  mu_run_test(test_base32);
}

