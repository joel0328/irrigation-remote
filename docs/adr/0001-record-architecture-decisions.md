# ADR 0001: Record architecture decisions

- Status: accepted
- Date: 2026-11-03
- Deciders: Joel Anthony
- Related: handoff prompt, [versioning](../versioning.md)

## Context

The design is a proposal with many open questions. Decisions will be made over months, partly in conversation with the friend's company, and the reasoning is easy to lose.

## Options considered

1. Keep decisions in the open-questions table only. Short, but it loses the reasoning and the alternatives.
2. One ADR per accepted decision, in the repository beside the docs. A little more writing; the reasoning stays with the code and is versioned with it.

## Decision

Use ADRs in `docs/adr/`, numbered, in the [template](template.md) format. Each accepted decision that changes the architecture, closes an open question or picks a component gets one.

## Consequences

- Open questions link to the ADR that answers them.
- Reversing a decision means writing a new ADR, so the history stays readable.
