/* Known-answer tests from FIPS-197 Appendix C.1 and RFC 4493 section 4. */
#include "irr/aes128.h"
#include "minitest.h"

static void test_fips197_c1(void)
{
    uint8_t key[16], pt[16], ct[16], out[16];
    mt_hex("000102030405060708090a0b0c0d0e0f", key);
    mt_hex("00112233445566778899aabbccddeeff", pt);
    mt_hex("69c4e0d86a7b0430d8cdb78070b4c55a", ct);
    irr_aes128_ctx ctx;
    irr_aes128_init(&ctx, key);
    irr_aes128_encrypt(&ctx, pt, out);
    CHECK_MEM(out, ct, 16);
}

static void test_fips197_key_expansion_last_round(void)
{
    /* FIPS-197 Appendix A.1: last round key for 2b7e1516... */
    uint8_t key[16], rk10[16];
    mt_hex("2b7e151628aed2a6abf7158809cf4f3c", key);
    mt_hex("d014f9a8c9ee2589e13f0cc8b6630ca6", rk10);
    irr_aes128_ctx ctx;
    irr_aes128_init(&ctx, key);
    CHECK_MEM(&ctx.round_keys[160], rk10, 16);
}

static const char *RFC_KEY = "2b7e151628aed2a6abf7158809cf4f3c";
static const char *RFC_MSG =
    "6bc1bee22e409f96e93d7e117393172a"
    "ae2d8a571e03ac9c9eb76fac45af8e51"
    "30c81c46a35ce411e5fbc1191a0a52ef"
    "f69f2445df4f9b17ad2b417be66c3710";

static void cmac_case(size_t len, const char *expect_hex)
{
    uint8_t key[16], msg[64], expect[16], tag[16];
    mt_hex(RFC_KEY, key);
    mt_hex(RFC_MSG, msg);
    mt_hex(expect_hex, expect);
    irr_aes128_ctx ctx;
    irr_aes128_init(&ctx, key);
    irr_cmac(&ctx, msg, len, tag);
    CHECK_MEM(tag, expect, 16);
}

static void test_rfc4493_empty(void) { cmac_case(0, "bb1d6929e95937287fa37d129b756746"); }
static void test_rfc4493_16(void) { cmac_case(16, "070a16b46b4d4144f79bdd9dd04a287c"); }
static void test_rfc4493_40(void) { cmac_case(40, "dfa66747de9ae63030ca32611497c827"); }
static void test_rfc4493_64(void) { cmac_case(64, "51f0bebf7e3b9d92fc49741779363cfe"); }

static void test_ct_equal(void)
{
    uint8_t a[4] = {1, 2, 3, 4}, b[4] = {1, 2, 3, 4}, c[4] = {1, 2, 3, 5};
    CHECK(irr_ct_equal(a, b, 4));
    CHECK(!irr_ct_equal(a, c, 4));
}

int main(void)
{
    RUN(test_fips197_c1);
    RUN(test_fips197_key_expansion_last_round);
    RUN(test_rfc4493_empty);
    RUN(test_rfc4493_16);
    RUN(test_rfc4493_40);
    RUN(test_rfc4493_64);
    RUN(test_ct_equal);
    return MT_DONE();
}
