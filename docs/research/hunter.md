# Hunter SmartPort and the REM line

Hunter's ROAM and ROAM XL remotes plug into a "SmartPort" on Hunter controllers. Hunter publishes the wiring but not the signal. Community projects have reverse-engineered the signal on the REM wire. This is the best-understood port of the four brands researched, and still unmeasured by us. Read [README](README.md) for status labels.

## Documented by Hunter

| ID | Claim | Status | Source | Accessed |
|---|---|---|---|---|
| HUN-001 | The SmartPort harness has three control wires: red and white to the controller's AC terminals, blue to R (or REM on some controllers). Controller-specific: SRC red to left AC, white to next AC, blue to R; X-Core red and white to the two 24 VAC terminals, blue to REM; Pro-C main module and ICC power module red to bottom AC, white to upper AC, blue to REM. ACC and I-Core have the SmartPort built in. | documented | [ROAM XL manual](https://www.hunterirrigation.com/sites/default/files/om_roamxl_em.pdf) | 2026-11-02 |
| HUN-002 | The manual does not give a pin count or any electrical specification for the REM signal. | unknown | ROAM XL manual | 2026-11-02 |
| HUN-003 | ROAM XL: 27 MHz band, open-field range up to 2 miles, remote run time 1 to 90 minutes, one station at a time on most controllers (starting one stops any running station; programs are an exception). | documented | ROAM XL manual | 2026-11-02 |
| HUN-004 | ROAM (standard): 433.925 MHz, superheterodyne receiver, open-field range up to 1,000 ft, 10 selectable run times from 1 to 90 minutes. Transmitter and receiver each have an address 0 to 127 (default 1); the receiver learns the transmitter's address within about 10 seconds of power-up and beeps four times on success. | documented | [FCC exhibit: owner's manual](https://fcc.report/FCC-ID/M3URMT/1061873.pdf) | 2026-11-02 |
| HUN-005 | The receiver runs on 24 VAC at 0.010 A, powered through the controller. | documented | FCC exhibit above | 2026-11-02 |
| HUN-006 | Maximum station number is adjustable 1 to 240 (default 15). 240 works only with the ACC; for SRC, Pro-C and ICC use 1 to 48. | documented | FCC exhibit above | 2026-11-02 |
| HUN-007 | Harness limits: SmartPort no more than 15 m (50 ft) from the controller; shielded cable recommended beyond about 2 m (6 ft). The manuals disagree on the shield threshold (5 ft in one place). A lengthened harness can cause an ERR message. Receiver is for temporary outdoor use only. | documented | ROAM XL manual, FCC exhibit | 2026-11-02 |
| HUN-008 | The FCC test report for the ROAM transmitter (FCC ID M3URMT) is a Section 15.231 emissions test and says nothing about modulation, encoding or the SmartPort signal. | documented | [FCC test report](https://fcc.report/FCC-ID/M3URMT/1061875.pdf) | 2026-11-02 |
| HUN-009 | The filing for the long-range receiver (M3URMLR) could not be opened. | unknown | fccid.io | 2026-11-02 |

## Reverse-engineered by the community

The sources are the original work by Scott Shumate (credited by all the others), a Hackaday project, and two GitHub projects built on a Hunter Pro-C or X-Core. The firmware was tested on a Pro-C PC300 only. Verify licences before copying code: `hunter.cpp` in the first repo is marked CC0, but the repository as a whole carries the OpenSprinkler licence.

| ID | Claim | Status | Source | Accessed |
|---|---|---|---|---|
| HUN-010 | REM is one signal wire. The protocol is a custom single-wire interface "akin to Dallas 1-Wire" with large, sparse packets. | reported | [Hackaday project](https://hackaday.io/project/186458-hunter-sprinkler-wifi-adapter) | 2026-11-02 |
| HUN-011 | Wake-up: line high 325 ms, then low 65 ms. Start pulse: high 900 µs, then low about 208 µs. | reported | [dolenle/OpenSprinkler-Firmware-Hunter, hunter.cpp](https://github.com/dolenle/OpenSprinkler-Firmware-Hunter); [ecodina/hunter-wifi](https://github.com/ecodina/hunter-wifi) | 2026-11-02 |
| HUN-012 | Bits are pulse-width coded, most significant bit first, one per slot of about 2.08 ms. A 0 is high 208 µs then low 1,875 µs. A 1 is high 1,875 µs then low 208 µs. In the bit-banged version the line idles low and the pulses are high. | reported | ecodina/hunter-wifi (`HunterRoam.cpp`) | 2026-11-02 |
| HUN-013 | The first repo makes the same waveform with a UART at 4,807 baud and inverted polarity, sending 0x00 or 0xFF per slot. | reported | dolenle repo | 2026-11-02 |
| HUN-014 | A start-zone frame is 121 slots (about 250 ms) followed by a stop pulse. It carries zone 1 to 48 and run time 0 to 240 minutes, each repeated several times: zone plus 0x17 at bit positions 23 to 29 and 36 to 42, zone plus 0x23 at 49 to 55 and 62 to 68, zone plus 0x2f at 75 to 81 and 88 to 94, run time as nibbles at 31 to 34, 44 to 47, 57 to 60, 70 to 73, 83 to 86 and 96 to 99, and the low nibble of zone minus 1 at 109 to 112. Bits 9 and 10 are 1 for zones above 12, otherwise 2. The rest of the frame is a fixed template (see the cited source). | reported | dolenle repo | 2026-11-02 |
| HUN-015 | Stop all is a start command for zone 1 with run time 0. | reported | dolenle repo | 2026-11-02 |
| HUN-016 | A program-run frame is 56 slots; program number minus 1 sits at bits 31 and 32; programs 1 to 4. | reported | dolenle repo | 2026-11-02 |
| HUN-017 | No checksum is computed in the code; the frame is a fixed template with redundant fields. | inferred | HUN-014 | 2026-11-02 |
| HUN-018 | Nothing in either implementation reads a reply from the controller. | inferred | both repos | 2026-11-02 |
| HUN-019 | REM is referenced to the controller's AC#2 terminal, not to a shared ground. Both projects tie the adapter's supply to AC#2. One says the signal is 5 V with respect to AC#2; a 3.3 V GPIO also works. | reported | [ecodina docs](https://github.com/ecodina/hunter-wifi), [marek-polak/hunter-wifi](https://github.com/marek-polak/hunter-wifi) | 2026-11-02 |
| HUN-020 | One project reports two X-Core MCU revisions with 3.3 V or 5 V internal logic, citing a GitHub issue comment. | reported | marek-polak README | 2026-11-02 |
| HUN-021 | Because REM is referenced to an AC terminal, a Hunter adapter must run from a floating or isolated supply. | inferred | HUN-019 | 2026-11-02 |
| HUN-022 | The library accepts run times up to 240 minutes while the ROAM manuals state 1 to 90 for the remote; the controller may clamp the value. | inferred | HUN-003, HUN-014 | 2026-11-02 |
| HUN-023 | Scott Shumate's original write-up and the loullingen.lu wiring page could not be opened from the research environment. | unknown | hackster.io, loullingen.lu | 2026-11-02 |

## Not yet known

- Exact idle voltage on REM, whether the line is open-drain, and the polarity convention as seen on a scope.
- Whether the controller ever replies on REM.
- Whether ROAM (433.925 MHz) and ROAM XL (27 MHz) receivers speak the same wire protocol on the SmartPort. Probably, but nothing confirms it.
- Whether the X-Core, Pro-C, ICC, ACC and I-Core all accept the same frames.

## Next steps

1. Open the unopened pages (Hackster write-up, loullingen.lu, earlier pages of the Home Assistant thread "Irrigation Hunter X-Core remote control using REM pin"). A person with a normal browser may succeed where the research environment did not.
2. Capture a real ROAM receiver's REM wire during a start, a stop and a program run. See [measurement plan](measurement-plan.md).
3. Compare the capture against HUN-011 to HUN-016 and update the status of each row.
