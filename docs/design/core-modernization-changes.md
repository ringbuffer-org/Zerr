# `core_modernization` vs `main` — Change Comparison Report

_Branch: `core_modernization` · compared against `origin/main` · hand-authored doc under `docs/design/`_

## Overview

**22 commits** transforming the codebase, spanning **52 files** (`+1,443 / −1,272`). The `core/`
library alone: **38 files, `+884 / −1,071`** — a **net reduction of ~187 lines** despite adding
documentation.

**Nature of the change:** this is a **modernization / refactor branch, not a feature branch.** No
audio algorithm was rewritten. When whitespace/reformatting is excluded, the actual logic churn in
`core/src` is modest (`+220 / −263`) — the bulk is C++ modernization, boilerplate removal, and
tooling. There are, however, a few **real semantic changes** (memory ownership, mode typing, a Max
macro-clash fix) flagged in the risk section.

---

## 1. Type System Overhaul (`types.h`)

The type vocabulary was consolidated and hardened:

| Removed | Replaced with |
|---|---|
| `Complex`, `FFTBuffer`, `SpecBuffer` | (FFT now uses `fftw_complex` / `Samples` directly) |
| `Block`, `AudioBuffer`, `AudioBuffers` | unified into `Samples` / `Blocks` |
| `FeatureName` (alias) | `std::string` directly |
| `Mode = std::string` | strong enums `GenMode` / `TriggerMode` |
| `t_featureNames` (C-style PD struct) | removed (was outdated) |
| `Pair` | renamed `SpeakerPair` (clarity) |
| `typedef struct {...} SystemConfigs` | `struct SystemConfigs {...}` |

**New:** `enum class GenMode { Trigger, Trajectory }`, `enum class TriggerMode { Random }`, and a
validating `parseGenMode(const std::string&)` helper that **throws `std::invalid_argument`** on bad
input — replacing the previous unchecked string comparison.

## 2. Compile-Time Constants (`configs.h`)

All preprocessor macros → **namespaced `inline constexpr`** inside `namespace zerr`:

```cpp
#define PI 3.14159265        →   inline constexpr double pi = 3.14159265;
#define AUDIO_BUFFER_SIZE... →   inline constexpr std::size_t AUDIO_BUFFER_SIZE = 2048;
```

- **`PI` → `pi`**: deliberately lowercased to **avoid a macro clash with the Max/MSP SDK** (a
  genuine Windows/Max build fix, not cosmetic).
- **New extracted constants** replacing magic numbers previously buried in `.cpp` files:
  `DEFAULT_ONSET_DEBOUNCE = 50`, `LOG_FLOOR = 1e-10`, `DEFAULT_ROLLOFF_PERCENT = 0.85`.

## 3. Metadata Boilerplate Removal (all 8 features + base class)

The `FeatureExtractor` base dropped three pure-virtual getters — `get_name()`, `get_category()`,
`get_description()` — and every feature (Centroid, Rolloff, RMS, …) dropped its
`static const std::string name/category/description` members and their accessors. Descriptions were
**moved into Doxygen comments** instead. This is the single largest source of the line reduction
(each feature header shrank ~30–50%).

Also in the base class: the typo **`set_initialize_statue` → `set_initialize_status`** was fixed.

## 4. Memory Ownership: Raw Pointers → RAII ⚠️ *(semantic change)*

The most meaningful behavioral change. `EnvelopeGenerator` and `SpeakerManager`/`Speaker`:

```cpp
// before
Logger* logger;               logger = new Logger();
SpeakerManager* speakerManager;
OnsetDetector* onsetDetector;
~EnvelopeGenerator();         // manual cleanup

// after
Logger logger;                                    // value member
std::unique_ptr<SpeakerManager> speakerManager;   // owned
std::unique_ptr<OnsetDetector>  onsetDetector;
~EnvelopeGenerator() = default;                   // RAII
```

This eliminates manual `new`/`delete` and closes potential leaks. A code comment now flags that
**member declaration order matters** (config members must precede the objects that depend on them).

## 5. Const-Correctness, `noexcept`, Pass-by-Reference

- Getters marked `const noexcept` (e.g. `is_initialized()`, `get_frame_size()`).
- Hot-path signatures changed `fetch(AudioInputs)` → **`fetch(const AudioInputs&)`** across the base
  class and all features — removes a per-block copy of the audio input struct.
- `EnvelopeGenerator(..., Mode)` → `(..., GenMode)`; `getIndexesByTrigger(Param, Mode)` →
  `(Param, TriggerMode)`.

## 6. Modern C++ Idioms (mechanical, behavior-preserving)

- **Raw index loops → range-for**: `for (size_t i...) actvSpkIdx[i]` → `for (auto idx : actvSpkIdx)`.
- **Structured bindings**: `for (auto& [key, spkr] : speakers)` replacing `.first`/`.second`.
- **`typedef` → `using`**; **`#define` → `constexpr`**; **C-cast/raw ptr → `static_cast`**.
- Member-initializer lists instead of assignment in constructor bodies
  (`: index(index), position(position)`).
- `clear()+assign` → `move`.

## 7. Tooling & Infrastructure

| Area | Change |
|---|---|
| **Doxygen** | New `docs/Doxyfile`, `docs/header.html`, `doxygen-awesome-css` submodule; docs reorganized under `docs/` |
| **CI** | New `.github/workflows/deploy-doxygen.yml` — auto-builds & deploys API docs to **GitHub Pages** on push to `main` touching `core/include/**` or `docs/**` |
| **build.sh** | Added **`-c` clean flag** with per-target `clean_*` functions, added `core` as an explicit target, and target-name validation |
| **.gitignore** | Ignore `docs/build/` (generated docs) |
| **docs/design** | The project status report added previously |

## 8. Wrapper Adaptations

The PureData and Max/MSP wrappers were updated to match the new core API (enum modes instead of
strings, removed metadata getters, const-ref signatures): `puredata/src/zerr_features~.cpp`
(105 lines), `zerr_envelopes.cpp`, `zerr_features.cpp/.h`,
`maxmsp/.../zerr_envelopes.hpp`, and the `zerr_envelopes~-help.pd` help patch.

---

## Behavioral Impact & Risk Assessment

| Change | Behavior risk |
|---|---|
| Pointer → `unique_ptr`/value (RAII) | **Low-positive** — fixes lifetime/leak risk; declaration order now load-bearing |
| `Mode` string → strong enums + `parseGenMode` throwing | **Low** — adds validation (new throw path where invalid mode strings were previously silently accepted) |
| `PI` → `pi`, `#define` → `constexpr` | **Positive** — resolves Max SDK macro clash |
| `fetch` by const-ref | **None** — pure optimization |
| Metadata getter removal | **API change** — anything calling `get_name()`/etc. no longer compiles (wrappers already updated) |
| Range-for / structured bindings / formatting | **None** — behavior-preserving |

**No formal test suite exists**, so correctness rests on manual help-patch testing. Already verified
during the build fix: **core compiles cleanly** against this branch and **all 4 PureData externals
link and build**.

---

**Bottom line:** a disciplined, net-negative-LOC modernization pass — stronger types, RAII memory
safety, const-correctness, constants over macros, and a full Doxygen/CI documentation pipeline —
with three small but real semantic improvements (RAII ownership, validated enum modes, Max
macro-clash fix) and no intended algorithmic changes.
