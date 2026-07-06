# Zerr\* — Project Status & Design Report

_Generated: 2026-07-06 · Branch analysed: `dependency_fix` (see §1 for rename)_

> Hand-authored design/status docs live under `docs/design/`.
> Auto-generated API docs (Doxygen) are emitted to `docs/build/` and are **not** part of this tree.

---

## 1. Current Working Status

**Active branch:** `dependency_fix` — working tree clean, **21 commits ahead of `origin/main`**
(unmerged). It is the most advanced local line of work: `dev`, `improve_deps_manage`,
`windows_compile`, and `max_doc` are all strictly *behind* it (by 51, 51, 34, 60 commits).

**What the branch actually contains** — despite the name, the commits are a **code-quality /
modernization sweep**, not dependency changes:

- Strong-typed enums (`Mode` split into `GenMode` / `TriggerMode`)
- `const` / `noexcept` correctness, removed magic numbers, `const AudioInputs&` to remove copies,
  `move` instead of clear+assign
- Type simplification (`Pair` → `SpeakerPair`, removed unused types)
- Doxygen documentation added (`docs/Doxyfile`, `doxygen-awesome-css` submodule), metadata
  boilerplate stripped from headers

⚠️ **Branch-name vs. content mismatch.** The branch has been re-scoped/renamed to reflect its
actual modernization + docs content (see the branch rename performed alongside this report).

**Branch sprawl:** 6 local + 12 remote branches. `jack` is heavily diverged (**289 ahead /
13 behind main**). Several look abandoned or parallel (`PD_modules_test_Makefiles`, `core_to_lib`,
`add_cmake`). Consolidation recommended.

---

## 2. Build Status — ✅ Fixed (2026-07-06)

**Problem found:** the repo had been relocated (from `…/Documents/workspace/zerr_workspace/Zerr`
to `…/Workspace/zerr-060823/Zerr`). The stale `core/build/CMakeCache.txt` still referenced the old
absolute source path, so any incremental `cmake --build .` failed with
`The source directory "…/zerr_workspace/Zerr/core" does not exist`. Build outputs
(`core/build`, `core/lib`, `libzerr_core.a`) are gitignored, so nothing stale was committed — the
breakage was local build state only.

**Fix applied & verified:**

1. Wiped `core/build` and `core/lib`.
2. `conan install` (conan 2.13.0; deps `fftw/3.3.10`, `yaml-cpp/0.8.0` served from cache).
3. `cmake` configure + build → `libzerr_core.a` compiles **cleanly** against current source.
4. `make install` → reinstalled to `core/lib`.
5. **Downstream check:** rebuilt all 4 PureData externals
   (`zerr_features~`, `zerr_envelopes~`, `zerr_combinator~`, `zerr_disperser~`) — they link
   against the fresh core (arm64, `.pd_darwin`).

**Minor build-hygiene note:** the core objects are compiled for a newer macOS deployment target
(15.0) than pd-lib-builder links against (11.0), producing harmless `ld` warnings. Consider pinning
a common `MACOSX_DEPLOYMENT_TARGET` (or `CMAKE_OSX_DEPLOYMENT_TARGET`) across core and wrappers to
silence them and keep release binaries portable.

---

## 3. Design Overview

Platform-independent C++17 static core (`zerr_core`, namespace `zerr`) wrapped per host environment.

```
Audio In → FeatureExtractor → FeatureBank → FeatureProcessor
                                                  ↓
Speakers ← AudioDisperser ← EnvelopeCombinator ← EnvelopeGenerator
```

Audio features drive spatial distribution — no manual positioning.

**Core subsystems (`core/`):**

- **Features (8):** spectral — Centroid, Rolloff, Flatness, Flux; temporal — RMS,
  ZeroCrossingRate, CrestFactor, ZeroCrossings. All inherit `FeatureExtractor`.
- **Modules (7):** SpeakerManager, FeatureBank, FeatureProcessor, EnvelopeGenerator,
  EnvelopeCombinator, AudioDisperser.
- **Utils:** `types.h`, `configs.h`, FrequencyTransformer (FFTW3 wrapper), RingBuffer,
  OnsetDetector, LinearInterpolator, Logger.

**Type model (`types.h`):** `Sample=double`, `Param=float`, `Index=int`;
`AudioInputs{block, wave, spec}`; strong enums `GenMode{Trigger, Trajectory}` /
`TriggerMode{Random}`; `Position` carries both `Cartesian` and `Spherical`;
`TopoMatrix = map<Index, Indexes>`.

**Dependencies:** Conan 2.x → `fftw/3.3.10`, `yaml-cpp/0.8.0`. Speaker layouts are YAML in
`configs/` (quad_4, ring_8, line_16, ambisonic_21, …).

**Platform wrappers & support (from README):**

| Env                        | Linux | macOS | Windows | Build system            |
| -------------------------- | :---: | :---: | :-----: | ----------------------- |
| **PureData** (4 externals) |   ✅   |   ✅   |    ✅    | pd-lib-builder + Make   |
| **Max/MSP** (5 `mc.zerr.*`)|   ➖   |   ✅   |   🛠️    | Min-DevKit + CMake      |
| **JACK** (skeleton)        |  🛠️   |  🛠️   |   🛠️    | Meson                   |
| **SuperCollider**          |   ⏳   |   ⏳   |    ⏳    | planned                 |

---

## 4. Open WIP Issues & Functional Gaps

**Core algorithm TODOs (functional, not cosmetic):**

- `speakermanager.cpp:454` — `// TODO: seems incorrect` → **flagged likely bug**; needs
  investigation.
- `speakermanager.cpp` — coordinate handling incomplete: `:102` verify Cartesian/Spherical
  consistency, `:120` orientation structure, `:217` equal weighting for spherical.
- `envelopegenerator.cpp` — `:144` envelope interpolator missing, `:197` linear panning should
  become a parameterized crossfade, `:207` a hardcoded `DISTANCE_SCALE` to remove/parameterize.
- `envelopecombinator.cpp:87` — possible bug: using `systemcfg.block_size` "could cause bug
  (sometimes smaller)".

**Feature completeness:**

- **Trajectory mode** envelope interpolation is unimplemented (TODO only).
- **TriggerMode** enum has a single value (`Random`) — selection strategies not yet expanded.
- **PD message interface** for runtime control (set trajectory vector, topological matrix,
  enable/disable speakers) is marked *"still in development"* in `zerr_envelopes~-help.pd`.
- **JACK client** is a 4-file skeleton (`zerr.h/.cpp`, `main.cpp`, `meson.build`), no README —
  earliest-stage wrapper.

**Testing:** no formal test suite; verification is manual via PD/Max help patches. Given the scale
of the current refactor (~1,300 lines changed across 51 files vs. `main`), this is a real
regression risk.

**Dead code:** PD `get_port_count()` in `zerr_combinator.h`/`zerr_disperser.h` marked "remove if
not needed"; empty doc stub at `zerr_envelopes~.cpp:252`.

---

## 5. Recommended Next Steps (priority order)

1. ~~**Fix the build**~~ — ✅ done (§2).
2. ~~**Rename/re-scope `dependency_fix`**~~ — ✅ done; still needs review + merge to `main` before
   it drifts further (and a matching remote-branch update).
3. **Triage `speakermanager.cpp:454`** ("seems incorrect") and the `envelopecombinator.cpp:87`
   block-size concern — these are correctness risks, not polish.
4. **Prune branches** — decide the fate of `jack` (289 ahead) and the stale `origin/PD*`,
   `add_cmake`, `core_to_lib` branches.
5. **Add smoke tests** for the core pipeline to protect the refactor (even a minimal harness given
   "no formal test suite").
