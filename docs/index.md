# Irrigation Remote

A research and design project for a low-cost handheld radio remote that starts and stops zones on residential irrigation controllers. It does the same job as the TRC Commander. This site holds the research, the proposed design, the radio protocol, and the firmware and hardware as they are built.

!!! danger "Nothing here has been built or tested"
    Every design, protocol and firmware feature on this site is a proposal. No radio link, relay, fail-safe or confirmation feature has run on hardware. The portable firmware library passes unit tests on a PC, and that is all. Research claims are labelled by how well they are supported: only a capture or test made by this project counts as `confirmed`, and there are none yet.

## How it is meant to work

A handheld transmitter sends an authenticated command over the 902 to 928 MHz band to a receiver at the controller. The receiver's relays sit in series with the controller's zone wires, so it can run a zone on any 24 VAC controller without knowing that brand's remote-port protocol. The receiver holds the run timer: if the link or power is lost, the zone stops. See [architecture](architecture.md).

## Start here

- [Requirements](requirements.md) and [architecture](architecture.md)
- [Open questions](open-questions.md) and [build plan](build-plan.md)
- [Safety and regulatory notes](safety-regulatory.md)
- [Firmware](firmware.md), [radio protocol](protocol/index.md) and [versioning](versioning.md)
- [Decision records](adr/index.md) and [changelog](changelog.md)

## Research by brand

How each brand's own remote talks to its controllers. This matters for optional brand adapters; the in-line route does not depend on it.

- [Claim status across all brands](research/status.md)
- [TRC Commander](research/trc-commander.md)
- [Hunter](research/hunter.md)
- [Rain Bird](research/rain-bird.md)
- [Irritrol and Toro](research/irritrol-toro.md)
- [Measurement plan](research/measurement-plan.md), [vendor requests](research/vendor-requests.md), [research log](research/log.md)

Evidence rules and status labels: [research conventions](research/README.md).
