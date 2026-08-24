# Dependency Fallbacks — What Is Valid, and What Breaks

_Reference. Not a plan of record — nothing here is scheduled work._

**Conan is the only supported way to resolve `fftw` and `yaml-cpp` in this repo.** This document
exists for whoever is later asked to add a non-conan path. Read it before touching any
`find_package` call, the `CONAN_*` variables in `puredata/Makefile`, or `conanfile.txt`.

There is no `USE_SYSTEM_DEPS`-style fallback in the tree by design: an untested second resolve path
silently reintroduces the ABI split that [`repo-audit-2026-07-30.md`](repo-audit-2026-07-30.md) §4
was written to close.

## 1. What conan supplies today

One `conan install` at the repo root produces three consumption surfaces:

| Surface | Generator | Consumed by |
| --- | --- | --- |
| `build/conan_toolchain.cmake` | `CMakeToolchain` | `core/CMakeLists.txt`, `maxmsp/CMakeLists.txt` |
| `build/*-config.cmake` | `CMakeDeps` | the `find_package(FFTW3)` / `find_package(yaml-cpp)` calls in both |
| `build/conandeps.mk` | `MakeDeps` | `puredata/Makefile` |

`puredata/Makefile` consumes exactly five variables from `conandeps.mk`:

```
CONAN_INCLUDE_DIRS_YAML_CPP   CONAN_LIB_DIRS_YAML_CPP   CONAN_LIBS_YAML_CPP
CONAN_INCLUDE_DIRS_FFTW       CONAN_LIB_DIRS_FFTW
```

**A fallback must cover all three surfaces.** The Pd wrapper is the hard one: pd-lib-builder has no
`find_package`, so it needs literal `-I` / `-L` / `-l` strings. Any proposal that only handles the
CMake side leaves the Pd externals resolving from a different provider than the core they link —
which is the exact failure mode the root `conanfile.txt` exists to prevent (§3.8).

## 2. Viable fallbacks, ranked

| Option | CMake surface | Makefile surface | Verdict |
| --- | --- | --- | --- |
| **vcpkg toolchain file** | drop-in replacement for `conan_toolchain.cmake` + config files | none — hand-write the `-I/-L/-l` | Best CMake story, still needs Makefile work |
| **pkg-config** | `pkg_check_modules` | `$(shell pkg-config --cflags yaml-cpp)` | Only option that maps *cleanly* onto pd-lib-builder. `yaml-cpp.pc` and `fftw3.pc` both ship |
| **System packages + `find_package(CONFIG)`** | homebrew / apt / vcpkg configs | hardcoded paths or pkg-config | Workable on Linux; on macOS collides with §3.3 and §3.4 |
| **`FetchContent` / `add_subdirectory` from source** | builds in-tree | in-tree paths | **Avoid** — hits the CMake 4 wall (§3.5) |

## 3. What breaks

### 3.1 Target names are not stable across providers

Verified against the generated files in `build/` for the pinned revisions:

| Provider | yaml-cpp targets | FFTW targets |
| --- | --- | --- |
| conan `yaml-cpp/0.8.0` (CMakeDeps) | `yaml-cpp` **and** `yaml-cpp::yaml-cpp` | — |
| upstream yaml-cpp 0.8 config | `yaml-cpp::yaml-cpp` only | — |
| conan `fftw/3.3.10` (CMakeDeps) | — | package aggregate `fftw::fftw`; components `FFTW3::fftw3`, `FFTW3::fftw3f`, `FFTW3::fftw3l` |
| various `FindFFTW` modules | — | `FFTW3::fftw3` or `fftw::fftw`, inconsistently |

Note the shape difference: conan's fftw recipe exposes the *package* as `fftw::fftw` but the usable
libraries as `FFTW3::*` components — the namespaces do not match, which is the trap. Reproduce the
list on any resolve with:

```bash
grep -rhoE "add_library\([A-Za-z0-9_:.-]+ INTERFACE IMPORTED\)" build/*.cmake | grep -v _DEPS_TARGET
grep -rhoE "FFTW3::[A-Za-z0-9]+" build/*.cmake | sort -u
```

Current call sites, both of which must be audited together when the provider changes:

- `core/CMakeLists.txt` — `target_link_libraries(zerr_core_static PUBLIC yaml-cpp FFTW3::fftw3)`
- `maxmsp/source/projects/*/CMakeLists.txt` — `target_link_libraries(… ${ZERR_LIBRARY} yaml-cpp FFTW3::fftw3)`

> **`fftw3` vs `fftw3f`.** `Sample` is `double` (`core/include/utils/types.h`), so this must be the
> double-precision library. Conan exposes `FFTW3::fftw3f` (single) and `FFTW3::fftw3l` (long double)
> as siblings of the one we want, so a one-character slip links cleanly and then produces silent
> garbage in the spectral features rather than an error.

### 3.2 `YAML_CPP_STATIC_DEFINE`

Set in **two separate places** — `core/CMakeLists.txt` (`add_definitions`) and `puredata/Makefile`
(`cflags`). It is required when linking yaml-cpp **statically**; without it the headers declare
`dllimport` on Windows.

If a fallback provides a **shared** yaml-cpp, this define must be removed from both. Change one and
not the other and you get a link failure in whichever target you forgot.

### 3.3 Static vs shared

The build products are loadable bundles (`.pd_darwin`, `.mxo`, `.dll`). A shared `fftw` or
`yaml-cpp` becomes a runtime dependency the end user must install, and on macOS an `@rpath` that
will not resolve inside Pd or Max. **Keep both static.** Homebrew ships shared by default — this is
the most likely way a well-meaning fallback breaks end users rather than the build.

Check for accidental shared linkage on a built external:

```bash
otool -L puredata/zerr_features~.pd_darwin | grep -Ei "yaml|fftw"   # expect no output
```

### 3.4 macOS deployment target

`core/CMakeLists.txt` pins `CMAKE_OSX_DEPLOYMENT_TARGET` to 10.13 — read the comment there, it
explains why the value must not be raised in isolation. Homebrew binaries are built for the host's
macOS version, so linking them reintroduces the exact `minos` mismatch that made this a problem in
the first place.

Verify after any dependency change:

```bash
otool -l core/lib/libzerr_core.a | grep -A4 LC_BUILD_VERSION | grep minos | sort -u
```

Expect `minos 11.0` on Apple Silicon — clang clamps 10.13 up to arm64's own floor. `15.0` means the
pin is not reaching the compile.

> Do **not** pin `os.version` in the conan profile instead. It is part of the `package_id`, so no
> ConanCenter prebuilt would ever match and every dependency would rebuild from source, in CI too.

Note that the prebuilt yaml-cpp in the conan cache is itself built at a newer floor, so the link step
emits `object file … was built for newer 'macOS' version (15.0) than being linked (11.0)` warnings.
Those are expected and come from upstream's binary, not from this project's settings.

### 3.5 CMake 4 rejects the bundled `CMakeLists.txt`

`conanfile.txt` pins recipe revisions (`fftw#3b4cceb8…`, `yaml-cpp#1aa37121…`) because both
libraries' own `CMakeLists.txt` declare `cmake_minimum_required` below 3.5, which CMake 4.x removed
support for. The pinned ConanCenter revisions set `CMAKE_POLICY_VERSION_MINIMUM=3.5` as a *cache*
variable — the only placement that works, since line 1 runs before the toolchain file loads.

Any from-source fallback — `FetchContent`, vcpkg building from source, an unpatched distro package —
hits the same wall. This is the main reason option 4 in §2 is not viable.

### 3.6 Architecture

`maxmsp/CMakeLists.txt` sets `CMAKE_OSX_ARCHITECTURES` to `x86_64;arm64` when `C74_BUILD_FAT` is on
(Xcode 12+). Homebrew and vcpkg provide single-arch libraries, so a fat build fails to link the
missing slice. Conan resolves per-arch; a fat build needs both resolved.

### 3.7 Windows has two incompatible toolchains

- **PureData** builds with MinGW-w64 gcc 13 / `libstdc++11` via `profiles/mingw`, driven by
  `mingw32-make`.
- **Max/MSP** needs MSVC to produce `.mxe64` — a different runtime and C++ ABI.

These cannot share one conan resolve or one CMake configure. `build.sh` refuses `maxmsp` outright on
Windows for this reason, and it is also why there is no CMake superbuild in this repo: one configure
means one toolchain.

`profiles/mingw` pins `tools.build:compiler_executables` to the `x86_64-w64-mingw32-*` names. The
core CI job used to bypass the profile with inline settings because that pin was untested on a
runner; it no longer does. Since the `ci.yml` restructure both the core and the PureData jobs
resolve through `profiles/mingw`, and the pinned names do resolve — verified in run 32297202438,
which logged `tools.build:compiler_executables={'c': 'x86_64-w64-mingw32-gcc', ...}` on a green
Windows core build.

One caveat that survives: the profile declares `compiler.version=13` while the runners now carry
gcc 16.1.0. It works because the `libstdc++11` ABI is stable across those versions, but the
declared version is drifting from the real one, and `compiler.version` is a `package_id` input — so
nothing in the resolve notices when the runner's toolchain moves. `.github/actions/setup-deps`
compensates by keying its conan cache on `ImageVersion`, which turns a runner toolchain bump into a
cache miss rather than a silent reuse of libraries built by the previous compiler.

### 3.8 Core and wrappers must come from one resolve

This is the invariant everything else serves. `core/`, `puredata/` and `maxmsp/` each used to run
their own `conan install`; three independent resolutions meant three profile evaluations, which is
how the core came to be compiled for a different macOS deployment target than the wrappers linked
against.

A fallback that supplies the CMake targets from vcpkg while leaving `puredata/Makefile` on conan
recreates that split exactly.

## 4. Checklist before shipping a fallback

- [ ] All three surfaces in §1 covered, Makefile included
- [ ] Target names verified against the new provider (§3.1), double-precision FFTW confirmed
- [ ] Static vs shared decided; `YAML_CPP_STATIC_DEFINE` consistent in **both** places (§3.2)
- [ ] No shared `yaml-cpp`/`fftw` in `otool -L` on a built external (§3.3)
- [ ] `minos` check passes on macOS (§3.4)
- [ ] Architecture matches the wrappers' expectation (§3.6)
- [ ] Core and every wrapper resolve from the same provider (§3.8)
- [ ] `./build.sh -c deps core puredata maxmsp` then a full rebuild succeeds
- [ ] Pd and Max help patches load the rebuilt externals

## 5. Why conan, and why it stays

See [`repo-audit-2026-07-30.md`](repo-audit-2026-07-30.md) §2.2 for the duplicated-conanfile problem
and §4 for the fix. The short version: conan is the only mechanism currently giving all three
consumption surfaces from a single pinned, committed profile — and the committed profiles in
`profiles/` are what make the core/wrapper ABI agreement reproducible across machines and CI.
