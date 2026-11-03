# ADR 0002: Public repository under the MIT licence

- Status: accepted
- Date: 2026-11-03
- Deciders: Joel Anthony
- Related: [Q5, Q6](../open-questions.md), [safety and regulatory: intellectual property](../safety-regulatory.md#intellectual-property)

## Context

The seed documents assumed the friend's company might own the work and defaulted to a private repository with no licence until ownership was settled (Q5, Q6).

On 2026-11-03 Joel said that the friend's company does not own anything in this project, that the repository should be public, and that the licence will probably be MIT.

## Options considered

1. Private repository, no licence. Safe if ownership were unclear, but it no longer is.
2. Public under MIT. Simple and permissive. Others can reuse the code, including commercially.
3. Public under a copyleft licence (GPL or similar). Keeps derivatives open, but limits how the friend's company or others can build on it.

## Decision

Public GitHub repository `irrigation-remote` under Joel's account, licensed under MIT, copyright Joel Anthony. Joel confirms the MIT choice before the first push.

## Consequences

- Everything committed is public: no keys, tokens, customer sites or personal data. The firmware key stays a placeholder.
- The MIT licence covers this project's own code and text. Linked third-party documents keep their own terms; summaries and links only, as the research rules already say.
- GPL code (for example the OpenSprinkler Hunter firmware) still must not be copied in. Under MIT that matters even more, because the GPL terms would conflict.
- Vendor names appear only to describe compatibility. Nothing suggests endorsement by any vendor.
