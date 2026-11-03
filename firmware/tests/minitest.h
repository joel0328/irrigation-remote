/*
 * Minimal unit-test helpers. One test executable per source file; each
 * returns non-zero if any check failed, so CTest reports it.
 */
#ifndef MINITEST_H
#define MINITEST_H

#include <stdio.h>
#include <string.h>

static int mt_failures = 0;
static int mt_checks = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        mt_checks++;                                                         \
        if (!(cond)) {                                                       \
            mt_failures++;                                                   \
            printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);         \
        }                                                                    \
    } while (0)

#define CHECK_EQ(a, b)                                                       \
    do {                                                                     \
        long long mt_a = (long long)(a), mt_b = (long long)(b);              \
        mt_checks++;                                                         \
        if (mt_a != mt_b) {                                                  \
            mt_failures++;                                                   \
            printf("  FAIL %s:%d: %s == %s (%lld != %lld)\n", __FILE__,      \
                   __LINE__, #a, #b, mt_a, mt_b);                            \
        }                                                                    \
    } while (0)

#define CHECK_MEM(a, b, n)                                                   \
    do {                                                                     \
        mt_checks++;                                                         \
        if (memcmp((a), (b), (n)) != 0) {                                    \
            mt_failures++;                                                   \
            printf("  FAIL %s:%d: %s matches %s\n", __FILE__, __LINE__, #a,  \
                   #b);                                                      \
        }                                                                    \
    } while (0)

#define RUN(test)                                                            \
    do {                                                                     \
        int mt_before = mt_failures;                                         \
        test();                                                              \
        printf("%s %s\n", mt_failures == mt_before ? "ok  " : "FAIL", #test); \
    } while (0)

#define MT_DONE()                                                            \
    (printf("%d checks, %d failed\n", mt_checks, mt_failures),               \
     mt_failures ? 1 : 0)

/* Parse a hex string (no separators) into out. Returns byte count. */
#if defined(__GNUC__)
__attribute__((unused))
#endif
static size_t mt_hex(const char *hex, unsigned char *out)
{
    size_t n = 0;
    while (hex[0] && hex[1]) {
        unsigned int b;
        sscanf(hex, "%2x", &b);
        out[n++] = (unsigned char)b;
        hex += 2;
    }
    return n;
}

#endif
