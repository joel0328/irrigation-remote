# Contributing

Thanks for looking. This is a small project, so the rules are short.

## Workflow

1. Branch from `main`. Open a pull request; CI must pass. `main` is protected.
2. Use [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/). Types: `feat`, `fix`, `docs`, `research`, `test`, `ci`, `build`, `refactor`, `chore`, `hw`. Example: `research(rainbird): add 6-pin capture from ESP-LXME`.
3. Add a line to `CHANGELOG.md` under `Unreleased` for anything a user of the firmware, hardware or docs would notice.
4. An accepted architecture decision gets an ADR in `docs/adr/`.

Version and tag rules: [docs/versioning.md](docs/versioning.md).

## Research

- Every claim gets an ID, a status, a source link and an access date. Never reuse or renumber an ID.
- Open and read the source yourself. A search snippet is not a source.
- Summarise in your own words and link. Do not paste manuals, schematics or long passages.
- Keep conflicting sources side by side; do not quietly pick one.
- Only a capture or test made for this project is `confirmed`. Put the capture in `captures/` with its metadata YAML (`captures/TEMPLATE.yaml`).
- Add a row to `docs/research/log.md` for every research session.
- Run `python tools/claims.py --check` before committing.

## Code

- The portable library in `firmware/core` makes no hardware calls and has host tests for every behaviour. New behaviour comes with a test.
- Do not copy GPL code (for example the OpenSprinkler Hunter firmware) into this repository. Reading it to understand a protocol is fine; note what you read in the research log, then write original code.
- Never commit keys, tokens or personal data. Keys in firmware are placeholders.
- Mark anything that has not run on hardware as untested in the docs.

## Safety

The project works on the 24 VAC side of irrigation controllers only. Mains wiring is out of scope. Read `docs/safety-regulatory.md` before working on real equipment.

## Docs

```sh
pip install -r requirements-docs.txt
mkdocs build --strict    # fails on broken links and bad claim tables
mkdocs serve             # preview at http://127.0.0.1:8000
```
