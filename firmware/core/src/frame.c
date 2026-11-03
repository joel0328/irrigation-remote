#include "irr/frame.h"

#include <string.h>

#include "irr/version.h"

void irr_put_u16(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
}

void irr_put_u32(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

uint16_t irr_get_u16(const uint8_t *p)
{
    return (uint16_t)(p[0] | (p[1] << 8));
}

uint32_t irr_get_u32(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
}

uint8_t irr_proto_ver_byte(void)
{
    return (uint8_t)((IRR_PROTO_MAJOR << 4) | (IRR_PROTO_MINOR & 0x0f));
}

/* While MAJOR is 0 every minor version may break the wire format, so the
 * version byte must match exactly. From 1.0 on, only MAJOR must match. */
static int version_accepted(uint8_t ver)
{
    uint8_t ours = irr_proto_ver_byte();
    if (IRR_PROTO_MAJOR == 0) {
        return ver == ours;
    }
    return (ver >> 4) == (ours >> 4);
}

irr_frame_result irr_frame_encode(const irr_aes128_ctx *key, irr_header *hdr,
                                  const uint8_t *payload, uint8_t payload_len,
                                  uint8_t *out, size_t *out_len)
{
    if (payload_len > IRR_MAX_PAYLOAD) {
        return IRR_FRAME_TOO_BIG;
    }
    hdr->ver = irr_proto_ver_byte();
    hdr->len = payload_len;

    out[0] = hdr->ver;
    out[1] = hdr->type;
    irr_put_u32(&out[2], hdr->net_id);
    irr_put_u32(&out[6], hdr->counter);
    out[10] = hdr->len;
    if (payload_len) {
        memcpy(&out[IRR_HEADER_LEN], payload, payload_len);
    }

    size_t signed_len = IRR_HEADER_LEN + payload_len;
    uint8_t tag[IRR_AES_BLOCK];
    irr_cmac(key, out, signed_len, tag);
    memcpy(&out[signed_len], tag, IRR_TAG_LEN);
    *out_len = signed_len + IRR_TAG_LEN;
    return IRR_FRAME_OK;
}

irr_frame_result irr_frame_decode(const irr_aes128_ctx *key, const uint8_t *buf,
                                  size_t len, irr_header *hdr,
                                  const uint8_t **payload)
{
    if (len < IRR_HEADER_LEN + IRR_TAG_LEN) {
        return IRR_FRAME_SHORT;
    }
    uint8_t plen = buf[10];
    if (plen > IRR_MAX_PAYLOAD || len != (size_t)IRR_HEADER_LEN + plen + IRR_TAG_LEN) {
        return IRR_FRAME_BAD_LEN;
    }
    if (!version_accepted(buf[0])) {
        return IRR_FRAME_BAD_VERSION;
    }

    size_t signed_len = IRR_HEADER_LEN + plen;
    uint8_t tag[IRR_AES_BLOCK];
    irr_cmac(key, buf, signed_len, tag);
    if (!irr_ct_equal(tag, &buf[signed_len], IRR_TAG_LEN)) {
        return IRR_FRAME_BAD_TAG;
    }

    hdr->ver = buf[0];
    hdr->type = buf[1];
    hdr->net_id = irr_get_u32(&buf[2]);
    hdr->counter = irr_get_u32(&buf[6]);
    hdr->len = plen;
    *payload = &buf[IRR_HEADER_LEN];
    return IRR_FRAME_OK;
}
