#include "base64.h"

int countchr(char* str, char chr) {
    int i;
    for (i = 0; *str != '\0'; *str++ == chr ? i++ : 0);
    return i;
}

size_t b64enclen(size_t datalen) {
    if (datalen % 3 == 0) return datalen / 3 * 4;
    else return (datalen + 3 - datalen % 3) / 3 * 4;
}

size_t b64declen(char* data) {
    size_t datalen;
    datalen = strlen(data);
    assert(datalen % 4 == 0);
    return datalen / 4 * 3 - countchr(data, '=');
}

int b64encode(void* in, size_t inlen, char* out, size_t outlen) {
    char B64CHR[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    char PAD = '=';
    uint8_t num3[3], num4[4], padnum, j;
    uint32_t tempnum;
    size_t i, outptr = 0;

    if (outlen < b64enclen(inlen)) return 1;

    for (i = 0; i < inlen; i += 3) {
        num3[0] = ((uint8_t*)in)[i];

        if (inlen - i > 1)
            num3[1] = ((uint8_t*)in)[i + 1];
        else
            num3[1] = 0;

        if (inlen - i > 2)
            num3[2] = ((uint8_t*)in)[i + 2];
        else
            num3[2] = 0;

        tempnum = num3[0] << 16;
        tempnum += num3[1] << 8;
        tempnum += num3[2];

        num4[0] = (tempnum >> 18) & 63;
        num4[1] = (tempnum >> 12) & 63;
        num4[2] = (tempnum >> 6) & 63;
        num4[3] = tempnum & 63;

        for (j = 0; j < 4; j++) {
            out[outptr++] = B64CHR[num4[j]];
        }
    }

    if (inlen % 3 == 0) padnum = 0;
    else padnum = 3 - inlen % 3;

    for (i = 0; i < padnum; i++) {
        out[--outptr] = PAD;
    }

    return 0;
}

int b64decode(char* in, size_t inlen, void* out, size_t outlen) {
    uint8_t B64TABLE[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0,
        0, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
        22, 23, 24, 25, 0, 0, 0, 0, 63, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34,
        35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    uint8_t num3[3], num4[4], j;
    uint32_t tempnum;
    size_t i, outptr = 0;

    i = b64declen(in);

    if (outlen < i) return 1;

    outlen = i;

    for (i = 0; i < inlen; i += 4) {
        num4[0] = B64TABLE[(uint8_t)(in[i])];
        num4[1] = B64TABLE[(uint8_t)(in[i + 1])];
        num4[2] = B64TABLE[(uint8_t)(in[i + 2])];
        num4[3] = B64TABLE[(uint8_t)(in[i + 3])];

        tempnum = (num4[0] & 63) << 18;
        tempnum += (num4[1] & 63) << 12;
        tempnum += (num4[2] & 63) << 6;
        tempnum += (num4[3] & 63);

        num3[0] = (tempnum >> 16) & 255;
        num3[1] = (tempnum >> 8) & 255;
        num3[2] = tempnum & 255;

        for (j = 0; j < 3 && outptr < outlen; j++) {
            ((uint8_t*)out)[outptr++] = num3[j];
        }

        if (outptr >= outlen) break;
    }

    return 0;
}
