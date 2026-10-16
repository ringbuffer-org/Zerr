# Max/MSP health check plan — 2026-10-11

_Deferred plan. Written 2026-10-11 while PRs #19 and #20 (combinator mode) still have open items. Pick this up after those are resolved; re-check the file:line references against the tree at that time, since the wrapper and core lines will have moved._

_Status: not started. Audit findings verified against branch `combinator-runtime-mode` @ f53f8b6._

_Update 2026-10-12: PRs #19, #20 and #21 are merged. The Max objects are now `zerr.envelopes~`,
`zerr.combinator~`, `zerr.disperser~` and `zerr.features~` (no `mc.` prefix; the `mc.` features
variant is deleted), and the source folders are `maxmsp/source/projects/zerr.*_tilde/`. Read every
`mc.zerr.*` path and name below with that rename applied; PR 4's self-cast fix now targets
`zerr.disperser_tilde/zerr.disperser_tilde.cpp` and PR 5 covers one features object, not two._

## Context

A composer friend will use the Max version of Zerr* for an etude soon, on an Apple Silicon Mac.
The goal is a robust, installable Max package: hardened externals, help patches and docs that
match the code, and a zip that installs cleanly with Gatekeeper quarantine instructions.

Decisions taken with the user (2026-10-11):

- Target: Apple Silicon only. No universal binary work.
- Merge PRs #19 and #20 (combinator mode) first, then branch the new work from `main`. The
  combinator listening pass becomes part of this health check.
- Core scope: Max wrappers get full hardening; `core/` gets defensive guards only (no behaviour
  change). Pd externals must keep building. Thread safety between setters and `perform` is a later PR.
- Delivery: ad-hoc signed zip plus `xattr -dr com.apple.quarantine` instructions. No Developer ID.
- Defaults (decided 2026-10-12, board ID 97 "all externals should work together with default
  args"): every object must instantiate from its plain name. `zerr.features~` → `rms`;
  `zerr.envelopes~` → `trajectory` on a compiled-in 8-speaker ring (the circulation_8 positions,
  loaded from a YAML string so it works outside an installed package); `zerr.combinator~` → 2 sets,
  `max`; `zerr.disperser~` already takes no arguments. The plain chain
  `zerr.features~ → zerr.envelopes~ → zerr.combinator~ → zerr.disperser~` must make sound move.

### Where things stand (recap)

**Branches / PRs.** `main` is at the Pages decision (PR #16). `combinator-core-static-mode` (PR #19)
and `combinator-runtime-mode` (PR #20, stacked on #19) are open, mergeable, CI green on all seven
jobs, but never listened to in Max. Working tree: `.gitignore` adds `.claude/` (uncommitted),
`docs/design/paper-gap-audit-2026-09-04.md` is untracked.

**Board.** ID 126 "Max/MSP wraper" is Inspecting. ID 136 (combinator mode) In progress, waiting on
the listening pass. ID 72 (memory leaks), 97 (default args), 100 (spread gain), 95 (JACK) In
progress. 21 paper-gap rows (141-162) Not started. ID 93 flagged as actually open.

**Known-unfixed audio bugs** (memory `audio-runtime-bugs-todo`): envelopes `perform64` can
throw into Max; `EnvelopeGenerator::processFunc` uninitialised; envelopes leaks `x` on failed `new`.

### What the Max audit found (verified in source, 2026-10-11)

Crash class:
1. `mc.zerr.disperser~` casts the raw `t_object*` to the Min class
   (`mc.zerr.disperser_tilde.cpp:46,61`). The real layout is `minwrap<T>{maxobject_header; T m_min_object}`
   (`min-api/include/c74_min_operator_vector.h:106-110`), so every `inputchanged` /
   `multichanneloutputs` call reads and writes the Max object header.
2. `mc.zerr.envelopes~` and both features objects size buffers from `sys_getblksize()` at creation
   and ignore `dsp64`'s `maxvectorsize`/`samplerate` (`mc.zerr.envelopes_tilde.cpp:217,281-285`;
   `zerr_envelopes.hpp:69-70,91,97`; `mc.zerr.features_tilde.cpp:117,173-176`; `zerr_features.hpp:40,90,98`).
   Vector size growth or an upsampled `poly~` overflows the heap. Sample rate is frozen too.
3. Envelopes and features `perform` throw `std::invalid_argument` and are called bare from
   `perform64` (`zerr_envelopes.hpp:86-88` / `:291`; `zerr_features.hpp:84-86` / `:180`).
4. Core: `active del` of the current speaker or `topo del` of all links leaves an empty candidate
   list, indexed via `uniform_int_distribution(0,-1)` (`speakermanager.cpp:159-161,252-259`);
   empty `trajVector` gives `% 0` (`:174-187`); `indexChannelLookup` built once (`envelopegenerator.cpp:55-58`)
   so `active set/del` reads `distances[chnl]` out of range (`:148,154-159`); a YAML with zero
   speakers passes `initialize()` then crashes in trigger mode (`:62`).
5. Core log routed through `post(msg.c_str())` as a format string (`zerr_envelopes.hpp:63`).

Silent-failure class (the composer sees "could not create" with no reason):
- Misspelled feature name: core throws, wrapper swallows, NULL with no message (`zerr_features.hpp:59-63`, `.cpp:120-123`).
- Envelopes `argc != 2` or `initialize()` false: NULL with no message (`.cpp:111-113,219-223`). Any `@attr` breaks argc.
- `print` prints nothing (logInfo below default WARNING level, `logger.cpp:21-29`).
- Disperser: multichannel source → silence, no message (`.cpp:104-107`).
- Features reads names from all `argc` args including `@attr` tokens (`.cpp:102` vs `:112-114`).

Packaging class:
- `./build.sh -i maxmsp` fails: no `install()` rule anywhere under `maxmsp/` (`build.sh:299-301`). CLAUDE.md:48 documents it.
- Nothing produces a Max package. CI uploads `maxmsp/externals` only (`ci.yml:265-271`).
- `maxmsp/help/*.mxo` are hand copies that shadow fresh builds and would duplicate objects in a package.
- `package-info.json` claims x64 + Windows, Max 8 min; folder is `maxmsp` so the package would be
  named "maxmsp"; homepatcher is a one-comment stub with a typo.
- `maxmsp/ReadMe.md` is Min-DevKit template text about nonexistent objects. `package-lock.json` is a stray npm file.
- Per-project CMake comment about transitive FFTW3/yaml-cpp is false; disperser links core needlessly.

Docs/help class:
- Envelopes help labels `interval` as "FadeIn/Out time" (it is the onset debounce). All three inlet
  assists say "Input source signal". Error strings use `current_speaker`/`topomatrix`/`trajectory`/
  `trigger_interval` instead of the message names. Stale `zerr_` underscore names across help patches.
- Disperser help says the source is `noise~`; it is `cycle~ 440`. MIN_RELATED lists nonexistent objects.
- Combinator help: `mode` messages wired to one instance only, `add` arg and `@mode`/`bang`/`chans` unshown.
- README claims MacOS(Intel) ✅ and precompiled builds; CLAUDE.md claims fat binaries and lists FeatureProcessor as live.
- Help patches embed an absolute local path in fpic `pic` fields and off-screen window rects.

## Plan

Eight small PRs, ordered by blast radius: packaging/docs → wrappers → core guards → release.
`./build.sh maxmsp` and `./build.sh puredata` must pass after every PR. Pattern to copy for
wrapper hardening: `maxmsp/source/projects/mc.zerr.combinator_tilde/zerr_combinator.hpp`
(`prepare()` from dsp64, idempotent on unchanged shape; `perform` noexcept; `ready` atomic; `silence()`).

### Step 0 — land the combinator PRs

Merge #19 then #20 into `main` (both green, mergeable). Their Max listening pass is scenario 6 of
the checklist below. Commit the pending `.gitignore` change (`.claude/`) and the untracked
`docs/design/paper-gap-audit-2026-09-04.md` on a tiny docs PR or fold into PR 2.

### PR 1 — installable package: install rule, zip, CI artifact

- `maxmsp/CMakeLists.txt` after the project loop (`:70-75`): read `version` from
  `package-info.json` via `string(JSON)`; default `CMAKE_INSTALL_PREFIX` to `maxmsp/dist/Zerr`
  (same cache-guard idiom as `core/CMakeLists.txt:66-69`); `install(DIRECTORY externals/ …
  USE_SOURCE_PERMISSIONS)` (keeps the ad-hoc signature), `help/` matching `*.maxhelp` only,
  `extras/`, `media/`, root `configs/*.yaml` → `misc/` (Max adds `misc/` to the search path, so
  `path_absolutepath` in `mc.zerr.envelopes_tilde.cpp:149,168,185` resolves `circulation_8` with no
  copy in `help/`; fallback: also install to `help/`), `package-info.json README.md LICENSE` → `.`.
  Warn at configure time if `help/*.mxo` exist.
- Per-project `CMakeLists.txt:30-31` (5 files): fix the false "propagate transitively" comment; link
  only what is used (envelopes: core + yaml-cpp; features ×2: core + FFTW3; combinator: core;
  disperser: nothing). Add `ZERR_MAX_VERSION` compile definition for `bang` reports.
- `build.sh:299-301`: `cmake --install maxmsp/build` then
  `ditto -c -k --sequesterRsrc --keepParent Zerr Zerr-<ver>-macos-arm64.zip`. `clean_maxmsp`
  (`:347-351`) also removes `externals/`, `dist/`, `help/*.mxo`.
- `.github/workflows/ci.yml:258-272`: install + zip + `codesign --verify --deep --strict` +
  `lipo -archs` on each `.mxo`; upload the zip as `zerr-max-macos-arm64`.
- `.gitignore` add `maxmsp/dist/`; `git rm maxmsp/package-lock.json`;
  `git mv maxmsp/help/circulation_8.yaml configs/` (Pd keeps its own copy, `puredata/Makefile:86`);
  locally `rm -rf maxmsp/help/*.mxo`.
- Accept: `./build.sh -c maxmsp && ./build.sh -i maxmsp` exits 0; `dist/Zerr/` has 5 `.mxo`,
  5 `.maxhelp`, no `.mxo` in `help/`, 5 yaml in `misc/`; signatures verify; CI artifact is the zip.

### PR 2 — metadata, user README, overview patch, platform claims

- `maxmsp/package-info.json`: `name: "Zerr"`, `max_version_min: "9.0"` (help patches saved by
  9.0.7), `macintosh.platform: ["aarch64"]`, drop `windows`, bump `version` (propose `0.3.0`; tags
  v0.1/v0.2 exist).
- Replace `maxmsp/ReadMe.md` → `maxmsp/README.md`: what Zerr is + pipeline; requirements (Max 9,
  Apple Silicon native, macOS 11+); install steps including
  `xattr -dr com.apple.quarantine ~/Documents/Max\ 9/Packages/Zerr`; troubleshooting ("unable to
  load object bundle executable" → xattr; "Cannot find YAML file" → search path / `misc/`); YAML
  format; per-object reference (args · inlets · outlets · messages) for all five objects, including
  the combinator's equal-channel-count rule and the envelopes' "speaker must be active to appear in
  traj/topo/curr" rule.
- `maxmsp/extras/Zerr_Overview.maxpat`: real overview (fix typo, one row per object with
  description and a `pcontrol` help opener).
- Root `README.md:9-14,26-28`: Intel and Windows Max → not shipped; point to the zip. `CLAUDE.md:48`
  (`-i maxmsp` assembles `dist/Zerr` + zip), `:69,:80` (FeatureProcessor is not a live stage),
  `:103` (arm64 only; fat only with Xcode generator, untested).
- New `docs/design/max-release-process.md`: maintainer steps + the checklist below.

### PR 3 — correct the five help patches (`maxmsp/help/*.maxhelp`)

Edit in Max 9 (or scripted JSON edits), review by text dump.
- envelopes: "FadeIn/Out time" → "interval: minimum time between jumps in ms (onset debounce),
  trigger mode only"; `mc.zerr_envelopes~` → dotted; add `active set`, `topo add/del`, `bang`,
  `print` examples; comment on the `active del` → `traj` trap; outlet order = YAML order.
- disperser: `noise~` comment → `cycle~`; `[zerr_disperser~]` → dotted; inlet 1 must be mono.
- combinator: wire `mode` messages to both instances; add `2 add` example, `@mode`, `bang`,
  `getchans`; delete stray `mc.+~`.
- features ×2: underscore names, typos.
- All: move window rects on-screen; re-import the logo from a new `maxmsp/media/zerr_logo.png` so
  the absolute `/Users/zeyuyang42/...` path disappears.

### PR 4 — mc.zerr.disperser~ self-cast fix

`maxmsp/source/projects/mc.zerr.disperser_tilde/mc.zerr.disperser_tilde.cpp`
- `:46,:61`: `auto& x = wrapper_find_self<mc_zerr_disperser_tilde>(obj)->m_min_object;`
  (`c74_min_object_wrapper.h:99-111`).
- `:65-68`: return `true` for inlet 0 so `_mc_outputs` re-evaluates; when `count != 1` emit one
  `cerr` line "source inlet expects a mono signal (got N channels); output muted". Keep
  `operator()` silent.
- `:20,:23,:28-29`: description, MIN_RELATED (`mc.zerr.envelopes~, mc.zerr.combinator~,
  mc.zerr.features~`), assist text, inlet 1 type `"multichannelsignal"`.

### PR 5 — features ×2: shared header, prepare() on dsp64, noexcept perform, arg validation

_Status 2026-10-12: implemented as PR #28 on the single `zerr.features~` (the mc. variant is gone, so no
shared header); the valid-name list comes from a new `FeatureBank::availableFeatures()` in core.
`zerr_max_log.hpp` moves to PR 6, the first user of it._

- New `maxmsp/source/shared/zerr_features.hpp` replacing the two identical copies; new
  `shared/zerr_max_log.hpp` (core→Max printer: strip `[ERROR] `/`[WARNING] `/`[INFO] ` written by
  `logger.cpp:68-76`, route to `object_error/object_warn/object_post(owner, "%s", body)`);
  `target_include_directories` per project.
- `ZerrFeatures::prepare(cfg, err*)`: build a **fresh** `FeatureBank` each time (`initialize`
  appends to `activated_features`, `featurebank.cpp:31-35`), try/catch, size `inputBuffer` to
  `cfg.block_size`, swap under `ready`. Called in `new` (so a misspelled name fails creation with the
  core's message plus "Valid features: rms zcr flx ctd rlf cf flt zc") and in dsp64 with real
  `samplerate`/`maxvectorsize`.
- `perform` noexcept: `ready`/pointer/count checks, clamp to block_size, zero tail,
  `try { … } catch (...) { silence(); }`.
- **Default argument:** no positional args → feature list `{"rms"}` (one outlet), with an
  `object_post` saying so once. `offset < 1` is no longer an error.
- `.cpp:112-114`: iterate `i < offset` (not `argc`), require `A_SYM`. Assist names the feature per
  channel/outlet. `bang` posts version, features, block, sr.
- Leak-free `new`: `bool dsp_ready` in the struct; every failure → `object_free(x); return NULL`;
  `free` calls `dsp_free` only if `dsp_ready`; `delete` centralised in `free`.

### PR 6 — mc.zerr.envelopes~: prepare() on dsp64, noexcept perform, error routing

_Status 2026-10-12: implemented as PR #29, including the plain-name defaults for envelopes and combinator and
the shared `maxmsp/source/shared/zerr_max_log.hpp` / `zerr_default_layout.hpp`._

Core gets two **additive** methods (no Pd caller changes):
`EnvelopeGenerator::setSystemConfigs(SystemConfigs)` (stores cfg, `assign(block_size)` on every
input/output buffer; message state in `SpeakerManager` is untouched by construction) and
`setLogLevel(LogLevel)` (forwards to own logger and `speakerManager->logger`). Rebuild-and-replay
and wrapper chunking were rejected: the former needs new getters plus an order-sensitive op log,
the latter adds latency and leaves `setTriggerInterval` on the creation sample rate.

- `zerr_envelopes.hpp` rewritten on the combinator pattern: ctor takes `t_object*` and installs the
  printer **before** `initialize()` (today the YAML load error is lost, `logger.cpp:28`); sets INFO
  level so `print` works; `prepare(cfg)` = `setSystemConfigs` + resize wrapper buffers + re-apply a
  cached `interval` ms (so debounce samples follow the new rate); `perform` noexcept as in PR 5.
- **Default arguments:** 0 args → `trajectory` + the compiled-in ring; 1 arg → that mode + the
  compiled-in ring; 2 args → as today. More than 2 positional args is the only count error.
  Core: `SpeakerManager` gains a constructor/loader that takes YAML *text* (`YAML::Load`) beside
  the existing path form (`speakermanager.cpp:63` `YAML::LoadFile`), and `EnvelopeGenerator`
  passes it through; the wrapper embeds the circulation_8 positions as a string constant in
  `maxmsp/source/shared/zerr_default_layout.hpp`. Pd keeps using the path form.
- `.cpp`: more than 2 positional args → error "expects at most 2 arguments: [trigger|trajectory] [config]";
  validate mode with `parseGenMode` before constructing; stop at the first YAML hit (`:148-195`);
  `initialize()` false → "failed to load speaker configuration %s"; dsp64 calls `prepare`; assist
  strings per inlet (trigger/phase, spread, volume); error prefixes `curr:`/`topo:`/`traj:`/
  `interval:` (`:359,:370,:391,:441,:482`); `traj` accepts a single index (`:440`); `bang` via
  `object_post` with version; `dsp_ready` pattern from PR 5.
- `mc.zerr.combinator_tilde.cpp:135,:142,:147,:157,:179`: same `dsp_ready`/`object_free` pattern.
  **Default argument:** no positional args → 2 sets (mode already defaults to `max`);
  `offset < 1` is no longer an error.

### PR 7 — core defensive guards (no behaviour change on non-crashing paths)

_Status 2026-10-12: implemented as PR #30 (stacked on #29); a harness against the built library runs ten
crash scenarios, all guarded._

- `envelopegenerator.h:128` `processFunc = nullptr`; `:130` `triggerMode = TriggerMode::Random`.
  `envelopegenerator.cpp:47-48` drop the shadowing `int` and stray `;`. `:148,:191-192` use
  `find` instead of `operator[]`; `:159` bounds-check `distances[chnl]`; loop bounds use
  `min(input size, output size)`.
- `speakermanager.cpp:58-150` widen the `try` over the whole parse loop (`key.as<int>()` throws
  on a non-integer key); return false with a logged error when `speakers.empty()`.
  `:159-162` `getRandomIndex` empty → `currIdx`; `:174-187` empty `trajVector` → `{currIdx,currIdx}`;
  `:245-278` missing/empty topo row → stay on `currIdx` (silent, audio thread); `:280`
  `getDistanceVector` via `find`; `_setActiveSpeakerIndexs` validates all indices before
  `clear()` (`:523`) and refuses an empty list; `_delActiveSpeakerIndexs` refuses a deletion that
  would empty `actvSpkIdx` or `trajVector`, moves `currIdx` to the first remaining speaker with a
  warning; topo mutators ignore empty lists, no `operator[]` insert for inactive speakers
  (`:397`), warn when a row becomes empty; `_getRandomIndexes` returns `{}` for `l <= 0`.
  `speakermanager.h:311` `currIdx{-1}`.
- `envelopecombinator.h:110-113` comment: root = geometric mean, not RMS.
- Accept: checklist 7-11; `./build.sh puredata` green; open `puredata/help/zerr_envelopes~-help.pd` once.

### PR 8 — release

_Status 2026-10-12: the code part is PR #31 (help-text defaults, draft release notes in
`docs/design/release-notes-0.3.0.md`); the manual steps (overview restyle, Max checklist, tag, GitHub
Release) remain._

**Release notes must open with a "Breaking" section** (user request, 2026-10-12): the Max objects lost
the `mc.` prefix in PR #21 (`mc.zerr.envelopes~` → `zerr.envelopes~`, `mc.zerr.combinator~` →
`zerr.combinator~`, `mc.zerr.disperser~` → `zerr.disperser~`, `mc.zerr.features~` removed in favour
of the single-channel `zerr.features~`), so patches written against the old names no longer
instantiate; and `root` combination mode now computes the geometric mean instead of behaving like
`max` (PR #19).

**Before the zip goes out: the user restyles `maxmsp/extras/Zerr_Overview.maxpat` by hand in Max**
(the PR #22 version works but is plain); commit the re-saved file as-is.

Re-run the full checklist on the built zip, fold any help-text deltas from PRs 4-7 into the help
patches, tag, attach `Zerr-<ver>-macos-arm64.zip` to a GitHub Release, send the README install
section to the composer. Update memory `audio-runtime-bugs-todo` (all three bugs closed) and the
board: ID 136 → Done after scenario 6; ID 126 stays Inspecting until the friend reports back.

## Verification checklist (manual, in Max 9 on Apple Silicon)

Prep every run: `./build.sh -c maxmsp && rm -rf maxmsp/help/*.mxo && ./build.sh -i maxmsp`;
quit Max; remove old `~/Documents/Max 9/Packages/{Zerr,zerr,maxmsp}`; copy `dist/Zerr` there;
start Max; `bang` each object and confirm the Console shows the new version string (stale-binary
detector, see memory `maxmsp-help-stale-external`). Once: download the zip through a browser and
confirm `xattr -l` shows quarantine, then that the README's xattr step fixes loading.

1. Vector size 64→512 with DSP on, each help patch open: no crash; envelopes keep
   `active/traj/topo/curr` (`print` before/after identical).
2. Each object inside `poly~ … up 2` / `down 2`: no crash; `interval 1000` still ≈1 s at the new rate.
3. `mc.zerr.features~ ctd rsm` → one error with the valid list; `mc.zerr.features~ 440` → "must be symbols".
4. Envelopes with 1 arg / unknown mode / missing file / `@foo 1`: one meaningful error each, no crash.
5. Disperser with `mc.pack~ 2` into inlet 0: error, 1-channel silence; back to mono: audio resumes.
6. Combinator `mode root/add/max` while running, both instances: smooth switch; `bang` reports
   mode; unconnected inlet → error + silence. (This is the outstanding #19/#20 listening pass.)
7. Trigger mode: `curr 4`, `active del 4` → warning, triggers continue; `active del 1 … 8` → refused.
8. `topo del 1 2 3 4 5 6 7 8` → warning; `curr 1` + triggers → stays on 1, no crash.
9. `traj 5` accepted; `traj 3 9` → "speaker 9 is not activated", previous traj kept.
10. Empty `standard:` YAML and a YAML with key `a:` → red box with a parse/"no speakers" error.
11. `print` shows active/traj/topo; `active set 100%` prints safely.
12. All five help patches open on-screen, no console errors, every message box matches its comment.
13. Pd regression after PRs 6/7: `./build.sh puredata`, open the Pd envelopes help once.
14. Defaults (board ID 97): in an empty patcher create `zerr.features~`, `zerr.envelopes~`,
    `zerr.combinator~`, `zerr.disperser~` with no arguments; all four instantiate. Chain them
    `cycle~ 220 → features → envelopes (inlet 1) → combinator (inlet 1) → disperser (inlet 2)`,
    `cycle~` also into disperser inlet 1, `→ mc.dac~`; with DSP on the sound moves around the
    8-channel ring as the loudness changes. Then mark ID 97 Done.

## Risks and out of scope

- `prepare()` mutates buffers in dsp64 under the same "chain is not running" assumption the
  combinator already makes (`zerr_combinator.hpp:89-91`). Only mutates on an actual shape change.
- Distance vector is positional over `actvSpkIdx` (`speakermanager.cpp:439-450`) while
  `_processTrigger` indexes by output channel: PR 7 only bounds-checks; spread after `active set`
  stays semantically off (pre-existing, follow-up).
- `EnvelopeGenerator::perform` and `FeatureBank::perform` still return by value (audio-thread
  allocation). Follow-up as PR #19 did for the combinator.
- `misc/` search-path assumption for yaml; fallback is a copy in `help/`.
- Out of scope: setter/perform thread safety; universal/Intel binary; Windows `.mxe64`; Pd parity
  (board ID 156); notarization; porting the disperser off Min; a release workflow.
