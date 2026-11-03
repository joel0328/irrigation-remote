# Firmware

Status: early scaffolding. The portable library builds and passes unit tests on a PC. Nothing has run on a microcontroller.

## Layout

```
firmware/
  core/            portable C11 library, no hardware calls
    include/irr/   public headers
    src/
  tests/           host unit tests (CTest)
  port/stm32wl/    STM32WL receiver port: startup, linker script, board stubs
```

## What exists and how far it is proven

| Part | Where | State |
|---|---|---|
| AES-128 encrypt and AES-128-CMAC | `core/src/aes128.c` | Passes the FIPS-197 and RFC 4493 known-answer tests on a PC. Software reference only, not hardened against side channels. <span class="untested">untested on hardware</span> |
| Frame encode and decode, 8-byte CMAC tag | `core/src/frame.c` | Unit-tested: round trip, every single-bit flip rejected, wrong key, length and version checks. <span class="untested">untested on hardware</span> |
| Rolling-counter replay check | `core/src/counter.c` | Unit-tested: stale, duplicate-retry and fresh cases, counter exhaustion. Counter persistence is not written. <span class="untested">untested on hardware</span> |
| Run-timer state machine | `core/src/runtimer.c` | Unit-tested: runs end without any message, break-before-make, one zone at a time, clamp and default, clock wrap. <span class="untested">untested on hardware</span> |
| Receiver and handheld message handling | `core/src/receiver.c`, `handheld.c` | Unit-tested end to end through an in-memory link, including replay and reflection attacks. <span class="untested">untested on hardware</span> |
| STM32WL startup and linker script | `port/stm32wl/` | Builds in CI with arm-none-eabi-gcc. Memory sizes not checked against the datasheet. <span class="untested">never run</span> |
| Radio, relays, voltage sense, flash storage, watchdog | `port/stm32wl/src/board_stub.c` | **Stubs only.** No driver written. <span class="untested">not implemented</span> |
| Handheld UI and port | none | Not started. |
| Pairing and key provisioning | none | Not designed. The port uses an all-zero placeholder key that must never be used in the field. |

The radio link, the relays, the fail-safe in hardware, and the zone-live confirmation are **not demonstrated**. The unit tests show that the logic does what the tests check, and nothing more.

## Build and test on a PC

You need CMake 3.20 or newer, Ninja (or Make), and a C compiler (GCC, Clang, or `zig cc`).

```sh
cmake -S firmware -B build/host -G Ninja
cmake --build build/host
ctest --test-dir build/host --output-on-failure
```

Add `-DIRR_SANITIZE=ON` for AddressSanitizer and UBSan (Linux and macOS). CI runs the tests with GCC and Clang, both with sanitizers.

## Build the STM32WL port

Needs `arm-none-eabi-gcc` and newlib. Use an absolute path for the toolchain file.

```sh
cmake -S firmware/port/stm32wl -B build/stm32wl -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=$PWD/firmware/port/stm32wl/arm-none-eabi.cmake
cmake --build build/stm32wl
```

The port deliberately avoids vendor code for now. When a board is available, the plan is to add STM32CubeWL (HAL, LL and the SubGHz radio driver) as a pinned dependency and replace the stubs one at a time, each with a bench test written up in the repository.

## Design rules

- The core makes no hardware calls. Everything it needs (time, radio bytes, sense readings, storage) is passed in, so all of it can be tested on a PC (NFR-07).
- The receiver rewrites its outputs from the run timer on every loop pass. A run ends because the receiver's own timer runs out, not because a stop message arrives (REQ-08).
- Relays must be normally open, so loss of power means all zones off.
- Counters are stored before a reply is sent, so a reset cannot reuse a counter value.
- Keys in the repository are placeholders. A key-management plan comes before any field use.

## Versions

Firmware `0.1.0-dev`, protocol `0.1` (draft). See [versioning](versioning.md).
