# Hardware

KiCad projects, one folder per board. Nothing designed yet; Phase 3 of the [build plan](../docs/build-plan.md).

Planned:

- `receiver/`: STM32WL module, 8 zone relays plus master or pump, voltage sense per output, fused 24 VAC input with protection to survive above 28 VAC (NFR-02).
- `handheld/`: STM32WL module, buttons and display, battery.

Each released revision is tagged `hw-revX` and its folder holds the schematic PDF, Gerbers, BOM, pick-and-place and a bring-up report. See [versioning](../docs/versioning.md).

Prototype work is on the 24 VAC side only. No mains wiring.
