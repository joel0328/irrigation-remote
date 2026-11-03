# Decision records

An architecture decision record (ADR) captures one decision: the context, what was decided, and what follows from it. Add one when a decision is accepted, especially when it closes an [open question](../open-questions.md) or changes the [architecture](../architecture.md).

How to add one: copy the [template](template.md) to `NNNN-short-title.md` with the next number, fill it in, add it to `mkdocs.yml` under Decisions, and list it below. Never renumber an ADR. To reverse a decision, write a new ADR that supersedes the old one, and mark the old one `superseded by NNNN`.

| ADR | Title | Status | Date |
|---|---|---|---|
| [0001](0001-record-architecture-decisions.md) | Record architecture decisions | accepted | 2026-11-03 |
| [0002](0002-public-repository-mit-licence.md) | Public repository under the MIT licence | accepted | 2026-11-03 |

## Proposed decisions waiting for an ADR

These are in the design proposal but not yet accepted by Joel and the friend. Each becomes an ADR when it is.

- In-line (station-line) route as the v1 connection, brand adapters optional.
- STM32WL on both ends, 902 to 928 MHz, LoRa for v1.
- AES-128-CMAC authentication with rolling counters ([protocol v0.1](../protocol/v0.1.md)).
- Receiver-held run timer as the fail-safe; one zone at a time; 8 zones in v1.
- Receiver power source ([Q1](../open-questions.md)).
