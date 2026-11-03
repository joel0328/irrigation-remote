"""MkDocs hooks for the Irrigation Remote site.

1. Replace the marker ``<!-- claims-status-table -->`` with a table that counts
   claims by status across every research file (see tools/claims.py).
2. On research pages, render status cells as coloured badges.
3. Fail the build if a claims table has a bad status, prefix or duplicate ID.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import claims  # noqa: E402

MARKER = "<!-- claims-status-table -->"
STATUS_CELL = re.compile(
    r"\|\s*((?:confirmed|documented|reported|inferred|unknown)"
    r"(?:\s*/\s*(?:confirmed|documented|reported|inferred|unknown))*)\s*\|"
)


def _badge(label: str) -> str:
    return f'<span class="claim-status claim-{label}">{label}</span>'


def _badges(match: re.Match) -> str:
    labels = [s.strip() for s in match.group(1).split("/")]
    return "| " + " ".join(_badge(s) for s in labels) + " |"


def on_pre_build(config, **kwargs):
    research = Path(config["docs_dir"]) / "research"
    _, problems = claims.scan(research)
    if problems:
        raise SystemExit("Claims table errors:\n  " + "\n  ".join(problems))


def on_page_markdown(markdown, page, config, files, **kwargs):
    if MARKER in markdown:
        rows, _ = claims.scan(Path(config["docs_dir"]) / "research")
        markdown = markdown.replace(MARKER, claims.status_table(rows))
    if page.file.src_uri.startswith("research/"):
        out = []
        for line in markdown.splitlines():
            if claims.parse_row(line):
                line = STATUS_CELL.sub(_badges, line, count=1)
            out.append(line)
        markdown = "\n".join(out)
    return markdown
