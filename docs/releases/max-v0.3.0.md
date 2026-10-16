<!-- Body of the GitHub release max-v0.3.0 (CI attaches it to the draft). Kept after
     publishing as the changelog. Process: docs/design/release-process.md -->

## Breaking

- **Max object names lost the `mc.` prefix.** `mc.zerr.envelopes~` → `zerr.envelopes~`,
  `mc.zerr.combinator~` → `zerr.combinator~`, `mc.zerr.disperser~` → `zerr.disperser~`.
  `mc.zerr.features~` is removed; `zerr.features~` (one signal outlet per feature) is the only
  features object. Patches using the old names no longer instantiate: retype the object boxes.
  The names printed in the PdMaxCon 2025 paper are the old ones. (#21)
- **`root` combination mode now computes the geometric mean.** The Max combinator used to compute
  `max` whatever its argument said, so `2 root` behaved like `2 max`; patches using `root` now get
  quieter output. Pd always computed the geometric mean. (#19)

## Max package

First release of Zerr* for Max, as a package for Max 9 on Apple Silicon (arm64, macOS 11+).

- `zerr-max-0.3.0-macos-arm64.zip`: unzip, move `Zerr` into `~/Documents/Max 9/Packages`, run
  `xattr -dr com.apple.quarantine` on it (the externals are signed ad hoc, not notarized), restart
  Max. Full steps and troubleshooting in the package README. (#22, #23, #27)
- Four objects with help patches and an overview patcher: `zerr.features~`, `zerr.envelopes~`,
  `zerr.combinator~`, `zerr.disperser~`. Speaker layouts ship in `misc/`.
- Every object works from its plain name: `zerr.features~` extracts `rms`; `zerr.envelopes~` runs
  trajectory mode on a built-in 8-speaker ring; `zerr.combinator~` combines two sets;
  `zerr.disperser~` takes no arguments. (#28, #29)
- `zerr.combinator~` gained a runtime `mode` message and `@mode` attribute. (#20)

- The built-in layout of `zerr.envelopes~` (no layout argument) is now the regular eight-speaker
  ring of `ring_8.yaml`, labelled `built-in ring` in the console; it used to be the irregular
  studio ring from the old `circulation_8.yaml`, which no longer ships with the package.

## Fixes

- `zerr.envelopes~` and `zerr.features~` no longer overflow when the signal vector size grows
  after creation or inside a resampled `poly~`; they adopt the chain's rate and vector size.
  Neither can throw into Max's audio thread any more. (#28, #29)
- `zerr.disperser~` wrote its channel counts into Max's object header on every patch-cord
  change; it now addresses its own instance, re-queries its outlet when the source changes, and
  reports a multichannel source once instead of muting silently. (#26)
- Creation errors are reported: wrong argument count, unknown mode, unreadable or empty layout,
  misspelled feature name. Failed creations no longer leak. `print` on `zerr.envelopes~` prints.
  Core log lines reach the Max Console with the right severity. (#28, #29)
- Core: ordinary messages can no longer crash the host. Deleting the current speaker or every
  link from it, emptying the trajectory, a typo in `active set`, a layout with no speakers or a
  non-integer key, a speaker without a position: each is refused with an error or handled
  silently on the audio thread. (#30)
- `zerr.combinator~` builds the core combinator for real; before #19 the mode argument was parsed
  and discarded. (#19)

## Build and tooling

- `./build.sh -p|-i|-l <puredata|maxmsp>`: stage a package and zip, install by copy into the
  host's user folder, or install by symlink for development; same flags for both hosts. (#24)
- `cmake --install maxmsp/build` assembles the Max package; CI uploads the zip. (#22)

## Known limitations

- Apple Silicon only; no Intel or universal build, no Windows Max build.
- Externals are signed ad hoc, hence the quarantine step.
- Thread safety between control messages and the audio callback is unchanged (control-thread
  setters mutate state the audio thread reads); a later release will address it.
