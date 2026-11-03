# Build plan (proposal)

Five phases with a gate between each. A phase is not done until its gate is met and recorded in the repository (test results, photos, captures). Nothing here has been started.

| Phase | Goal | Gate |
|---|---|---|
| 0. Research and decisions | Close the highest-value unknowns and agree scope with the friend | Open questions Q1 to Q9 answered or consciously deferred; vendor requests sent or declined |
| 1. Radio link on dev boards | Two STM32WL boards exchange authenticated messages; measure range and current | Packet success rate and range written up; handheld battery estimate; keys and counter tested for replay |
| 2. Receiver outputs on the bench | Relay outputs, voltage sense, run timer, fail-safe, with 24 VAC and real solenoids | Fail-safe tested by pulling the radio and the power; no stuck-on case found in repeated tests |
| 3. First hardware revision | KiCad design for handheld and receiver; assembled boards | Board bring-up report; fuse and protection tested |
| 4. Field trial | Use at a real site with a real controller | Written trial notes from the friend's company; list of faults found |
| 5. Brand adapters (optional) | Add a remote-port adapter where signals are documented | Capture-backed protocol notes and a bench test with the real controller |

## Firmware layout proposed

- A portable C library for protocol, framing, authentication, run-timer state machine. No hardware calls inside it.
- Host unit tests for that library, run in CI.
- A thin hardware layer for STM32WL (HAL or LL) kept separate.
- The firmware version is carried in the protocol hello message so mismatched pairs can be detected.

## Versioning

See the root README and `docs/versioning.md` in the repository for the scheme the handoff prompt asks you to set up. Short form: research snapshots, protocol versions, firmware releases and hardware revisions each have their own tag family.
