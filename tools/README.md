# Tools

| File | Purpose |
|---|---|
| `claims.py` | Reads the claim tables in `docs/research/`. `python tools/claims.py` prints counts by status; `--check` fails on a bad status, unknown prefix or duplicate ID. |
| `mkdocs_hooks.py` | MkDocs hook: builds the claim-status table, renders status badges, and stops the build on bad claim tables. |
| `backlog_to_gh.py` | Prints `gh issue create` commands from `backlog/issues.md`. Dry run only; it never calls GitHub itself. |

Protocol decoders for captures (for example a Hunter REM decoder for sigrok or Saleae) will go here once there is a capture to test them against.
