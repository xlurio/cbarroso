#include <stdint.h>
#include <unistd.h>
#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cbarroso/_hash.h>
#include <cbarroso/constants.h>

#define ROTATE(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))

#define HALF_ROUND(a, b, c, d, s, t) \
    a += b;                          \
    c += d;                          \
    b = ROTATE(b, s) ^ a;            \
    d = ROTATE(d, t) ^ c;            \
    a = ROTATE(a, 32);

#define SINGLE_ROUND(v0, v1, v2, v3)    \
    HALF_ROUND(v0, v1, v2, v3, 13, 16); \
    HALF_ROUND(v2, v1, v0, v3, 17, 21);

typedef struct SipHashSecret
{
    uint64_t k0, k1;
} SipHashSecret;

SipHashSecret *sipHashSecret;
uint8_t wasSecretInitialized = 0;

static int8_t sInitializeSecret()
{
    const char *urandomPath = "/dev/urandom";
    FILE *urandomFile = fopen(urandomPath, "r");
    ssize_t secretSize = sizeof(SipHashSecret);

    if (urandomFile == NULL)
    {
        fprintf(stderr, "Failed to open '%s'\n", urandomPath);
        return CBR_ERROR;
    }

    sipHashSecret = malloc(sizeof(SipHashSecret));

    if (sipHashSecret == NULL)
    {
        fprintf(stderr,
                "Failed to allocate memory for SipHash secret '%s'\n",
                urandomPath);
        fclose(urandomFile);
        return CBR_ERROR;
    }

    do
    {
        ssize_t bytesRead = read(fileno(urandomFile),
                                 sipHashSecret,
                                 (size_t)secretSize);

        if (bytesRead < 0)
        {
            fprintf(stderr,"Failed to read '%s'\n",urandomPath);
            fclose(urandomFile);
            free(sipHashSecret);
            return CBR_ERROR;
        }

        secretSize -= bytesRead;
    } while (0 < secretSize);

    fclose(urandomFile);

    wasSecretInitialized = 1;

    return CBR_SUCCESS;
}

static hash_t sSiphash13(uint64_t k0, uint64_t k1, const void *src, size_t srcSize)
{
    uint64_t b = (uint64_t)srcSize << 56;
    const uint8_t *in = (const uint8_t *)src;

    uint64_t v0 = k0 ^ 0x736f6d6570736575ULL;
    uint64_t v1 = k1 ^ 0x646f72616e646f6dULL;
    uint64_t v2 = k0 ^ 0x6c7967656e657261ULL;
    uint64_t v3 = k1 ^ 0x7465646279746573ULL;

    uint64_t t;
    uint8_t *pt;

    while (srcSize >= 8)
    {
        uint64_t mi;
        memcpy(&mi, in, sizeof(mi));
        mi = le64toh(mi);
        in += sizeof(mi);
        srcSize -= sizeof(mi);
        v3 ^= mi;
        SINGLE_ROUND(v0, v1, v2, v3);
        v0 ^= mi;
    }

    t = 0;
    pt = (uint8_t *)&t;
    switch (srcSize)
    {
    case 7:
        pt[6] = in[6];
    case 6:
        pt[5] = in[5];
    case 5:
        pt[4] = in[4];
    case 4:
        memcpy(pt, in, sizeof(uint32_t));
        break;
    case 3:
        pt[2] = in[2];
    case 2:
        pt[1] = in[1];
    case 1:
        pt[0] = in[0];
        break;
    }
    b |= le64toh(t);

    v3 ^= b;
    SINGLE_ROUND(v0, v1, v2, v3);
    v0 ^= b;
    v2 ^= 0xff;
    SINGLE_ROUND(v0, v1, v2, v3);
    SINGLE_ROUND(v0, v1, v2, v3);
    SINGLE_ROUND(v0, v1, v2, v3);

    t = v0 ^ v1 ^ v2 ^ v3;
    return t;
}

hash_t hashBuffer(const void *buffer, size_t len)
{
    if (len <= 0)
    {
        return 0;
    }

    if (!wasSecretInitialized)
    {
        if (sInitializeSecret() < 0)
        {
            fprintf(stderr, "Failed to initialize SipHash-1-3 secret\n");
            exit(EXIT_FAILURE);
        }
    }

    return (hash_t)sSiphash13(
        le64toh(sipHashSecret->k0), le64toh(sipHashSecret->k1),
        buffer, len);
}
