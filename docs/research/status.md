# Claim status

Counts of research claims by status, generated from the claims tables each time the site is built (`tools/claims.py`). A claim with a combined status such as `documented / inferred` counts under both labels, so a row's counts can add up to more than its number of claims.

<!-- claims-status-table -->

## Reading the table

- <span class="claim-status claim-confirmed">confirmed</span>: measured by this project, with a capture or photo in `captures/`.
- <span class="claim-status claim-documented">documented</span>: stated by the manufacturer or another official source.
- <span class="claim-status claim-reported">reported</span>: stated by a third party and not checked.
- <span class="claim-status claim-inferred">inferred</span>: our own reasoning from other rows.
- <span class="claim-status claim-unknown">unknown</span>: searched for and not found. These rows are the research backlog.

The build fails if a claims table has a status outside this list, an unknown ID prefix, or a duplicate ID. Run `python tools/claims.py --check` to check locally.
