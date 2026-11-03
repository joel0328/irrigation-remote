"""Read the claims tables in docs/research/*.md.

A claim row is a Markdown table row whose first cell is an ID like ``HUN-007``
and whose third cell is a status such as ``documented``. A combined status
like ``documented / inferred`` counts once under each label.

Used by the MkDocs hook (to build the status page) and runnable on its own:

    python tools/claims.py            # print counts
    python tools/claims.py --check    # fail on bad or duplicate IDs
"""

from __future__ import annotations

import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

STATUSES = ("confirmed", "documented", "reported", "inferred", "unknown")
PREFIXES = {
    "HUN": "Hunter",
    "RB": "Rain Bird",
    "IRR": "Irritrol and Toro",
    "GEN": "Cross-brand",
}
ROW = re.compile(r"^\|\s*([A-Z]{2,4})-(\d{3})\s*\|(.*)$")


def parse_row(line: str):
    m = ROW.match(line.strip())
    if not m:
        return None
    prefix, num, rest = m.groups()
    cells = [c.strip() for c in rest.strip().strip("|").split("|")]
    if len(cells) < 2:
        return None
    labels = [s.strip() for s in cells[1].split("/")]
    return f"{prefix}-{num}", prefix, labels


def scan(research_dir: Path):
    """Return (rows, problems). rows: list of (file, id, prefix, labels)."""
    rows, problems, seen = [], [], {}
    for path in sorted(research_dir.glob("*.md")):
        for lineno, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
            parsed = parse_row(line)
            if not parsed:
                continue
            cid, prefix, labels = parsed
            where = f"{path.name}:{lineno}"
            bad = [s for s in labels if s not in STATUSES]
            if bad:
                problems.append(f"{where}: {cid} has unknown status {bad}")
            if prefix not in PREFIXES:
                problems.append(f"{where}: {cid} has unknown prefix {prefix}")
            if cid in seen:
                problems.append(f"{where}: {cid} duplicates {seen[cid]}")
            seen[cid] = where
            rows.append((path.name, cid, prefix, labels))
    return rows, problems


def counts(rows):
    by_prefix = defaultdict(Counter)
    total = Counter()
    for _, _, prefix, labels in rows:
        for s in labels:
            by_prefix[prefix][s] += 1
            total[s] += 1
    return by_prefix, total


def status_table(rows) -> str:
    by_prefix, total = counts(rows)
    head = "| Brand | Claims | " + " | ".join(STATUSES) + " |"
    sep = "|---|---:|" + "---:|" * len(STATUSES)
    lines = [head, sep]
    for prefix, name in PREFIXES.items():
        n = sum(1 for r in rows if r[2] == prefix)
        if not n:
            continue
        cells = " | ".join(str(by_prefix[prefix][s]) for s in STATUSES)
        lines.append(f"| {name} (`{prefix}`) | {n} | {cells} |")
    cells = " | ".join(f"**{total[s]}**" for s in STATUSES)
    lines.append(f"| **All** | **{len(rows)}** | {cells} |")
    return "\n".join(lines)


def main(argv):
    research = Path(__file__).resolve().parent.parent / "docs" / "research"
    rows, problems = scan(research)
    print(status_table(rows))
    for p in problems:
        print("ERROR:", p, file=sys.stderr)
    return 1 if (problems and "--check" in argv) else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
