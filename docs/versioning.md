# Versioning

Four kinds of thing in this repository change at different speeds, so each has its own tag family. Tags are annotated (`git tag -a`) and say in their message what was checked.

| Thing | Tag family | Example | Rule |
|---|---|---|---|
| Research snapshots | `research-YYYY.MM` | `research-2026.11` | Tag when a research round finishes. Every research change has an entry in [the research log](research/log.md). If two rounds finish in one month, add a suffix: `research-2026.11.2`. |
| Radio protocol | `proto-vMAJOR.MINOR` | `proto-v0.1` | The spec in `docs/protocol/` and the code that implements it share the version. A breaking wire change bumps MAJOR (MINOR while MAJOR is 0). |
| Firmware | `fw-vMAJOR.MINOR.PATCH` | `fw-v0.1.0-dev.1` | Semantic versioning. Nothing gets a release tag until it has run on hardware; before that, use `-dev.N` pre-release tags. |
| Hardware | `hw-revX` | `hw-revA` | One tag per board revision, with fabrication outputs and a bring-up report in `hardware/<board>/`. |

Nothing has been tagged yet.

## Research

- Claim IDs are permanent. Never reuse or renumber one. A wrong claim gets a new status or a superseding row ([conventions](research/README.md)).
- A research change is a commit that touches a brand file **and** adds a log row. Commit type `research:` (see below).
- When a round ends, update the log, check the [claim status page](research/status.md), then tag `research-YYYY.MM` on `main`.

## Radio protocol

- The protocol version lives in `firmware/core/include/irr/version.h` (`IRR_PROTO_MAJOR`, `IRR_PROTO_MINOR`) and in the name of the spec page, for example `docs/protocol/v0.1.md`. Change both in the same commit.
- Each spec page has a status line: `draft`, `implemented` (code and host tests match), or `field-tested` (run between two real boards).
- While MAJOR is 0, any MINOR change may break the wire format, and the firmware accepts only an exact version match. From 1.0, a MINOR change must stay compatible (new optional message types only) and the firmware accepts any MINOR within its MAJOR.
- A new version gets a new page. Old pages stay, marked `superseded`.

## Firmware

- The version is in `firmware/core/include/irr/version.h`. `IRR_FW_DEV` is 1 until a release.
- The receiver reports its firmware version and protocol version in the HELLO reply, so a mismatched pair can be detected ([protocol](protocol/v0.1.md)).
- Release tag rule: `fw-vX.Y.Z` only after the build has run on hardware and the test notes are in the repository. Until then, `fw-vX.Y.Z-dev.N`.
- MAJOR changes when a receiver and handheld from different MAJOR versions can no longer work together. MINOR for new features. PATCH for fixes.

## Hardware

- One folder per board under `hardware/` (for example `hardware/receiver/`, `hardware/handheld/`).
- `hw-revX` tags a released revision: schematic PDF, Gerbers, BOM, pick-and-place, and a bring-up report. Revision letters start at A and skip I and O.
- Bodge wires and rework on an assembled board are written down in that revision's bring-up report, not hidden in the next revision.

## Commits, branches and the changelog

- **Conventional Commits.** Types used here: `feat`, `fix`, `docs`, `research`, `test`, `ci`, `build`, `refactor`, `chore`, `hw`. Scope is optional, for example `feat(firmware): …`, `research(hunter): …`, `hw(receiver): …`. A breaking protocol change adds `!` and a `BREAKING CHANGE:` footer.
- **Branches.** `main` is protected once the repository is shared (pull requests required, CI must pass). After the first commit, changes go through pull requests, even when working alone, so each change has a record and a CI run.
- **Changelog.** `CHANGELOG.md` is kept by hand in [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) style, with an `Unreleased` section at the top. Each tag moves the relevant lines under a heading for that tag.
- **Decisions.** Each accepted architecture decision gets an ADR in `docs/adr/` ([decision records](adr/index.md)).

## Captures

Raw logic-analyzer or scope captures go in `captures/<brand>/<controller-model>/`, each with a metadata YAML beside it (format in the [measurement plan](research/measurement-plan.md) and `captures/TEMPLATE.yaml`). Files over 1 MB go through Git LFS; `.gitattributes` already routes the usual capture formats there.
