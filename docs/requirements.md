# Requirements (v1 proposal)

Status: proposal. Nothing is built or tested. Every item is open to change by Joel and the friend.

## Scope agreed so far

- Residential-scale irrigation.
- One prototype unit, for the friend's company only. Not for sale.
- Do the job of a commercial irrigation remote at a lower cost. Also a learning project for Joel.
- One-way control is enough. Confirmation that a zone actually started is wanted.
- The friend does not know which controller brands he will meet in the field, so brand coverage matters.

## Functional requirements

| ID | Requirement | Priority |
|---|---|---|
| REQ-01 | Handheld transmitter selects a zone and a run time and starts it. | must |
| REQ-02 | Handheld can stop the running zone. | must |
| REQ-03 | Receiver sits at the valves or controller and switches one zone at a time. | must |
| REQ-04 | v1 supports 8 zones. | must |
| REQ-05 | A master valve or pump-start output is switched with the zone. | should |
| REQ-06 | Receiver sends back a confirmation that the zone output is live (voltage sensing). | should |
| REQ-07 | Receiver sends back a current reading to show a solenoid is actually drawing current. | could |
| REQ-08 | The receiver owns the run timer. If the link or power is lost, the zone stops. | must |
| REQ-09 | Default run time limit and maximum run time are set in the receiver. Proposed: 20 minutes by default, 2 hours maximum. | should |
| REQ-10 | Only one zone runs at a time. | must |
| REQ-11 | Messages are authenticated so another remote cannot start a zone (AES-128 with a rolling counter proposed). | must |
| REQ-12 | A pairing procedure binds one handheld to one receiver. | must |
| REQ-13 | The in-line wiring route works with any 24 VAC controller brand without knowing its remote-port signals. | must |
| REQ-14 | Optional brand adapters for a controller's remote port, added only where the signal is documented and testable. | could |

## Non-functional requirements

| ID | Requirement |
|---|---|
| NFR-01 | Handheld runs from a battery. Target runtime to be set in Phase 1. |
| NFR-02 | The receiver is fused and tolerates controller output up to the normal 24 VAC range. Design the input protection to survive at least 28 VAC. |
| NFR-03 | Use a pre-certified radio module if one fits the cost. If not, a plan to certify the design under FCC Part 15 is needed before any use beyond the prototype. |
| NFR-04 | Range target to be agreed with the friend. Residential lots are the stated scope. |
| NFR-05 | Cost target to be agreed. The goal is well below the cost of commercial irrigation remotes. |
| NFR-06 | Weather resistance of the receiver enclosure. IP rating to be set. |
| NFR-07 | Firmware is written so the protocol and state machines can be unit tested on a PC. |

## Not in v1

- Programming or reading schedules in the controller.
- More than one zone at a time.
- Phone or cloud interface (a later idea; the NUCLEO-WB55RG is 2.4 GHz and is not suited for the field link).
- Selling the product.

## Decisions still needed

See [open questions](open-questions.md).
