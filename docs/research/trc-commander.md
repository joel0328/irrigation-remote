# TRC Commander and DCI

The TRC Commander is the product this project is meant to match at lower cost. It is a handheld FM transmitter plus receivers that attach to irrigation controllers, sold to commercial contractors. This page records what is publicly documented. Read [README](README.md) first for status labels.

## Background

Remote Control Technology (RCT) began its irrigation remote line in 1982. TRC Irrigation Remotes, a Washington-state business formed by private equity investors, bought RCT's irrigation assets, including the Commander and Sidekick FM systems, around 2005 to 2006. RCT kept its industrial wireless business. The Commander manual calls this RCT's 6th generation of remote control products.

## Claims

| ID | Claim | Status | Source | Accessed |
|---|---|---|---|---|
| TRC-001 | Handheld transmitter on one 9 V alkaline battery; sends a proprietary FM signal ("PDTFM digital" on the spec sheet) to receivers, which turn valves on and off. | documented | [Operation and Installation Manual](https://manualmachine.com/trc/commander/22123582-operation-and-installation-manual), [Transmitter spec sheet](https://cdn.shopify.com/s/files/1/1987/5531/files/Commander-Transmitter-v1.pdf) | 2026-10-31 |
| TRC-002 | Spec sheet says Part 95-E and Part 15 compliant, up to 250 zones with the extended-station version. Manual says no FCC operator licence, site survey or base station is needed. | documented | spec sheet, manual | 2026-10-31 |
| TRC-003 | Range claims differ by seller: about 2 miles on one dealer page, up to 5 miles on another and in RCT's own sale announcement. Transmit power quoted as 2 W. Treat as best case line of sight. | reported | [Cannon Water](https://cannonwater.com/rct-01051-001-001-commander-receiver-only-for-rain-bird-modular-5-pin-controllers), [Pool360](https://www.pool360.com/Product/pool-tir-58-1023), [Lawn & Landscape](https://www.lawnandlandscape.com/news/rct-sells-irrigation-division-to-trc-irrigation-remotes) | 2026-10-31 |
| TRC-004 | 9999 field-programmable group codes; up to 199 receivers per group code from one transmitter; random-access station operation; 20-minute default shutoff, adjustable up to 2 hours per station; audible low-battery indicator. | documented | [Sprinkler Warehouse kit page](https://www.sprinklerwarehouse.com/trc-0-1041-commander-32-station-sprinkler-system-permanent-receiver-kit) | 2026-10-31 |
| TRC-005 | Operation is two keystrokes. Pressing the two-digit 99 "Valve Off" key deactivates. Controllers with more than 99 stations need the extended-station transmitter. | documented | manual page 11; [TRC receiver sheet](https://remotecontroltech.com/wp-content/uploads/2018/09/Commander-Rain-Bird-RX-Cut-V1.pdf) | 2026-11-02 |
| TRC-006 | Universal receiver connects to the controller's zone wiring through a 37-pin male D-sub cable: pins 1 to 19 and 21 to 33 are the 32 stations, pin 20 is 24 VAC, pin 34 is master valve or pump, pin 37 is valve common, pins 35 and 36 unused. Cable is 37 conductors of 22 AWG, 300 V rated, about 30 in. | documented | [PCC and QC cut sheet](https://cdn.shopify.com/s/files/1/1987/5531/files/PCC-and-QC-Cut-Sheet-v1.pdf) | 2026-11-02 |
| TRC-007 | PCC is the permanent cable (parts 01278, 01279, 01280 for 12, 24, 32 stations); QC is a temporary cable ending in alligator clips (same station counts). Both fit TRC Commander and Sidekick receivers. | documented | cut sheet above | 2026-11-02 |
| TRC-008 | The receiver takes 24 VAC from the controller through the connector. Transformer output above 28 VAC can cause permanent damage. Solenoid current above 3 A trips a resetting fuse. Do not plug a separate transformer into the receiver connector while it carries controller power. | documented | [TRC Commander Manual](https://remotecontroltech.com/wp-content/uploads/2021/03/TRC-Commander-Manual.pdf) | 2026-11-02 |
| TRC-009 | DCI-1 is an internal connection: a molex receptacle onto 6 pins on the controller's circuit board, with a circular plug through a knockout, for Irritrol MC, Irritrol Dial and Superior Sterling controllers. DCI-2 is the portable plug that twists onto it and mates with a 37-pin clamshell on the receiver. | documented | TRC Commander Manual | 2026-11-02 |
| TRC-010 | TRC describes the DCI products as converting the universal receiver's control logic to the controller's, so the system can run as many stations as the controller has instead of the 12, 24 or 32 limit. | documented | [DCI-1 page](https://remotecontroltech.com/product/dci-1), [DCI-RM page](https://remotecontroltech.com/product/dci-rm-logic-converter-for-rain-master) | 2026-11-02 |
| TRC-011 | DCI parts and list prices seen: DCI-1 01381 $78; DCI-1-H 01386 $76; DCI-2 (Irritrol) 01382 $253; DCI-H (Hunter) 01384 $253; DCI-RM (Rain Master) 01383 $253; DCI-RB5 (Rain Bird 6-pin to 5-pin) 01385 $545; Rain Bird 6-pin interface cable 01387 $107; Weathermatic cable 01388 $107. | documented | [DCI cable list](https://remotecontroltech.com/product-category/accessories/connectors-and-cables/dci-cables) | 2026-11-02 |
| TRC-012 | The DCI pages and manual give no pinouts, signals or power for the logic converters. | unknown | same pages | 2026-11-02 |
| TRC-013 | Permanent receiver cards exist for Rain Bird ESP MC and LX, Irritrol Dial and MC, Superior Sterling, HIT Logic 2 and 3, Griswold IDC, Weathermatic and Hunter. The Irritrol card uses a white-connector cable to a six-pin molex on the controller face. | documented | TRC Commander Manual | 2026-11-02 |
| TRC-014 | A single Commander receiver for Rain Bird modular 5-pin controllers lists at $1,113 at one dealer. | reported | [Cannon Water](https://cannonwater.com/rct-01051-001-001-commander-receiver-only-for-rain-bird-modular-5-pin-controllers) | 2026-10-31 |
| TRC-015 | No FCC ID for any TRC product was found. | unknown | searches | 2026-11-02 |
| TRC-016 | Warranty is stated as three years in the older manual and one year on current listings; the transmitter case is called both aluminium and plastic; the antenna is 6 in or 8 in depending on the page. | reported | multiple | 2026-10-31 |

## Two connection routes

TRC uses two different routes, and the project's architecture depends on telling them apart.

1. **Station-line route (universal receiver).** The receiver switches the controller's zone wires itself through the 37-pin cable. Works with any 24 VAC controller. Limited by the number of physical station lines (12, 24 or 32). This is what the project's v1 design copies. See TRC-006.
2. **Controller-logic route (DCI and receiver cards).** The receiver talks to the controller's own board, and the controller does the switching. No station limit, no wiring to zone terminals, but each brand needs its own interface. See TRC-009 to TRC-013.

## Open questions

- What signals do the DCI logic converters carry? Nothing public (TRC-012). Candidates: measure a converter, or ask TRC.
- Is the DCI-1 six-pin board header the same bus as the Irritrol five-pin header? No source says so.
