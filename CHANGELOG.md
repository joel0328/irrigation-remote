# Changelog

All notable changes to this project are written here by hand. The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/). Tag families and version rules are in [docs/versioning.md](https://github.com/joel0328/irrigation-remote/blob/main/docs/versioning.md).

Nothing has been tagged or released yet.

## Unreleased

### Added

- Repository layout, MkDocs Material documentation site with Mermaid diagrams, and GitHub Actions workflows for the docs and firmware.
- Research from the earlier sessions (2026-10-31 to 2026-11-02): TRC Commander, Hunter, Rain Bird, Irritrol and Toro, with claim IDs and statuses; requirements, architecture, open questions, safety notes, build plan, measurement plan and vendor request drafts.
- Generated claim-status page; the docs build fails on malformed claim tables.
- Versioning scheme for research, protocol, firmware and hardware (`docs/versioning.md`).
- Decision records: ADR 0001 (record decisions), ADR 0002 (public repository, MIT).
- Radio protocol v0.1 draft.
- Portable firmware library (`firmware/core`): AES-128 and AES-128-CMAC, frame encoding and authentication, rolling-counter replay protection, receiver run-timer state machine, receiver and handheld message handling. Host unit tests pass. **Not run on hardware.**
- STM32WL receiver port scaffold: startup code, linker script, board stubs. **Builds only; no drivers.**
- Drafts of the research and engineering backlog (`backlog/issues.md`).
