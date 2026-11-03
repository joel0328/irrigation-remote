# Irrigation Remote

An open, low-cost handheld radio remote for residential irrigation controllers: walk the property, start a zone, check the heads, stop it, move on.

> **Status: research and design.** Nothing has been built or tested on hardware. The portable firmware library passes unit tests on a PC, and that is all so far.

## Why

Irrigation technicians use handheld remotes such as the TRC Commander to run zones without walking back to the controller. They are expensive: one dealer lists a single Commander receiver at over $1,100 ([TRC-014](docs/research/trc-commander.md)). Brand remotes such as Hunter ROAM or Rain Bird LIMR plug into their own brand's controllers. This project aims to do the same job for much less, on whatever 24 VAC controller is on site.

It is also a learning project: sub-GHz radio, embedded security, and taking a board from bench to field.

## How it is meant to work

```
 Handheld ──── 902–928 MHz, authenticated ────▶ Receiver ──▶ zone relays ──▶ valves
 (STM32WL)  ◀──────── acknowledgement ────────  (STM32WL)        ▲
                                                                  │
                                         existing controller's zone wires
```

- **Works with any 24 VAC controller.** The receiver's relays sit in series with the controller's zone wires, so it needs no brand-specific protocol. Adapters for brand remote ports (Hunter SmartPort, Rain Bird, Irritrol) are optional extras, researched in [docs/research](docs/research/).
- **Fails safe.** The receiver holds the run timer. If the radio link or the power is lost, the zone stops on its own.
- **Confirms.** The receiver acknowledges each command and reports whether 24 VAC is actually present on the zone output.
- **Authenticated.** AES-128-CMAC with rolling counters, so another remote, or a recording of yours, cannot start a zone.
- **v1 target:** 8 zones plus a master valve or pump output, one zone at a time.

## What is in the repository

| Folder | Contents | State |
|---|---|---|
| [`docs/`](docs/) | Requirements, architecture, research by brand, protocol spec, decisions. Built into the project site. | Research ongoing |
| [`firmware/core`](firmware/core/) | Portable C library: framing, authentication, replay protection, run timer | Host-tested only |
| [`firmware/port/stm32wl`](firmware/port/stm32wl/) | STM32WL receiver port | Scaffold; drivers are stubs |
| [`hardware/`](hardware/) | KiCad projects, one folder per board | Not started |
| [`captures/`](captures/) | Logic-analyzer captures of brand remote ports, with metadata | None yet |
| [`tools/`](tools/) | Docs build hooks, claim checker, future decoders | |
| [`backlog/`](backlog/) | Drafts of research and engineering issues | |

## Quick start

Docs site (Python 3.10 or newer):

```sh
pip install -r requirements-docs.txt
mkdocs serve
```

Firmware host tests (CMake 3.20+, a C compiler):

```sh
cmake -S firmware -B build/host
cmake --build build/host
ctest --test-dir build/host --output-on-failure
```

More in [docs/firmware.md](docs/firmware.md).

## Research rules

Every factual claim about another vendor's product carries an ID, a status (`confirmed`, `documented`, `reported`, `inferred`, `unknown`), a source and an access date. Only a measurement made by this project counts as `confirmed`. Sources are summarised and linked, never copied. See [docs/research/README.md](docs/research/README.md).

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md). Versioning and tags: [docs/versioning.md](docs/versioning.md).

## Licence

MIT. See [LICENSE](LICENSE).

Brand names belong to their owners and are used only to describe compatibility. This project is not affiliated with or endorsed by TRC, Hunter, Rain Bird, Irritrol, Toro or any other vendor.
