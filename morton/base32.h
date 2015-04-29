#pragma once

#include <ctype.h>
#include <string.h>

static const int BASE32_BITS = 5;
static const int BASE32_MASK = (1 << 5) - 1;
static const int BASE32_CHAR_MIN = 48;
static const int BASE32_CHAR_MAX = 91;

static const char BASE32_ENCARR[] = "0123456789BCDEFGHJKMNPQRSTUVWXYZ";
static const char BASE32_DECARR[] = {
    // numbers * 10
    /*0*/  0, /*1*/  1, /*2*/  2, /*3*/  3, /*4*/  4, /*5*/  5, /*6*/  6,
    /*7*/  7, /*8*/  8, /*9*/  9,

    // operators * 7
    /*:*/ -1, /*;*/ -1, /*<*/ -1, /*=*/ -1, /*>*/ -1, /*?*/ -1, /*@*/ -1,

    // upper alphabet * 26
    /*A*/ -1, /*B*/ 10, /*C*/ 11, /*D*/ 12, /*E*/ 13, /*F*/ 14, /*G*/ 15,
    /*H*/ 16, /*I*/ -1, /*J*/ 17, /*K*/ 18, /*L*/ -1, /*M*/ 19, /*N*/ 20,
    /*O*/ -1, /*P*/ 21, /*Q*/ 22, /*R*/ 23, /*S*/ 24, /*T*/ 25, /*U*/ 26,
    /*V*/ 27, /*W*/ 28, /*X*/ 29, /*Y*/ 30, /*Z*/ 31
};


int base32_decode(const char* str /* 0-term */, unsigned long long * result) {
    *result = 0;
    int i;
    for(i = 0; str[i]; ++i) {
        if (i > ((sizeof(unsigned long long) * 8) / 5)) {
            return -1;  /* overflow */
        }
        int ord = toupper(str[i]) - BASE32_CHAR_MIN;
        if ((ord < 0) || (ord >= (BASE32_CHAR_MAX - BASE32_CHAR_MIN))) {
            return -2; /* invalid char */
        }
        int bits = BASE32_DECARR[ord];
        if (bits < 0) {
            return -3; /* invalid char */
        }
        (*result) <<= BASE32_BITS;
        (*result) |= bits;
    }
    return 0;
}


int base32_encode(unsigned long long src, unsigned chars, char * result) {
    /* result is pre-allocated by user for atleast chars in length */
    int i;
    for(i = chars; i >= 0; --i) {
        unsigned bits = src & BASE32_MASK;
        src >>= BASE32_BITS;
        result[i] = BASE32_ENCARR[bits];
    }
    if (src) {
        return -1; /* src is overflow */
    }
    return 0;
}

