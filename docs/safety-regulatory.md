# Safety and regulatory notes

This is a working note, not legal or compliance advice. Have a qualified person check anything before the product is used outside the prototype.

## Electrical safety

- The receiver sits on the 24 VAC side only. The prototype never touches mains wiring.
- Fuse the 24 VAC input. TRC documents damage above 28 VAC and a 3 A resetting fuse in its universal receiver; use these as design reference points, not as proof.
- Valve solenoids are inductive. Add suppression across relay contacts and coils as needed, and test with real solenoids.
- Fail safe: loss of radio link or power must end the run (REQ-08). Test this explicitly.
- Wet outdoor environment: enclosure rating, cable glands, and conformal coating to be decided.

## Radio regulation (United States)

- The target band is 902 to 928 MHz, the ISM band covered by FCC Part 15.
- Using a pre-certified module may reduce the testing needed, but integrating one into a product can still need the integrator's own evaluation. Confirm with a test lab.
- The prototype is one unit for the company's own use. That is not a reason to skip checking the rules; record what was checked and what was assumed.
- Do not sell or give units to others until compliance is settled.
- Controllers from other brands use other bands (Hunter ROAM 433.925 MHz, Irritrol 906 to 922 MHz, Toro MURS). Keep the link away from interference there, and note that two 900 MHz systems may share the air.

## Intellectual property

- The friend's company may own the design. Confirm before choosing a licence or making the repository public (Q5, Q6).
- Do not copy code from GPL projects (the OpenSprinkler Hunter firmware is one) into this repository. Read them for understanding and write original code. Log what was read.
- Do not paste vendor manuals into the repository. Summarize and link.
- Be careful about reverse engineering a vendor's port if its licence or terms prohibit it. Note the question; ask the friend.

## Open items

Track each in [open questions](open-questions.md).
