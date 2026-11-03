# Radio protocol

The message format between the handheld and the receiver. Each version has its own page. The firmware's `IRR_PROTO_MAJOR` and `IRR_PROTO_MINOR` (in `firmware/core/include/irr/version.h`) must match the page the code implements. Rules for changing versions are in [versioning](../versioning.md).

| Version | Status | Page |
|---|---|---|
| 0.1 | draft; implemented in the portable library and host tests; never sent over a radio | [v0.1](v0.1.md) |

Status values: `draft` (spec written), `implemented` (code and host tests match the spec), `field-tested` (run between two real boards), `superseded`.
