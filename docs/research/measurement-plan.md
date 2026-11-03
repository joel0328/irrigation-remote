# Measurement plan

Public sources are nearly exhausted for the remote ports. The remaining way to get the signals is to capture them from real equipment. This page is the plan. Nothing here has been done yet.

## Equipment

| Item | Why |
|---|---|
| Logic analyzer, at least 8 channels, at least 25 MS/s (Saleae Logic 8 or DSLogic Plus class) | Hunter timing is in the 200 µs to 2 ms range; the Rain Bird 5-pin clock period is unknown and in the microseconds. |
| Oscilloscope or scope-capable meter | Idle levels, edge shape, whether lines are open-drain. |
| Multimeter | Check every pin for voltage before connecting the analyzer. |
| One real receiver per brand | Hunter ROAM-R or ROAM XL-R; Rain Bird LIMR-RX (and an LNK or LNK2 module if available); Irritrol CL-MR. |
| One real controller per brand and port generation | The ports differ by model. Record the exact model and firmware. |
| 24 VAC supply with a fuse, breakout cables for each connector | The prototype never touches mains wiring. |

## Safety

- Work only on the 24 VAC side. Never open or probe the mains side of a controller. Unplug the controller before attaching or moving probes.
- Measure with a multimeter before attaching any analyzer input. The Rain Bird 5-pin carries +12 V and +5 V on neighbouring pins.
- Hunter REM is referenced to the AC#2 terminal, not to earth ground (HUN-019). Confirm with a meter that nothing on the controller ties AC#2 to earth before connecting an analyzer whose ground goes to a computer's USB ground. If in doubt, use an isolated or battery-powered setup.
- Valve solenoids are inductive. Do not leave valves connected to the bench setup unless the test needs them, and suppress them if it does.

## Capture procedure (every capture)

1. Fill in `captures/<brand>/<controller-model>/<YYYY-MM-DD>_<what>.yaml` with: controller make, model, firmware revision, date code; receiver make, model, revision; analyzer, sample rate, channels and what each probes; supply voltage; what was done (start zone 3 for 10 min, stop all, and so on).
2. Save the raw capture next to it (`.sal`, `.sr` or `.csv`). Use Git LFS for anything over 1 MB.
3. Take a photo of the probe setup.
4. Write the decoded result into the brand file and change the claim status to `confirmed` only if the capture shows it.
5. Add a row to `docs/research/log.md`.

## Captures in priority order

| # | Brand | What to capture | Answers |
|---|---|---|---|
| 1 | Hunter | REM during start zone 1, start zone 13, start zone 48 if possible, stop all, run program A. Also idle level and the AC#2-referenced swing. | HUN-011 to HUN-019; whether the controller ever replies |
| 2 | Rain Bird 5-pin | Clock and data with nothing attached, with an LNK module attached, and with a LIMR receiver attached during start and stop | RB-010 to RB-016; whether LIMR uses the same bus |
| 3 | Rain Bird 6-pin | All six contacts with a LIMR receiver attached, idle and during a command | RB-020 and RB-021 |
| 4 | Irritrol | All six conductors of the RJ-11 jack on a Rain Dial-R or Total Control-R with a CL-MR attached | IRR-007 |
| 5 | Irritrol older | 5-pin header on an MC-E or KwikDial via the CMR-ADP | IRR-002, IRR-004 |

## Decision after captures

For each brand decide whether a port adapter is worth building (documented signal, small, testable) or whether to leave the brand to the in-line route. See [architecture](../architecture.md).
