<!-- Body of the GitHub release max-v0.3.1 (CI attaches it to the draft). Kept after
     publishing as the changelog. Process: docs/design/release-process.md -->

Patch release of Zerr* for Max 9 on Apple Silicon (arm64, macOS 11+). No object changes:
patches built with 0.3.0 work unchanged.

## Changed

- Text edits to the overview patcher, the `zerr.disperser~` help patch and the
  `zerr-cross-jump` example.

## Fixed

- The package zip no longer carries a `__MACOSX/` tree or `.DS_Store` files. (#51)
