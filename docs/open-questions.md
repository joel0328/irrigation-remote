# Open questions

Keep this list current. When a question is answered, move it to the decision column with a date and, if it changes the design, an ADR in `docs/adr/`.

## For Joel and the friend

| # | Question | Why it matters | Decision |
|---|---|---|---|
| Q1 | Power: receiver's own 24 VAC transformer, or draw from the controller? | Controls enclosure, wiring, and whether the receiver runs when the controller is off. | open |
| Q2 | Is a master valve or pump output needed in v1? | Adds a relay and a wiring step. | proposed yes (REQ-05) |
| Q3 | Zone count: 8 for v1, or more? | Sets relay and sense channel count. | proposed 8 |
| Q4 | Range target and cost target | Radio choice, antenna, enclosure. | open |
| Q5 | Repository owner, name and visibility | Friend's company may own the IP. Default to private until agreed. | 2026-11-03, Joel: the friend's company owns nothing here. Public repository `irrigation-remote` on Joel's GitHub account. [ADR 0002](adr/0002-public-repository-mit-licence.md) |
| Q6 | Licence | Depends on who owns the work. Do not pick one until Q5 is settled. | 2026-11-03, Joel: MIT, confirmed. [ADR 0002](adr/0002-public-repository-mit-licence.md) |
| Q7 | Contact vendors for remote-port specifications? | Vendors may decline or ask for an agreement. See [vendor requests](research/vendor-requests.md). | open |
| Q8 | Does the friend want brand adapters, or is the in-line route enough? | Adapters cost research time and may need vendor specs. | open |
| Q9 | Which dev board variant do we have for the radio test? Verify it covers 902 to 928 MHz. | A 868 MHz board will not do for US testing. | 2026-11-03, Joel: no suitable board yet; one must be bought. Still open: which board. Check the vendor's band coverage before buying (ST's site could not be reached on 2026-11-03). Two boards are needed for a link test. |
| Q10 | Commit e-mail address before going public: personal or GitHub no-reply | Commits are public forever once pushed. | open (handoff documents: 2026-11-03, Joel: not needed; removed, never committed) |

## Technical unknowns

| # | Unknown | Route to an answer |
|---|---|---|
| T1 | Rain Bird 5-pin and 6-pin remote-port signals | Vendor request, then capture |
| T2 | Irritrol and Toro RJ-11 and 5-pin signals | FCC internal photos, vendor request, capture |
| T4 | Whether the Hunter controller ever replies on REM | Capture |
| T5 | Brands not yet researched: Rain Master, Weathermatic, Superior and HIT Logic, Griswold, Orbit and other smart controllers | New research files |
| T6 | Real range and link margin in a residential yard | Phase 1 field test |
| T7 | Battery life of the handheld | Phase 1 measurement |
