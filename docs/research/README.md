# Research index and evidence conventions

This folder holds everything we know, and do not know, about how existing irrigation remotes talk to sprinkler controllers. Each brand file has a claims table. Read the status column before relying on a row.

## Status labels

| Status | Meaning |
|---|---|
| `confirmed` | We measured it ourselves on real hardware. A capture or photo exists in `captures/` and is linked. |
| `documented` | Stated by the manufacturer or another official source (manual, FCC filing, spec sheet). |
| `reported` | Stated by a third party (forum thread, hobby project, retailer page) and not independently checked. |
| `inferred` | Our own reasoning from other rows. Say which rows. |
| `unknown` | Searched for and not found, or found but unreadable. Keep these rows: they are the research backlog. |

Nothing in this repository is `confirmed` yet. Every row below was read from documents between 2026-10-31 and 2026-11-02, not measured.

## Claim IDs

Each row has an ID such as `HUN-007`. Never reuse or renumber an ID. If a claim is wrong, change its status or add a superseding row and say why. Other files cite claims by ID.

Prefixes: `HUN` Hunter, `RB` Rain Bird, `IRR` Irritrol and Toro, `GEN` cross-brand.

## Rules for adding research

1. Record the source URL, the date you read it, and what you actually saw. A search snippet is not a source: open the page.
2. Summarize in your own words. Do not paste manuals, schematics or long passages. Link instead. Quote at most a short phrase.
3. If a page could not be opened, say so in the row and mark the claim `unknown`.
4. Conflicts between sources stay visible. Add both rows.
5. Add an entry to `docs/research/log.md` for every research session.

## Files

- [Claim status](status.md): counts by status across all files, generated at build time
- [Hunter SmartPort / REM](hunter.md)
- [Rain Bird remote ports](rain-bird.md)
- [Irritrol and Toro remote ports](irritrol-toro.md)
- [Measurement plan](measurement-plan.md)
- [Vendor requests](vendor-requests.md)
- [Research log](log.md)
