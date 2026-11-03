# Rain Bird remote ports

Rain Bird controllers have a remote port that comes in two physical versions, 5-pin and 6-pin. Rain Bird publishes no pinout for either. One forum poster measured the 5-pin port on an ESP-RZX. The 6-pin port has no signal data anywhere we could find. Read [README](README.md) for status labels.

## Which controller has which port

| ID | Claim | Status | Source | Accessed |
|---|---|---|---|---|
| RB-001 | 6-pin: ESP-LXME and ESP-LXD. 5-pin: ESP-LXME2, ESP-LXIVM, ESP-ME3, ESP 2-Wire, ESP-TM2. | documented | [Rain Bird handheld remote options](https://www.rainbird.com/products/handheld-remote-control-options-rain-bird-controllers), [ProMax adapter sheet](https://www.rainbird.com/sites/default/files/media/documents/2025-11/itsrbpma2.pdf) | 2026-11-02 |
| RB-002 | The LIMR quick-start guide lists 6-pin for ESP-Modular and ESP-LX series controllers, and 5-pin for ESP-SMT, STP, ESP-ME and ESP-RZX. Models and generations vary, so check the physical port. The guide does not use the name "LIMR" in its text. | documented | [LIMR quick-start guide](https://www.rainbird.com/sites/default/files/media/documents/2018-02/man_LIMR_QuickStartGuide_en.pdf) | 2026-11-02 |
| RB-003 | LIMR parts: LIMR-KIT (900 MHz transmitter, receiver, quick-connects, case), LIMR-TX, LIMR-RX, LIMR-QC603 and QC630 (6-pin quick-connect cables, 3 ft and 30 ft). | documented | [ESP-LXME installation guide](https://www.rainbird.com/sites/default/files/media/documents/2018-02/man_ESP-LXME_InstallationTroubleshootingGuide.pdf) | 2026-11-02 |
| RB-004 | Rain Bird recommends mounting the LIMR quick-connect beside or above the controller, not below, because below can interfere with the controller's transformer and reduce operating distance. Antenna vertical, not inside a metal cabinet. | documented | LIMR quick-start guide | 2026-11-02 |
| RB-005 | 6-pin controllers send no status back to the LIMR, so use the OFF command to stop a zone. | documented | LIMR quick-start guide | 2026-11-02 |

## The 5-pin port (one person's measurements)

All rows come from one forum thread by one poster on one controller, [Reverse engineering ACCESSORY port - Rain Bird ESP-RZX](https://community.home-assistant.io/t/reverse-engineering-accessory-port-rain-bird-esp-rzx/998452). The thread's protocol decoding was still in progress when read. Treat it as a lead, not a specification.

| ID | Claim | Status | Source | Accessed |
|---|---|---|---|---|
| RB-010 | Pinout, left to right: +12 V, GND, +5 V, clock (about 3.3 V), data (about 3.3 V). | reported | ESP-RZX thread | 2026-11-02 |
| RB-011 | With nothing attached, the clock and data pins show a square wave of about 50 Hz and no frames. | reported | ESP-RZX thread | 2026-11-02 |
| RB-012 | The controller leaves idle when it sees activity on the data pin, and returns to idle as soon as that activity stops. | reported | ESP-RZX thread | 2026-11-02 |
| RB-013 | Frames are clocked and SPI-like, not UART. Two truncated captures begin 20 43 80 04 08 10 A1 00 02 02 and 02 07 16 80 02 00 43 80 04. No baud rate, frame length, checksum or command set known. | reported | ESP-RZX thread | 2026-11-02 |
| RB-014 | Resistance to ground: pin 4 180 kΩ, pin 5 100 kΩ; pins 4 and 5 are 4.7 kΩ apart; both are open to +5 V. | reported | ESP-RZX thread | 2026-11-02 |
| RB-015 | The controller does transmit on the data pin once activated; whether that is a two-way exchange is not established. | reported | ESP-RZX thread | 2026-11-02 |
| RB-016 | The LNK WiFi module and the LIMR receiver harness both plug into the 5-pin accessory port. This suggests the LIMR speaks the same bus; nothing confirms it. | documented / inferred | [ESP-Me manual p. 29](https://www.manualslib.fr/manual/50074/Rain-Bird-Esp-Me.html?page=29) | 2026-11-02 |
| RB-017 | LNK2 spec: plugs into the accessory port of ESP-ME3, ESP-Me, ESP-TM2, ESP-RZXe; rated 24 VAC and 55 mA maximum; no pin count or interface description. The rating does not match the +12 V and +5 V pins in RB-010, so it is probably the controller's supply rating. | documented / inferred | [LNK2 spec](https://www.rainbird.com/sites/default/files/media/documents/2022-06/d42005_24jn21_lnk2-wifi-tech-spec_en.pdf) | 2026-11-02 |

## The 6-pin port

| ID | Claim | Status | Source | Accessed |
|---|---|---|---|---|
| RB-020 | No pin functions, voltages or signals found. Only the cable names and the fact that the ProMax adapter cables end in RJ45 clips. | unknown | LXME guide, ProMax adapter sheet | 2026-11-02 |
| RB-021 | It may be the same bus on a different connector. No source says so. | inferred | RB-016 | 2026-11-02 |

## A third party already interfaces with these ports

| ID | Claim | Status | Source | Accessed |
|---|---|---|---|---|
| RB-030 | Imperial Technical Services sells an adapter (ITSRBPMA2, about $390 kit) that lets a Rain Master ProMax remote operate Rain Bird controllers through the 5-pin and 6-pin remote ports. It draws power through the controller's transformer and works with 1 to 240 stations depending on model. | documented | [ProMax adapter sheet](https://www.rainbird.com/sites/default/files/media/documents/2025-11/itsrbpma2.pdf) | 2026-11-02 |
| RB-031 | Rain Bird's sales page says the full technical specifications of that adapter are available on request from a Rain Bird sales representative. This is a route to real signal information without measuring. | documented | [sales sheet](https://www.rainbird.com/sites/default/files/media/documents/2025-12/flatened_2025-11-10_rain_bird_promax_remote_adapter_sales_sheet.pdf) | 2026-11-02 |
| RB-032 | Rain Bird's own wireless-valve patent application describes a universal configuration that connects to station output terminals with an AC input, and says that in some embodiments the interface needs no special protocol. This supports the in-line route working with any controller. | documented | [US 2020/0367452](https://uspto.report/patent/app/20200367452) | 2026-11-02 |
| RB-033 | Rain Bird patents US 7,400,944 and US 6,782,310 are about battery-powered wireless valve controllers and describe no remote port. | documented | uspto.report | 2026-11-02 |

## Next steps

1. Ask a Rain Bird sales representative for the remote-port technical specification (see [vendor requests](vendor-requests.md)).
2. Log the clock and data pins on a 5-pin and a 6-pin controller with a LIMR receiver and, if available, an LNK module. See [measurement plan](measurement-plan.md).
3. Read the rest of the ESP-RZX thread; its decoding was in progress.
