_Pending notes for the next Pure Data release (rule 7 in `docs/design/release-process.md`).
Renamed to `pd-v<ver>.md` when the version in `puredata/zerr-meta.pd` is decided. Lines added
since 2026-10-15 only; everything earlier since `v0.2` still has to be enumerated with
`git log v0.2..HEAD --oneline -- core puredata` before the first `pd-v` tag._

## Changed

- `zerr_envelopes~`: the gain inlet (third) reads 1 when nothing is connected to it; a float
  sent there sets a constant gain, as for any Pd signal inlet. It used to read 0, so a plain
  object with only a control signal was silent.

## Fixed

- The externals no longer trust the block size they were created with. A `[block~]` subpatch of
  any supported size (1 to 2048 samples for `zerr_features~`) now re-sizes the objects when DSP
  starts instead of writing past their buffers (#37–#42).
- `zerr_features~` says why it refuses a block size instead of printing the two numbers:
  the block must be 1 to 2048 samples, the analysis window, and the sample rate non-zero (#43).
  The help patch documents the limit.
