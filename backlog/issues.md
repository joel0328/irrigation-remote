# Backlog drafts

Drafts of GitHub issues, to be created once the repository exists on GitHub. `python tools/backlog_to_gh.py` prints a `gh issue create` command for each one; nothing is sent automatically.

Format: each issue is a `## ` heading (the title), a `labels:` line, then the body up to the next heading. Labels: `research`, `hardware`, `firmware`, `decision`, `vendor`.

Vendor issues are reminders for Joel or the friend. Nobody sends anything on their behalf.

## Decide: receiver power source (Q1)
labels: decision, hardware

Own 24 VAC transformer, or draw from the controller? Affects the enclosure, the wiring, and whether the receiver works when the controller is off. Record the decision as an ADR. See docs/open-questions.md Q1.

## Decide: master valve or pump output in v1 (Q2)
labels: decision, hardware

Proposed yes (REQ-05). Confirm with the friend, then write an ADR.

## Decide: zone count for v1 (Q3)
labels: decision, hardware

Proposed 8. Sets the relay and sense channel count.

## Decide: range target and cost target (Q4)
labels: decision

Needed before choosing antennas and the enclosure, and to judge Phase 1 results. Also record what comparable commercial remotes cost (NFR-05).

## Decide: whether to contact vendors (Q7)
labels: decision, vendor

Decide with the friend before any request goes out. See docs/research/vendor-requests.md.

## Decide: brand adapters or the in-line route only (Q8)
labels: decision

Adapters take research time and may need vendor specs. Ask the friend which brands he actually meets in the field.

## Buy two 902-928 MHz STM32WL boards (Q9)
labels: hardware, decision

No suitable board yet. Two are needed for the link test. Before buying, read the vendor's own page to confirm the band covers 902 to 928 MHz (ST's site timed out on 2026-11-03). Consider whether a pre-certified module board fits better than a Nucleo (NFR-03). Record the choice and order codes in Q9.

## Decide: commit e-mail before going public (Q10)
labels: decision

Commits are public once pushed. Choose a personal address or the GitHub no-reply address.

## Decide: MkDocs Material end of life
labels: decision

Material for MkDocs prints a notice that it reaches end of life on 2027-05-05, with Zensical as the successor. Decide when to migrate. Read the official notice first rather than relying on the build message.

## Rain Bird 5-pin and 6-pin remote-port signals (T1)
labels: research

Route: vendor request (see the vendor issue), then captures 2 and 3. Claims RB-010 to RB-021.

## Irritrol and Toro RJ-11 and 5-pin signals (T2)
labels: research

Route: FCC internal photos pages 2 and 3, vendor request, capture 4 and 5. Claims IRR-007, IRR-021, IRR-022.

## Does a Hunter controller ever reply on REM? (T4)
labels: research

Neither community implementation reads a reply (HUN-018). Answer with capture 1.

## Research Rain Master remote port (T5)
labels: research

New file docs/research/rain-master.md with an `RM` prefix (add it to tools/claims.py). Imperial's ProMax adapter shows Rain Master remotes are used across brands (RB-030).

## Research Weathermatic remote port (T5)
labels: research

New research file.

## Research Superior and HIT Logic remote ports (T5)
labels: research

New research file.

## Research Griswold remote port (T5)
labels: research

New research file.

## Research Orbit and other smart controllers (T5)
labels: research

Wi-Fi controllers may have an app or local API instead of a remote port. Scope: residential brands the friend actually meets.

## Measure real range and link margin in a yard (T6)
labels: hardware, firmware

Phase 1. Packet success rate against distance, with the chosen LoRa settings. Needs two boards (Q9).

## Measure handheld battery life (T7)
labels: hardware, firmware

Phase 1. Sleep and transmit current, then an estimate for a chosen battery (NFR-01).

## Hunter: open the pages the research environment could not reach
labels: research

Hackster write-up by Scott Shumate, loullingen.lu wiring page, earlier pages of the Home Assistant thread "Irrigation Hunter X-Core remote control using REM pin". A person with a normal browser may succeed (HUN-023). Log what is read.

## Hunter: compare a capture with HUN-011 to HUN-016
labels: research

After capture 1, update the status of each row. Promote to `confirmed` only what the capture shows.

## Rain Bird: read the rest of the ESP-RZX thread
labels: research

The protocol decoding was in progress when read (RB-013). Record any new findings as `reported` rows.

## Irritrol: read FCC exhibits 1808898 pages 2-3 and 1808905
labels: research

Use a desktop PDF reader. Look for pin labels on the receiver cable (IRR-021) and radio details (IRR-022).

## Capture 1: Hunter REM
labels: research, hardware

Start zone 1, 13, 48 if possible, stop all, run program A. Idle level and the AC#2-referenced swing. Check AC#2 against earth before connecting an analyzer. See measurement plan.

## Capture 2: Rain Bird 5-pin
labels: research, hardware

Clock and data with nothing attached, with an LNK module attached, and with a LIMR receiver during start and stop.

## Capture 3: Rain Bird 6-pin
labels: research, hardware

All six contacts with a LIMR receiver attached, idle and during a command.

## Capture 4: Irritrol RJ-11
labels: research, hardware

All six conductors on a Rain Dial-R or Total Control-R with a CL-MR attached.

## Capture 5: Irritrol older 5-pin header
labels: research, hardware

MC-E or KwikDial through a CMR-ADP.

## Vendor: Rain Bird remote-port specification
labels: vendor

The ProMax adapter sales sheet says the full specifications are available from a Rain Bird sales representative (RB-031). Draft in docs/research/vendor-requests.md. To be sent by Joel or the friend.

## Vendor: Imperial Technical Services interface
labels: vendor

Ask whether they will share or license the ITSRBPMA2 interface (RB-030).

## Vendor: Irritrol / Toro remote-port specification
labels: vendor

Irritrol technical assistance. See the tracker in vendor-requests.md.

## Vendor: Hunter SmartPort REM specification
labels: vendor

Hunter technical support.

## Firmware: add STM32CubeWL as a pinned dependency
labels: firmware

HAL, LL and the SubGHz radio driver. Decide git submodule or CMake FetchContent; pin a release tag. Check its licence terms (ST's licence, not MIT) and keep it outside the MIT-licensed tree.

## Firmware: verify the STM32WL linker script memory map
labels: firmware

Placeholder sizes (256 KB flash, 64 KB RAM). Check against the datasheet and RM0453 for the board bought, including the dual-core split on the WL55.

## Firmware: radio driver (LoRa) on the board layer
labels: firmware, hardware

Replace board_radio_* stubs. Choose spreading factor, bandwidth and channel plan within FCC Part 15 limits; record the reasoning.

## Firmware: relay outputs and voltage sense
labels: firmware, hardware

Replace board_set_outputs and board_read_sense stubs. Test break-before-make on the bench with real solenoids (Phase 2 gate).

## Firmware: counter persistence with wear levelling
labels: firmware

Store counters before replying. Block reservation scheme from protocol v0.1, "Counters". Test with power pulled mid-write.

## Firmware: watchdog and fault handling
labels: firmware

Independent watchdog; a reset must come up with all outputs off. Fault handler parks until the watchdog fires.

## Firmware: hardware AES on STM32WL
labels: firmware

Use the AES peripheral and check it against the same FIPS-197 and RFC 4493 vectors as the software version.

## Firmware: handheld application
labels: firmware

Buttons, display, send-and-retry loop, result display (acked, zone live, no answer). Not started.

## Design: pairing procedure (REQ-12)
labels: firmware, decision

How a handheld and a receiver agree on a key and net_id. Write an ADR and bump the protocol version.

## Design: key-management plan
labels: firmware, decision

Where keys are generated and stored, how they are replaced. Required before any field use; the current key is a placeholder.

## Protocol: let HELLO through across versions
labels: firmware

Today a version mismatch only shows as "no reply". See protocol v0.1, known gaps.
