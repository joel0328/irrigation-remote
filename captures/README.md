# Captures

Raw measurements of real equipment: logic-analyzer files, scope screenshots, photos of the probe setup. Nothing has been captured yet.

Layout: `captures/<brand>/<controller-model>/<YYYY-MM-DD>_<what>.<ext>`, with a metadata file of the same name ending in `.yaml` beside it. Copy [TEMPLATE.yaml](TEMPLATE.yaml).

Large files go through Git LFS automatically (see `.gitattributes`). Run `git lfs install` once per clone.

Procedure and safety: [docs/research/measurement-plan.md](../docs/research/measurement-plan.md). A claim becomes `confirmed` only when a capture here shows it, and the claim row links to the capture.
