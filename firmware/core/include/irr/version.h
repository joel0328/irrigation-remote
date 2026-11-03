/*
 * Firmware and protocol versions. See docs/versioning.md.
 *
 * Firmware: fw-vMAJOR.MINOR.PATCH. IRR_FW_DEV is 1 until a build has been run
 * on hardware and tagged without the -dev suffix.
 * Protocol: proto-vMAJOR.MINOR. Must match docs/protocol/vMAJOR.MINOR.md.
 */
#ifndef IRR_VERSION_H
#define IRR_VERSION_H

#define IRR_FW_MAJOR 0
#define IRR_FW_MINOR 1
#define IRR_FW_PATCH 0
#define IRR_FW_DEV   1

#define IRR_PROTO_MAJOR 0
#define IRR_PROTO_MINOR 1

#endif
