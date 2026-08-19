# Zerr\* — Repository Audit: Git State, Dependencies, Outstanding Work

_Generated: 2026-07-30 · Branch analysed: `core_modernization` @ `786c098`_

> Supersedes the branch/commit counts in [`project-status-report.md`](project-status-report.md),
> which still refers to this branch by its former name `dependency_fix` and reports a branch
> inventory that no longer matches the repository (see §1, note).

---

## 1. Git Working Status

|                                   |                                               |
| --------------------------------- | --------------------------------------------- |
| Branch                            | `core_modernization` — working tree **clean**  |
| vs `origin/core_modernization`    | **1 commit ahead** (`786c098`) — unpushed      |
| vs `main`                         | **23 ahead / 0 behind** → clean fast-forward available, never merged |

**Branch inventory** (4 local, 4 remote):

| Branch                | Last commit    | vs `core_modernization` | Verdict                          |
| --------------------- | -------------- | ----------------------- | -------------------------------- |
| `main`                | 2026-02-14     | 23 behind               | stale trunk                      |
| `core_modernization`  | 2026-07-07     | —                       | active                           |
| `improve_deps_manage` | 2026-01-18     | 7 ahead / 53 behind     | **stranded dependency work** (§2.3) |
| `max_doc`             | 2025-11-11     | 1 ahead / 62 behind, remote **[gone]** | deletable         |
| `origin/jack`         | **2023-06-14** | 13 ahead / 312 behind   | archival, pre-refactor tree      |

**Submodules** — all initialized, all on tags. No issues.

| Submodule                  | Version        |
| -------------------------- | -------------- |
| `puredata/pd-lib-builder`  | v0.7.0         |
| `maxmsp/source/min-api`    | v0.6.9-12      |
| `maxmsp/source/min-lib`    | v0.6.0         |
| `docs/doxygen-awesome-css` | v2.4.1         |

**Hooks** — `core.hooksPath=.githooks` is set, so the clang-format `pre-commit` hook is live;
`clang-format` is installed (`/opt/homebrew/opt/llvm/bin/clang-format`).

> **Note — doc drift.** `project-status-report.md` §1 states branch `dependency_fix`, 21 commits
> ahead, "6 local + 12 remote branches", and `jack` at 289 ahead / 13 behind. Measured today:
> `core_modernization`, 23 commits ahead, 4 local + 4 remote, `jack` at 13 ahead / 312 behind.

---

## 2. Dependency Issues

### 2.1 `conan` is not installed — the live blocker

`conan` is absent from `PATH` and is not present via pip or pipx. `build.sh` aborts immediately at
`check_tool conan`, so **no target can be configured from scratch**.

The tree only still builds because of leftover generated state:

- `core/build/` and `puredata/build/` contain prior conan output
  (`conan_toolchain.cmake`, `conandeps.mk`, `FFTW3Config.cmake`, …)
- `~/.conan2/p` still caches the `fftw` and `yaml-cpp` packages

Verified working in that state:

- `cmake --build core/build` → `[100%] Built target zerr_core_static`
- `make` in `puredata/` → up to date; all 4 `.pd_darwin` externals present
- `core/lib/libzerr_core.a` present (311 KB)

Wiping any `build/` directory, or invoking `./build.sh` at all, breaks until conan is reinstalled.

> **Status: partly mitigated** (§4). `build.sh` now checks for `conan` only inside `install_deps`,
> so a tree with a populated root `build/` builds without it — verified. Resolving dependencies
> still requires conan, which remains uninstalled by choice; the recommended command is
> `uv tool install conan==2.13.0`.

### 2.2 Three duplicated conanfiles

`core/conanfile.txt`, `puredata/conanfile.txt`, and `maxmsp/conanfile.txt` each declare an identical
`[requires]` — `fftw/3.3.10`, `yaml-cpp/0.8.0` — and differ only in generator:

| File                     | Generators                     |
| ------------------------ | ------------------------------ |
| `core/conanfile.txt`     | `CMakeDeps`, `CMakeToolchain`  |
| `maxmsp/conanfile.txt`   | `CMakeDeps`, `CMakeToolchain`  |
| `puredata/conanfile.txt` | `MakeDeps`                     |

Three independent `conan install` runs, none pinning a profile → settings drift between wrappers.

That drift is measurable, not theoretical. Via `otool -l`:

| Artifact                     | `minos` |
| ---------------------------- | ------- |
| `core/lib/libzerr_core.a`    | 15.0    |
| `zerr_features~.pd_darwin`   | 11.0    |

So the external advertises macOS 11.0 while carrying code compiled against a 15.0 SDK — a real
portability defect. (pd-lib-builder actually passes `-mmacosx-version-min=10.6`; clang clamps that up
to 11.0, arm64's floor.)

A second, separate cause of the same `ld` warning class: the **prebuilt ConanCenter binaries** were
themselves built for newer targets — ~357 warnings from `libfftw3.a`, more from `libyaml-cpp.a` (13.3).

`os.version` *is* part of the package_id — it was absent from the cached packages' `conaninfo.txt`
only because it was unset in the profile that built them, not because the recipes exclude it. So
pinning a deployment target in the Conan profile changes the package_id, no ConanCenter prebuilt
matches, and every dependency is rebuilt from source. That does eliminate this second warning class,
but at a cost that was judged not worth paying; §4 has the measured comparison. These warnings
therefore remain, by choice: 376 of them, concerning dependency objects rather than our own code.

Note also that the core's own 15.0-vs-11.0 gap was **latent, not broken**: the core includes only
long-stable headers (`<vector>`, `<map>`, `<memory>`, `<random>`, `<sstream>`, …) — no
`<filesystem>`, `<charconv>` or `<format>` — and calls no macOS APIs. The value of pinning the
target is preventing a *future* newer-only libc++ call from compiling silently and failing on an
older mac.

> **Status: addressed** — see §4.

### 2.3 The fix for §2.2 already exists and is stranded

`improve_deps_manage` (7 commits) contains exactly this consolidation:

- single root `conanfile.txt` (moved from `core/`), `maxmsp/` and `puredata/` conanfiles deleted
- new root `CMakeLists.txt` and `puredata/CMakeLists.txt`
- hardcoded conan paths in `puredata/Makefile` replaced with generic, assignable paths
- `core/CMakeLists.txt` extended to resolve system dependencies when conan is absent
- `build.sh` and `puredata/README.md` updated to match

It is **53 commits behind** and touches precisely the files `core_modernization` has since rewritten
— `core/CMakeLists.txt`, `build.sh`, `maxmsp/CMakeLists.txt`, `puredata/Makefile`. A merge will
conflict throughout. This needs **re-application, not a merge**, and it is the work most likely to
be silently lost.

> **Status: partly re-applied by hand, remainder declined.**
>
> - The conanfile consolidation and the generic `puredata/Makefile` paths were re-applied — see §4.
> - The **root `CMakeLists.txt` + `puredata/CMakeLists.txt` superbuild** was assessed and
>   **declined**. It was considered as a way to widen platform coverage and cannot do that: the Pd
>   path is a Makefile needing `mingw32-make`, which breaks under an MSVC generator, and a Windows
>   Max external needs MSVC, which cannot share one CMake configure with a MinGW-built core. One
>   configure means one toolchain. Its remaining value was IDE integration, which did not justify a
>   second build entry point beside `build.sh`.
> - The **`USE_SYSTEM_DEPS` / `find_package_with_help` fallback** was **declined** and replaced by
>   [`dependency-fallbacks.md`](dependency-fallbacks.md), which records which non-conan routes are
>   viable and what each one breaks. An untested second resolve path reintroduces exactly the ABI
>   split §4 closes.
> - Note the branch also carries two regressions against `core_modernization`: unpinned recipe
>   revisions in `conanfile.txt` (breaks under CMake 4, see §4) and
>   `CMAKE_OSX_DEPLOYMENT_TARGET "13.3"` in `maxmsp/CMakeLists.txt` against 10.13 everywhere else.
>
> Nothing further is owed to the branch; it can be deleted.

### 2.4 The JACK target is dead code but still advertised

`jack/` contains only 4 files (`zerr.h`, `zerr.cpp`, `main.cpp`, `meson.build`), yet
`jack/meson.build` lists ~15 sources that do not exist (`src/zerr.cpp`, `src/modules/mapper.cpp`,
`src/features/rms_amplitude.cpp`, `src/modules/trajectorygenerator.cpp`, …).

`jack/zerr.h` includes `audiorouter.h`, `mapper.h`, `trajectorygenerator.h` — module names deleted
from the core. Current `core/include/modules/`: `audiodisperser.h`, `envelopecombinator.h`,
`envelopegenerator.h`, `featurebank.h`, `featureextractor.h`, `featureprocessor.h`,
`speakermanager.h`.

Additional mismatches: never links `zerr_core`; pins `cpp_std=c++11` against a C++17 core; resolves
deps via pkg-config rather than conan. `./build.sh jack` — documented in `README.md` — cannot
succeed. The toolchain is not the problem: meson 1.2.0 and the `jack` pkg-config file are both
present locally.

### 2.5 Max/MSP is unverified against the new core

The 23-commit sweep touched exactly **one** maxmsp file
(`mc.zerr.envelopes_tilde/zerr_envelopes.hpp`) across 5 externals, and there is no Max CI workflow.

A grep of all Max sources for removed/renamed symbols (`t_featureNames`, `Pair`, `Mode::`, bare
`PI`, old `t_*` typedefs) returned **no hits**, so there is no obvious breakage. But
`core-modernization-changes.md` notes that metadata-getter removal means "anything calling
`get_name()` no longer compiles," and nothing had actually compiled the Max side against this core.

> **Status: resolved — it builds.** A clean-slate `./build.sh maxmsp` (2026-07-31, macOS/arm64,
> CMake 4.3.1) builds **all 5 externals with 0 errors** and no compiler warnings from our own
> sources: `mc.zerr.combinator~`, `mc.zerr.disperser~`, `mc.zerr.envelopes~`, `mc.zerr.features~`,
> `zerr.features~`. All five `.mxo` binaries are fresh, arm64, `minos 11.0`. So the modernization
> sweep did not break the Max wrappers despite touching only one file there.
>
> Two caveats found while verifying:
>
> - **`ctest` registers 0 tests.** `min-object-unittest.cmake` is included by every project, but
>   none ship a `*_test.cpp`, so it contributes nothing. The Max side has no automated coverage
>   either — consistent with §3's "no test suite".
> - **The Max externals are never fat binaries.** `maxmsp/CMakeLists.txt:9` gates `C74_BUILD_FAT`
>   on `CMAKE_GENERATOR MATCHES "Xcode"`, and falls back to `CMAKE_OSX_ARCHITECTURES =
>   CMAKE_SYSTEM_PROCESSOR` otherwise. `build.sh` uses the default Unix Makefiles generator, so
>   every build is single-architecture — `lipo -archs` confirms `arm64` only on all five. README.md
>   and CLAUDE.md advertise "fat binaries with Xcode 12+", which is true of the CMake logic but not
>   of anything `build.sh` produces. Distributing universal externals needs either
>   `-G Xcode` or an explicit `-DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"`.

### 2.6 CI never sees this branch

- `build-zerr-core-static-library.yml` — triggers only on `push` to `main`, paths `core/**`
- `build-puredata-plugins-with-pd-lib-builder.yml` — chains off the above via `workflow_run`
  (its own `push` trigger is commented out)
- `deploy-doxygen.yml` — triggers only on `push` to `main`, paths `core/include/**`, `docs/**`

Net effect: 23 commits rewriting the core have **never been built on Linux or Windows** — only
locally on arm64 macOS. No workflow covers Max/MSP or JACK at all. The Windows job in the PureData
workflow also carries two "Set up Conan" blocks, one commented out — leftover experimentation.

### 2.7 Minor

- **`doxygen` not installed locally**, so the docs pipeline this branch added cannot be exercised
  outside CI (which only runs on `main`). `docs/build/html` exists from an earlier run, gitignored.
- **CMake is 4.3.1**; all 5 maxmsp project `CMakeLists.txt` declare
  `cmake_minimum_required(VERSION 3.5)`, `min-lib` declares 3.10. Tested: 3.5 still configures under
  4.3.1 with a deprecation warning only — not broken, but on CMake's removal path.
- **Legacy duplicate** `maxmsp/source/projects/zerr.features_tilde/` alongside
  `mc.zerr.features_tilde/`, with its own copy of `zerr_features.hpp`. It is also the only project
  linking `FFTW3::fftw3` explicitly; its siblings rely on transitive propagation from `zerr_core`.

---

## 3. Outstanding Work

### Correctness risks flagged in code, untriaged

- `core/src/modules/speakermanager.cpp:454` — `// TODO: seems incorrect`
- `core/src/modules/envelopecombinator.cpp:87` — `systemcfg.block_size` "could cause bug (sometimes
  smaller)"

### Incomplete functionality

- `core/src/modules/speakermanager.cpp:102` — Cartesian↔Spherical consistency unchecked;
  `:120` orientation structure unfinished; `:217` spherical weighting not equalized
- `core/src/modules/envelopegenerator.cpp:144` — Trajectory-mode interpolator missing;
  `:197` linear panning should become a parameterized crossfade; `:207` hardcoded `DISTANCE_SCALE`
- `core/src/modules/featurebank.cpp:79` — should be an external function
- `TriggerMode` has a single value (`Random`) — selection strategies not expanded
- PD runtime message interface still marked "in development" in `zerr_envelopes~-help.pd`

### Dead code

- `puredata/include/zerr_combinator.h:54` and `zerr_disperser.h:49` — `get_port_count()`,
  both marked "remove if not needed"
- `puredata/src/zerr_envelopes~.cpp:252` — empty doc stub
- `maxmsp/.../mc.zerr.combinator_tilde.cpp:138` — "TODO: Initialize the ZerrCombinator based on the
  mode"

### No test suite

~1,580 insertions across 53 files, including semantic changes (RAII ownership, throwing enum
parsing, const-ref signatures), validated solely by "core compiles + 4 PureData externals link."
This is the largest unmitigated risk on the branch.

---

## 4. Dependency Handling — Changes Applied (2026-07-31)

Conan was **kept**, deliberately. Three things make it load-bearing and no CMake-native alternative
(FetchContent/CPM) covers them: `puredata/Makefile` consumes a `MakeDeps` `conandeps.mk` and
pd-lib-builder is not CMake; the Windows build cross-compiles via a conan profile; and distributable
externals need static, fat libraries.

Homebrew `fftw` and `yaml-cpp` are installed on the dev machine, which makes a system-dependency
fallback tempting — but brew's `yaml-cpp` ships **dylib only, no `.a`** (and 0.9.0 vs the pinned
0.8.0), and both are arm64-only. Linking them would embed a runtime dylib dependency in every
`.pd_darwin`/`.mxo` and preclude the `x86_64;arm64` fat build. System deps are a local dev escape
hatch, never the release path.

**What changed:**

| Change | Effect |
| --- | --- |
| Root `conanfile.txt`, with `CMakeDeps` + `CMakeToolchain` + `MakeDeps`; per-target conanfiles deleted | One dependency declaration, one install, one profile resolution — removes the drift's root cause |
| `profiles/macos`, `profiles/linux` (both `include(default)`), `puredata/mingw-profile.txt` → `profiles/mingw` | Committed, reviewable settings instead of an ambient per-machine `~/.conan2` default |
| `CMAKE_OSX_DEPLOYMENT_TARGET "11.0"` in `core/CMakeLists.txt` | Our objects are built at the floor the externals advertise, so the compiler rejects a newer-only libc++ API instead of silently shipping an external that fails on older macs. Set in CMake rather than as `os.version` in the profile — see below |
| Pinned recipe revisions in `conanfile.txt` (`fftw#3b4cceb8…`, `yaml-cpp#1aa37121…`) | Both libraries' bundled `CMakeLists.txt` declare `cmake_minimum_required` below 3.5 (fftw 3.0, yaml-cpp 3.4), which **CMake 4.x rejects outright**. These are the ConanCenter revisions that set `CMAKE_POLICY_VERSION_MINIMUM=3.5` as a *cache* variable — the only placement that works, since line 1 executes before the toolchain file loads. Pinned so a resolve cannot regress it |
| `puredata/Makefile`: `ZERR_CONAN_DIR ?= ../build` | Reads the shared root output; overridable, with an actionable error when absent |
| `build.sh`: `install_deps`/`ensure_deps`, new `deps` target, `-c deps` | One root resolve, shared by all targets; `conan` now only required when deps actually need resolving, so a populated `build/` builds without it |
| `build.sh`: `reconfigure_if_stale` | Discards a `core/build`/`maxmsp/build` that would otherwise **silently ignore** the toolchain. Two distinct failure modes, both observed: a pre-consolidation cache ignores a newly passed `CMAKE_TOOLCHAIN_FILE` (CMake only warns "Manually-specified variables were not used"), and a cache older than the toolchain keeps stale values because the toolchain assigns via `set(... CACHE ...)`, which never overrides an existing entry — that one kept `CMAKE_OSX_DEPLOYMENT_TARGET` empty and left objects at the host SDK despite the pin |
| CI: `pip install "conan>=2.13,<3"` | A bare `pip install conan` floated to latest; conan 3.x would have broken CI with no change on our side |
| CI: root install + `-pr:h=profiles/<host>`, toolchain from `$GITHUB_WORKSPACE/build` | CI and local builds resolve identically |
| `puredata/Makefile`: dropped duplicate `-lyaml-cpp` | Removes "ignoring duplicate libraries" from the link |

**Why the deployment target is set in CMake, not in the Conan profile.** Both work; they differ in
cost. `os.version=11.0` in `profiles/macos` additionally rebuilds the dependencies at that target,
which silences all 376 `ld` version-mismatch warnings — but `os.version` is part of the package_id
and ConanCenter builds its binaries with it unset, so no prebuilt would ever match and *every*
dependency would compile from source on every machine and every CI run without a `~/.conan2` cache.
It would also raise the x86_64 floor to 11.0, contradicting the 10.13 `maxmsp/CMakeLists.txt` asks
for in a fat build.

Measured both ways on macOS/arm64:

| | `os.version` in profile | `CMAKE_OSX_DEPLOYMENT_TARGET` in CMake (chosen) |
| --- | --- | --- |
| `libzerr_core.a` | `minos 11.0` | `minos 11.0` |
| Externals | `minos 11.0` | `minos 11.0` |
| Dependency resolution | built from source | all `Cache` / prebuilt |
| `ld` version-mismatch warnings | 0 | 376 (357 `libfftw3.a`, 19 `libyaml-cpp.a`) |
| Errors | 0 | 0 |

The warnings are about *dependency* objects being built for a newer target than the link floor; they
do not affect our code's correctness. For a genuinely warning-free link, do it as an explicit one-off
rather than by changing the profile:
`conan install . --output-folder=build --build=missing -pr:h=profiles/macos -pr:b=profiles/macos -s os.version=11.0`

**Other profile choices worth knowing:** `compiler.cppstd` is inherited, not pinned — the detected
`gnu17` is part of yaml-cpp's package_id, so forcing `17` would invalidate the cached binaries for no
behavioural gain. `compiler.version` is likewise inherited even though the detected value (apple-clang
14) trails the active toolchain (AppleClang 17), because it *is* part of the package_id and
re-detecting discards the cache.

**Verified end-to-end on macOS/arm64, with conan 2.13.0 present:**

- `./build.sh deps` → `conan install` resolves and builds both dependencies from source under
  CMake 4.3.1, all three generators emitted (`conan_toolchain.cmake`, CMakeDeps configs,
  `conandeps.mk`), `Install finished successfully`
- both dependencies resolve from `Cache` (fftw back to its original package_id `aedbe8f5…`)
- `core/lib/libzerr_core.a` → `minos 11.0` (was 15.0), from `core/CMakeLists.txt` alone — the
  toolchain sets no deployment target at all now
- all 4 PureData externals rebuild from scratch → `minos 11.0`, **0** duplicate-library warnings,
  **0** errors, and the 376 dependency version-mismatch warnings documented above
- `reconfigure_if_stale` fires on both failure modes and is idempotent (second run: 0 discards)
- `build.sh deps` fails with an actionable message when `conan` is absent; the `puredata/Makefile`
  guard likewise

- `./build.sh maxmsp` from a clean tree → **all 5 Max externals build, 0 errors**, binaries arm64 /
  `minos 11.0` (see §2.5, which this closes)

**Still unverified:** Linux and Windows/MinGW.

**Note for CI:** prebuilt binaries are reused, so no dependency is compiled from source on the macOS
jobs. One exception is inherent to the pinned recipe revisions: ConanCenter may not publish a binary
for a given revision against `compiler.version=14`, in which case that package is built once and then
cached. A `~/.conan2` cache step would make this a non-issue.

---

## 5. Code Review Findings — Fixed (2026-07-31)

A review of `main...HEAD` plus the uncommitted dependency work raised five findings. All are fixed
and verified; the first was a genuine host-crash bug introduced by the modernization sweep.

**1. Uncaught exception aborted the host — critical.** `Mode` (a validated string) became `GenMode`
plus a throwing `parseGenMode()` (`core/include/utils/types.h:60`). Both wrappers call it inside a
constructor with no handler — `puredata/src/zerr_envelopes.cpp:19` and
`maxmsp/.../zerr_envelopes.hpp:39` — so a typo'd creation argument escaped into the host's C call
stack, reached `std::terminate` and killed the process. Reproduced:
`[zerr_envelopes~ triggerz circulation_8]` →
`libc++abi: terminating due to uncaught exception of type std::invalid_argument` / `Pd: signal 6`,
losing unsaved patches. Before the refactor the same patch merely logged an error.

Fixed by catching at both wrapper boundaries (the pattern `puredata/src/zerr_features.cpp:26`
already used) and failing the way each host expects. `parseGenMode` keeps throwing, which is the
right contract for a library. Verified after the fix:
`error: zerr_envelopes~: Unknown GenMode: triggerz` / `error: ... couldn't create`, host alive;
valid modes (`trigger`, `trajectory`) still create normally.

**2. The new profiles were untracked — high.** `profiles/macos` and `profiles/linux` were written but
never `git add`ed (only `profiles/mingw` was tracked, via `git mv`). Since `build.sh` and both
rewritten workflows pass `-pr:h=profiles/<host>`, any fresh clone or CI run would have failed with
"Profile not found". Now tracked, along with this document.

**3. `clean` aborted mid-run — medium.** Two compounding causes. `clean_puredata` used
`cd … && make clean; cd …`, whose failure status trips `set -e` and exits before the remaining
targets. And the `$(error …)` guard in `puredata/Makefile` fires at *parse* time for every goal, so
`make clean` failed exactly when dependencies were absent — the state `-c deps` creates. Fixed both:
the guard is skipped for clean-only invocations, and the `cd` runs in a subshell with `|| true`.
Verified: `./build.sh -c deps puredata maxmsp` now cleans all three (exit 0), `make clean` works
with no dependencies present, and `make` still errors clearly.

**4. `deploy-doxygen.yml` had no `contents: read` — medium.** Declaring any `permissions` sets every
unlisted scope to `none`, leaving `actions/checkout` without repo read scope. Added.

**5. Deployment target disagreed with the wrappers — low.** The pin was 11.0 while
`maxmsp/CMakeLists.txt` pins 10.13 and the PureData macOS CI job passes
`-mmacosx-version-min=10.13`. Invisible on arm64 (clang clamps everything up to 11.0) but on an Intel
or fat build it reintroduced the same object-version mismatch in the opposite direction. Core now
pins **10.13**, the project's single declared floor; the built archive is still `minos 11.0` on
arm64, so nothing changes on Apple Silicon.

Also fixed: a new `-Wreorder-ctor` warning at `puredata/src/zerr_features.cpp` (mem-init list not in
declaration order — harmless, now silent).

**Re-verified after all fixes, from a fully clean tree:** `./build.sh puredata` and
`./build.sh maxmsp` both succeed from zero — 4 PureData externals, 5 Max externals, 0 errors, 0
warnings from our own sources.

---

## 6. Recommended Order

1. ~~**Restore conan**~~ — ✅ done: `uv tool install conan==2.13.0`, matching the cache version.
2. ~~**Re-apply `improve_deps_manage`'s consolidation**~~ — ✅ done by hand (§4). That branch and
   `max_doc` can now be deleted.
3. ~~**Build the Max externals once**~~ — ✅ done: all 5 build cleanly (§2.5).
4. **Push the branch** — `786c098` plus the dependency work in §4 is still local.
5. **Merge `core_modernization` → `main`** (clean fast-forward) so CI finally builds it on **Linux and
   Windows**, the two platforms still unverified — or temporarily widen the workflow triggers to run
   on the branch first.
6. **Decide `jack`'s fate** — fix it against the current core, or drop it from `build.sh` and
   `README.md` rather than shipping a target that cannot build (§2.4).
7. **Triage** `speakermanager.cpp:454` and `envelopecombinator.cpp:87` (§3); add a minimal smoke
   harness — `ctest` currently registers 0 tests on every platform.
8. **Audit the remaining exception escapes at wrapper boundaries.** §5's finding 1 was one instance of
   a general hazard: any exception crossing into Pd's or Max's C call stack aborts the host. The other
   throw in the core is `SpeakerManager::getSpeakerByIndex` (`speakermanager.cpp:169`,
   `std::out_of_range`). All four of its call sites are internal and pass indices derived from the
   manager's own maps, and the Pd wrapper validates user-supplied indices, so it is not obviously
   reachable — but if it ever fires during `perform` it kills the host with no error message. Worth
   confirming, and worth a rule that wrapper entry points never let an exception through.
8. **Decide on fat Max binaries** — `build.sh maxmsp` produces arm64-only, contradicting what
   README.md advertises (§2.5).
