# Irritrol and Toro remote ports

Irritrol and Toro (one company) have several generations of remote receiver, each with a different connection to the controller. The connectors are documented. The signals are not, and the FCC schematics that would help are not published. Read [README](README.md) for status labels.

## Connection generations

| ID | Claim | Status | Source | Accessed |
|---|---|---|---|---|
| IRR-001 | Newest receiver, CL-MR (part of the R-100-KIT, handheld CL-R1): a built-in RJ-11 cable plugs into an RJ-11 port on Rain Dial-R, Rain Dial Blue and Total Control-R controllers (front panel edge). Extension cables up to 50 ft must have six conductors. Never plug into a telephone system. The receiver draws power from the controller through the jack. | documented | [R-100 user manual](https://manualmachine.com/irritrol/r100kit/22750298-user-manual), [CRR manual](https://www.manualsdir.com/manuals/802520/irritrol-crr.html?page=4), [CRR brochure](https://cdn2.toro.com/en/-/media/Files/Irritrol/products/sensors-and-remotes/crr/crr_19tl1618NA.ashx) | 2026-11-02 |
| IRR-002 | Early MC-E Blue and KwikDial controllers have no RJ-11 jack. The CMR-ADP adapter goes from RJ-11 to a 5-pin header that slides onto the controller's 5-pin connector. | documented | R-100 user manual | 2026-11-02 |
| IRR-003 | Older CMR-KIT receiver: RJ-45 plug (CMR-CC) with two separate power wires that go to the controller's 24 VAC terminals (YEL and RED terminals on MC-E), either way round. Maximum receiver draw 75 mA. | documented | [CMR-KIT manual](https://manualmachine.com/irritrol/cmrkit/21099730-user-manual), [CMR-KIT compatibility guide](https://xrefs0.plumbersstock.com/irritrol/reference_gd_cmrkit.pdf) | 2026-11-02 |
| IRR-004 | Connection points for the older kit: MC-E has a 5-pin remote connector on the lower right of the timing board (a larger 6-pin connector is also present); KwikDial has a 5-pin connector above the terminal block; blue-faced Rain Dial has an RJ-45 socket on the back of the timing mechanism; Rain Dial Plus needs a KSR-ADP adapter board on the ribbon cable socket and only for date code 100400 or newer. | documented | CMR-KIT compatibility guide | 2026-11-02 |
| IRR-005 | Compatible controllers for CL-MR: Irritrol Rain Dial-R, Rain Dial Blue, Total Control-R, KwikDial, MC-E Blue (v5.02 or later); Toro Custom Command, TMC-212, TMC-424 and TMC-424E. For the older CMR-KIT: Irritrol Rain Dial-Plus and MC-E; Toro TMC-212, TMC-424, Greenkeeper 212. | documented | R-100 user manual, CMR-KIT manual | 2026-11-02 |
| IRR-006 | The Total Control-R user guide does not describe a remote port. | unknown | [Total Control manual](https://cdn2.toro.com/en/-/media/Files/Irritrol/products/controllers/total-control-r-series/manual_totalcontrol.ashx) | 2026-11-02 |
| IRR-007 | No pin assignments or wire colours are published for the 5-pin header, the RJ-45 socket or the RJ-11 jack. | unknown | all of the above | 2026-11-02 |

## Radio and addressing

| ID | Claim | Status | Source | Accessed |
|---|---|---|---|---|
| IRR-010 | CL-MR and CL-R1 FCC grants (FCC IDs OF7CLMR and OF7CLR1) cover 906 to 922 MHz under Part 15C, filed 2012-10-05. | documented | [fccid.io OF7CLMR](https://fccid.io/OF7CLMR) | 2026-11-02 |
| IRR-011 | Addressing is a 4-digit PIN (10,000 values), set on the transmitter and taught to the receiver by holding the receiver's button more than 8 seconds and then pressing Enter on the transmitter. About 1,000 ft line of sight. | documented | CRR brochure, FCC user guide | 2026-11-02 |
| IRR-012 | The transmitter shows a running-station icon, but no command acknowledgement is described. Stopping the transmitter does not stop a running station. | documented | R-100 user manual | 2026-11-02 |
| IRR-013 | Toro TMR-1 handheld (separate product): RJ-11 jack, receiver takes its own 22 to 26 VAC at up to 75 mA from the controller's 24 VAC terminals, MURS channels 151.82, 151.88, 151.94, 154.57 and 154.6 MHz, about 1.5 miles line of sight, transmitter maximum output 27 dBm (FCC ID OXP-TMR-1-TX). Default highest station 24, adjustable 1 to 512. Manual runs fixed at 10 minutes. | documented | [TMR-1 manual](https://manualmachine.com/toro/tmr1/2091772-user-manual), [TMR-1 specifications](https://manualslib.com/manual/818974/Toro-Tmr-1.html?page=22) | 2026-11-02 |

## What the FCC filing does and does not contain

| ID | Claim | Status | Source | Accessed |
|---|---|---|---|---|
| IRR-020 | The OF7CLMR grant requested a block diagram, schematics and an operational description, but those three are not published. The public exhibits are the user guide, internal and external photos, a label, test setup photos, a test report, and cover letters including a confidentiality request. | documented | fccid.io exhibit list | 2026-11-02 |
| IRR-021 | The internal photos (3 pages) show the receiver's cable wires soldered directly to the board. Pin labels could not be made out at the resolution available; only page 1 was viewed. | documented | [FCC internal photos](https://fccid.io/OF7CLMR/Internal-Photos/INTERNAL-PHOTOS-1808898) | 2026-11-02 |
| IRR-022 | The test report (exhibit 1808905) was not read: the PDF had no extractable text and the viewer could not be paged. It would show radio details only, not the controller port. | unknown | fccid.io exhibit 1808905 | 2026-11-02 |

A person with a desktop PDF reader can read pages 2 and 3 of the internal photos and the whole test report. That is worth doing before buying hardware, and cheap.

## Next steps

1. Read FCC exhibits 1808898 (pages 2 and 3) and 1808905 in a PDF reader; record findings here.
2. Ask Irritrol or Toro technical support for the remote-port interface specification (see [vendor requests](vendor-requests.md)).
3. Probe the RJ-11 jack with a CL-MR plugged in and a logic analyzer on all six conductors. See [measurement plan](measurement-plan.md).
