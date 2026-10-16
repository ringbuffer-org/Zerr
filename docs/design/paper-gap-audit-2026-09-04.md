# Paper-vs-implementation gap audit — 2026-09-04

_Research report. No code was changed for this audit._

## Purpose

Both published Zerr* papers describe capabilities as either implemented or planned. This audit
checks every such claim against the current tree so that nothing promised in print is silently
missing from the code. Each verified gap became a row on the Zerr* Tasks board (the private task
tracker); the board IDs are recorded in §3.

## Sources

| Short name | Paper |
| --- | --- |
| **I3DA 2023** | Z. Yang, H. von Coler, "Autogenous Spatialization for Arbitrary Loudspeaker Setups", *2023 Immersive and 3D Audio: from Architecture to Automotive (I3DA)*, IEEE. DOI 10.1109/I3DA57090.2023.10289141 |
| **PdMaxCon 2025** | Z. Yang, H. von Coler, "Zerr~ — Autogenous Spatialization in PD and Max", *PdMaxCon 2025* |

Tree audited: branch `combinator-runtime-mode` at `f53f8b6` (two commits ahead of `main`).
Where a claim is only satisfied on an unmerged branch (`jack-revival`, archived `origin/jack`),
that is stated explicitly; the verdict column always describes the audited tree.

## Method

1. Extracted every feature statement from both papers into a checklist (§1, §2).
2. Read the core library, all three wrappers, the shipped configs and help patches, and
   `docs/design/`. Every verdict cites `file:line` in the audited tree.
3. Cross-checked against the existing board rows so that nothing is double-tracked.

Verdicts: **IMPLEMENTED** · **PARTIAL** (present but incomplete, buggy, or not exposed on every
host) · **ABSENT** · **OUT-OF-REPO** (the paper describes something that never lived in this
repository, e.g. a performance patch).

---

## 1. I3DA 2023 claims

| # | Claim (section) | Verdict | Evidence |
| --- | --- | --- | --- |
| A1 | Temporal features: RMS, zero-crossing rate, crest factor (Table I) | IMPLEMENTED | `core/src/features/rootmeansquare.cpp:20-33`, `zerocrossingrate.cpp:21-35`, `crestfactor.cpp:20-39` |
| A2 | Spectral features: flux, centroid, rolloff, flatness (Table II) | IMPLEMENTED | `flux.cpp:19-30`, `centroid.cpp:21-37`, `rolloff.cpp:22-42`, `flatness.cpp:21-43`; all eight extractors registered in `core/src/modules/featurebank.cpp:80-90` |
| A3 | Trigger transformations: zero crossing, local extrema, inflection points, threshold violations (Table III, "all implemented transformations") | PARTIAL | Zero crossing exists as a sample-rate feature (`zerocrossings.cpp:19-30`). The other three are declared only, in `core/include/modules/featureprocessor.h:57-60`; `core/src/modules/featureprocessor.cpp` is an empty file and the header references a type (`t_featureValueList`) that no longer exists. The paper itself (IV.C) says the Feature Processor is left to the host, but Table III still lists them as implemented. |
| A4 | Standard speaker properties: index, Cartesian xyz, spherical az/el/dist, orientation yaw/pitch (Table IV) | PARTIAL | All parsed: `core/src/modules/speakermanager.cpp:78-119`. Orientation is stored and printed only; `getYaw()`/`getPitch()` have no callers, and no shipped YAML sets it (`:120-123`, `// TODO: keep the orientation structure`). |
| A5 | Only one coordinate format needs defining; the other is derived on load (III.B.1) | PARTIAL | Auto-fill works: `speakermanager.cpp:106-109`, `_spherical2cartesian` `:497-508`, `_cartesian2spherical` `:510-519`. Consistency check when both are given is a TODO (`:102-104`); both-missing is a bare `assert` (`:101`) that vanishes in release builds. |
| A6 | "Specific properties" assignable at runtime, manual or algorithmic (III.B.2) | PARTIAL | No generic facility and no YAML key. Three fixed collections are mutable through host messages: active set (`:282-300`), trajectory (`:302-319`), topology (`:321-338`). Only `standard` is read from YAML (`:72`). |
| A7 | Speaker masks — subset of speakers visible to one instance (III.B.2.a) | PARTIAL | Implemented as an active-index list. `_addActiveSpeakerIndexs` (`:557`) does not rebuild the distance matrix, trajectory or topology; `_delActiveSpeakerIndexs` (`:576`) skips the distance matrix. `EnvelopeGenerator::indexChannelLookup` is built once (`core/src/modules/envelopegenerator.cpp:55-58`) and never refreshed; the fix is commented out at `core/include/modules/envelopegenerator.h:153`. The Pd help patch labels the message "(buggy)" (`puredata/help/zerr_envelopes~-help.pd:152`). |
| A8 | Speaker trajectory: sequential chain, algorithmic sort by height/azimuth, manual definition (III.B.2.b) | PARTIAL | Chain mapping `speakermanager.cpp:174-190` and manual `setTrajectoryVector` `:302-319` work. The only sort is by numeric index (`:143`, `:549`); nothing sorts by z, elevation or azimuth. |
| A9 | Speaker topology: per-speaker link list, bidirectional or unidirectional (III.B.2.c) | PARTIAL | Link lists work (`:321-410`, default fully connected `:146-148`). Every mutator writes only `topoMatrix[source]` (`:376`, `:390`, `:407`); there is no bidirectional option. |
| A10 | Trajectory mapping of a 0–1 signal onto the speaker chain (III.C.1.a) | IMPLEMENTED | `envelopegenerator.cpp:171-203` |
| A11 | Interpolation method selectable: linear, hard threshold, other (Fig. 4) | PARTIAL | Linear crossfade only (`envelopegenerator.cpp:197-201`, `// TODO: change to parameterized crossfade`). Hard threshold occurs only in the degenerate same-speaker branch (`:194-196`). `LinearInterpolator` is used solely by the block-rate feature extractors. Trigger mode has no interpolation at all (`:144`, `// TODO: add interpolator`). |
| A12 | Trigger shifting with Random and Nearest jump selection by geometry or topology (III.C.1.b) | PARTIAL | Random over topology neighbours works (`speakermanager.cpp:259`). The Random/Nearest switch is commented out (`:262-274`); `_findNearest` (`:452-466`) is unreachable and carries `// TODO: seems incorrect` (it indexes a full distance row with a candidate-list position). `TriggerMode` has a single enumerator (`core/include/utils/types.h:44-46`) and is hard-coded in `envelopegenerator.cpp:63`. Geometric selection `get_indexs_by_geometry` (`speakermanager.cpp:203-243`) is self-declared obsolete with zero callers. |
| A13 | Spread: normal distribution around the main speaker, variance sets extent (III.C.2.a, Fig. 5) | PARTIAL | Spread is audio-rate and applied in trigger mode (`envelopegenerator.cpp:159`). The law is a `tan`-based linear distance taper (`_calculateGain` `:205-223`) with a hard-coded `DISTANCE_SCALE` (`:207`), not a Gaussian with a variance parameter. In trajectory mode the spread buffer is bound and never read (`:185`). |
| A14 | Overall gain modulated by a signal (III.C.2.b) | IMPLEMENTED | Audio-rate volume inlet, applied per sample in both modes (`:166`, `:195-200`); Pd inlets at `puredata/src/zerr_envelopes~.cpp:105-106` |
| A15 | Envelope Combinator: sum, average, max, k-th root of product (III.D, Eq. 1) | PARTIAL | `add`, `root`, `max` exist and are switchable at runtime through a lock-free atomic (`core/src/modules/envelopecombinator.cpp:51-76`, `:91-147`). There is no average/mean mode (`types.h:64-68`, `tryParseCombMode` `:79-94`). |
| A16 | Audio Disperser: per-speaker multiply (III.E) | IMPLEMENTED | `core/src/modules/audiodisperser.cpp:43-47` |
| A17 | Two encapsulations: standalone JACK client and Pd objects (IV.B) | PARTIAL | Pd: four externals built (`puredata/Makefile:63-72`). JACK: `jack/meson.build:3-30` lists sources that do not exist and `jack/zerr.h:12-15` includes deleted headers; it does not build on this branch. The `jack-revival` branch (`ea88d12`) rebuilds it against the current core but is unmerged. |
| A18 | YAML config loading plus runtime parameter changes via args and messages (IV.B) | IMPLEMENTED | Speaker YAML via yaml-cpp `speakermanager.cpp:58-151`; Pd messages `active`, `curr`, `topo`, `traj`, `interval`, `print` at `puredata/src/zerr_envelopes~.cpp:341-360` |
| A19 | Two config file types: speaker-array config **and** module config (features, mapping strategy) (IV.B) | ABSENT (module config) | Only speaker YAML exists in the tree (`configs/*.yaml`, all `standard.position` only). A module config lived at `origin/jack:configs/zerr_configs/preset1.yaml` and is reintroduced by `jack-revival:jack/config/example.yaml`, neither on the audited branch and neither readable by the Pd/Max externals. |
| A20 | Pd objects `zerr_features~`, `zerr_envelopes~`, `zerr_disperser~`, `zerr_combinator~` (IV.C) | IMPLEMENTED | `puredata/src/*.cpp`, help patches in `puredata/help/` |
| A21 | Feature Processor deliberately provided by the host, not a module (IV.C) | IMPLEMENTED (but documented inconsistently) | Header is dead (`featureprocessor.h:12-13` "set to bypass for now"); `CLAUDE.md` and `README.md` still list FeatureProcessor as a pipeline stage. |
| A22 | Future: more music programming environments; user study (V) | PARTIAL | Max/MSP delivered (see B5). SuperCollider is marked planned in `README.md` with no code. User study: archived board rows 70 and 118–123 record a thesis listening test; not tracked further by decision of the user. |

## 2. PdMaxCon 2025 claims

| # | Claim (section) | Verdict | Evidence |
| --- | --- | --- | --- |
| B1 | Pd package ships externals, help patches, example speaker configs, and a "collection of regularly updated presets" (3.2) | PARTIAL | Externals and four help patches ship (`puredata/Makefile:83-88`). Only `puredata/help/circulation_8.yaml` is installed; `puredata/examples/configs/` holds four more that are not installed. No `presets/` directory exists anywhere; `puredata/README.md:18` describes a `configs/presets` folder that does not exist. |
| B2 | JACK hard-coded mapping strategies translated into presets in Pd and Max (3.1) | ABSENT | No preset files in `puredata/`, `maxmsp/`, or `configs/`. The JACK strategies (trigger-jump mapper with Gaussian spread) exist only on archived `origin/jack:src/modules/mapper.cpp`. |
| B3 | JACK CLI version acknowledged as outdated (3.1) | IMPLEMENTED (as stated) | See A17. Board row ID 95 tracks the revival. |
| B4 | Pd multichannel (0.54+) support is "the highest priority for future development" (3.2) | ABSENT | No `CLASS_MULTICHANNEL`, `signal_setmultiout` or `s_nchans` anywhere under `puredata/`; all four externals use `CLASS_MAINSIGNALIN` with one `t_signal*` per port (`zerr_envelopes~.cpp:310-328`, `zerr_features~.cpp:82-100`, `zerr_combinator~.cpp:85-102`, `zerr_disperser~.cpp:80-97`). |
| B5 | Max externals `mc.zerr.envelopes~`, `mc.zerr.combinator~`, `mc.zerr.disperser~`, `mc.zerr.features~`, `zerr.features~` (3.3) | IMPLEMENTED | Five projects under `maxmsp/source/projects/`, all globbed by `maxmsp/CMakeLists.txt:69-75` |
| B6 | Low-level Max API used for dynamic multichannel adaptation at runtime (3.3, 3.4) | PARTIAL | Raw Max API in four projects; `mc.zerr.disperser~` is Min-API with hand-registered mc callbacks (`mc.zerr.disperser_tilde.cpp:81-96`). Full input-driven adaptation exists in the combinator (`mc.zerr.combinator_tilde.cpp:272-284`, `:294-324`) and disperser (`:44-77`). Envelopes and features have `multichanneloutputs` only, no `inputchanged`, and their `dsp64` ignores sample-rate/block-size changes (`mc.zerr.envelopes_tilde.cpp:281-285`, `mc.zerr.features_tilde.cpp:173-176`). Their inputs are fixed-mono by design, so this is a robustness gap rather than a missing feature. |
| B7 | Each Max external has a detailed help patch (3.3) | IMPLEMENTED | `maxmsp/help/*.maxhelp`, five files |
| B8 | Planned: initialization parameters dynamically adjustable at runtime (3.4) | PARTIAL | Delivered for the combinator mode only (`f53f8b6`; `mc.zerr.combinator_tilde.cpp:250-257`, Max only). Envelopes mode and speaker config are fixed at creation (`puredata/src/zerr_envelopes~.cpp:41-52`, `mc.zerr.envelopes_tilde.cpp:111-113`); feature list of `zerr_features~` is fixed at creation. Board row ID 97 covers default arguments, not runtime change. |
| B9 | Planned: intuitive visual feedback (3.4) | ABSENT | No UI object in any source. Help patches use stock meters only. Board row ID 94 already tracks this. |
| B10 | Planned: tool for loudspeaker configuration file generation (3.4) | ABSENT | No script, patch, or documented workflow; YAMLs are hand-written. `configs/line_16.yaml` contains 11 speakers (its Pd twin is correctly named `line_11.yaml`). |
| B11 | Planned: extensions for other hosts (3.4) | ABSENT | `README.md` lists SuperCollider as planned; no code. |
| B12 | Algorithms demonstrated: vector (azimuth) panning, Cartesian panning, spread modulation, triggered jumps random/sequenced (4.4) | PARTIAL | Vector panning and spread modulation are achievable with the shipped externals. Cartesian panning as drawn (Fig. 8, two speakers on the same axis activated together) needs either a patch-level construction or geometric selection, which is dead code (A12). Sequenced jumps have no code path. None of the four are shipped as example patches: `puredata/examples/` holds one patch, `maxmsp/patchers/` is empty, `maxmsp/extras/Zerr_Overview.maxpat` is a single comment box. |
| B13 | `zerr_envelopes~ trajectory ring` / `trigger ring`; `zerr_combinator~ 2 8 root` (Pd) and `2 root` (Max) (Fig. 1, Fig. 2) | PARTIAL | Envelopes forms work on both hosts. Pd combinator requires three arguments (`puredata/src/zerr_combinator~.cpp:29`), so the Max-style `2 root` fails in Pd; Pd has no runtime `mode` message (`:113`, only `dsp`) and its help lists only `root`/`max`. Max accepts `2 root`, `@mode`, and a live `mode` message. |
| B14 | Send-bus performance patch: synth modules, send modules, spatialization modules with meters and gain/spread sliders (4.2–4.3) | OUT-OF-REPO | Not in the repository. No `s~`/`r~` bus, no OSC or MIDI objects in any `.pd`. |

---

## 3. Gap list → board rows

All rows created 2026-09-04 with status `Not started`. Rows 1–4, 6–14 and 16–21 are sub-items of
the new parent **ID 141 — Paper feature gaps (I3DA 2023 / PdMaxCon 2025)** (itself under
**ID 18 — System development**). Row 5 hangs off **ID 100 — fix spread gain**; row 15 hangs off
**ID 136 — fix argument and add message for mode change**.

### Core algorithm

| # | Board ID | Task | Claim | Key evidence |
| --- | --- | --- | --- | --- |
| 1 | 142 | Nearest trigger-jump mode (geometry or topology) | A12 | `speakermanager.cpp:262-274`, `:452-466`; `types.h:44-46` |
| 2 | 143 | Sequenced trigger-jump mode | B12 | no code path; `envelopegenerator.cpp:189` |
| 3 | 144 | Selectable interpolation curve for trajectory panning | A11 | `envelopegenerator.cpp:197-201` |
| 4 | 145 | Honour the spread inlet in trajectory mode | A13 / B12 | `envelopegenerator.cpp:185` |
| 5 | 146 | Spread law: normal distribution with variance parameter | A13 | `envelopegenerator.cpp:205-223` |
| 6 | 147 | Trajectory auto-sort by height / azimuth | A8 | `speakermanager.cpp:143`, `:549` |
| 7 | 148 | Bidirectional topology links | A9 | `speakermanager.cpp:376`, `:390`, `:407` |
| 8 | 149 | Combinator average (mean) mode | A15 | `types.h:64-68` |
| 9 | 150 | Fix runtime speaker-mask add/del (rebuild distance matrix + channel lookup) | A7 | `speakermanager.cpp:557`, `:576`; `envelopegenerator.cpp:55-58`; `envelopegenerator.h:153` |
| 10 | 151 | Use speaker orientation (yaw/pitch) in envelope generation, or drop it | A4 | `speakermanager.cpp:116-123` |
| 11 | 152 | Validate cartesian vs spherical consistency when both are given | A5 | `speakermanager.cpp:101-104` |
| 12 | 153 | Persist trajectory / topology / mask in the speaker YAML | A6 / A19 | `speakermanager.cpp:72` |
| 13 | 154 | Trigger transformations (local extrema, inflection, threshold): implement or formally drop | A3 | `featureprocessor.h:57-60`, empty `featureprocessor.cpp` |

### Package and wrappers

| # | Board ID | Task | Claim | Key evidence |
| --- | --- | --- | --- | --- |
| 14 | 155 | Pd multichannel signal support (Pd 0.54+) | B4 | no `CLASS_MULTICHANNEL` in `puredata/` |
| 15 | 156 | zerr_combinator~ (Pd): runtime `mode` message, `add` in help, accept `2 root` | B13 / A15 | `zerr_combinator~.cpp:29`, `:113` |
| 16 | 157 | Module-config YAML + preset collection for Pd/Max (translate JACK strategies) | A19 / B1 / B2 | no `presets/`; `puredata/README.md:18` |
| 17 | 158 | Runtime-adjustable initialization parameters (envelopes mode, config reload, feature list) | B8 | `zerr_envelopes~.cpp:41-52`; `mc.zerr.envelopes_tilde.cpp:111-113` |
| 18 | 159 | Loudspeaker configuration file generation tool | B10 | none exists |
| 19 | 160 | Example patches for the four documented algorithms | B12 | `puredata/examples/`, `maxmsp/patchers/` |
| 20 | 161 | Send-bus performance patch added to examples | B14 | not in repo |
| 21 | 162 | SuperCollider (or other host) wrapper | A22 / B11 | `README.md` support matrix |

## 4. Existing rows touched by this audit

| Row | Finding |
| --- | --- |
| ID 93 — Jump interpolation between speakers (Inspecting) | The code has no interpolation in trigger mode (`envelopegenerator.cpp:144`). The `interval` message sets the onset debounce (`envelopegenerator.cpp:106-111`), not a fade. **Recommendation: move back to Not started.** Status left unchanged pending the user's decision. |
| ID 94 — Visual feedbacks? (Not started) | Matches B9; no new row. |
| ID 95 — Make jack version work again (In progress) | Matches A17/B3; `jack-revival` branch is the candidate fix. No new row. |
| ID 97 — add default args for externals (In progress) | Adjacent to B8 but about defaults; row 17 covers runtime change. |
| ID 100 — fix spread gain (In progress) | Row 5 is now its sub-item. |
| ID 136 — fix argument and add message for mode change (In progress) | Row 15 is now its sub-item (Pd parity). |
| Archived 63 / 64 / 65 / 104 / 105 / 113 | Mask, sub-index, topology, current-speaker, onset and debounce messages exist in the code; row 9 reopens the mask behaviour because the shipped implementation is inconsistent. |

## 5. Claimed and delivered

For balance, the claims that hold up fully in the audited tree:

- All eight feature extractors with short-name registration (`rms`, `zcr`, `flx`, `ctd`, `rlf`,
  `cf`, `flt`, `zc`).
- Cartesian and spherical speaker positions with automatic derivation of the missing one.
- Topology `set`/`add`/`del`, manual trajectory vector, active-speaker messages, `curr`,
  `interval`, `print` on both Pd and Max.
- Audio-rate spread and gain inlets on the envelope generator.
- `add` / `root` / `max` combination with a lock-free runtime switch in the core, exposed in Max
  through argument, attribute and message.
- Four Pd externals and five Max externals, each with a help patch; combinator and disperser adapt
  their output channel count to the mc input at runtime.

## 6. Appendix — side findings not tied to a paper claim

Recorded so they are not lost; none became a board row.

- `EnvelopeGenerator::reset()` (`envelopegenerator.h:106`) and `FeatureBank::reset(FeatureNames)`
  (`featurebank.h:64`) are declared but never defined; calling either is a link error.
- `EnvelopeGenerator::initialize` declares a local `int numOutlet` that shadows the member
  (`envelopegenerator.cpp:47`); the member is never set.
- `FrequencyTransformer` has no destructor; `fft_in`, `fft_out` and both plans leak; `ifft()` and
  `fft_output()` have no callers.
- `applyMovingAverage` (`core/src/utils/utils.cpp:49-82`) is dead and divides by the wrong size.
- `OnsetDetector` is a debouncer, not a detector; its doc comment describes amplitude-rise detection
  that the code does not do (`onsetdetector.h:23-25`).
- Pd `zerr_combinator~` allocates `n_inlet-1` inlet slots but frees `n_inlet` (`zerr_combinator~.cpp:45`
  vs `:66`); combinator, disperser and features leak on the `initialize()` failure path.
- An unknown feature name throws from `FeatureBank::_create` (`featurebank.cpp:99-104`) and is
  not caught by the Pd or Max features wrappers, so a typo aborts the host.
- `circulation_8.yaml` is required by both help systems but absent from root `configs/`.
- `#define TESTMODE` is commented out (`configs.h:6`), so every `#ifdef TESTMODE` block is dead.
- Documentation drift: `puredata/README.md` describes a directory layout and long feature names
  that no longer exist; `maxmsp/ReadMe.md` is Min-DevKit boilerplate naming objects that do not
  exist; `CLAUDE.md` lists FeatureProcessor as a live module and claims fat binaries. Per board
  rule, documentation work is not tracked as rows.
