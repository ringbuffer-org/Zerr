# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Zerr* is a channel-based audio spatialization system that distributes audio signals to arbitrary loudspeaker configurations based on audio features and speaker array properties. It is a C++17 project with platform-specific wrappers for PureData, Max/MSP, and JACK.

Reference paper: "Autogenous Spatialization for Arbitrary Loudspeaker Setups" (IEEE 2023).

## Build Commands

### Dependencies (resolved once, at the repo root)

Declared in the root `conanfile.txt` and installed into a single shared `build/` folder that every
target consumes: core and Max/MSP via `build/conan_toolchain.cmake`, PureData via
`build/conandeps.mk`. Committed profiles in `profiles/` (`macos`, `linux`, `mingw`) pin the settings
that must be reproducible across machines. Recipe revisions are pinned in `conanfile.txt` because both
libraries need a ConanCenter revision that supports CMake 4. The macOS deployment target is set in
`core/CMakeLists.txt` and `maxmsp/CMakeLists.txt`, not in the profile.

```bash
conan install . --output-folder=build --build=missing \
  -pr:h=profiles/macos -pr:b=profiles/macos   # or profiles/linux, profiles/mingw
```

`build.sh` does this automatically on first use. Conan is only needed when dependencies actually
need resolving — a tree with a populated `build/` still builds without it.

### Core Library (must be built first — other targets depend on `libzerr_core.a`)

```bash
cmake -S core -B core/build \
  -DCMAKE_TOOLCHAIN_FILE="$PWD/build/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build core/build
cmake --install core/build   # Installs to core/lib
```

### Using the build script (recommended)

```bash
./build.sh deps              # Resolve dependencies only
./build.sh -c deps           # Discard resolved dependencies
./build.sh core              # Build core library
./build.sh puredata          # Build PureData externals (auto-builds core if needed)
./build.sh -i puredata       # Build and install
./build.sh maxmsp            # Build Max/MSP externals
./build.sh -i maxmsp         # Build and install
./build.sh jack              # JACK client — currently broken, see docs/design/repo-audit-2026-07-30.md
```

### Code Formatting

```bash
clang-format -i <file>
```

Style: LLVM-based, 4-space indent, 100-column limit, left pointer alignment (`int* ptr`), Stroustrup braces.

### Testing

No formal test suite. Testing is done via help patches in PureData (`puredata/help/*.pd`) and Max/MSP environments.

## Architecture

### Processing Pipeline

```
Audio Input → FeatureExtractor → FeatureBank → FeatureProcessor
                                                      ↓
Speaker Output ← AudioDisperser ← EnvelopeCombinator ← EnvelopeGenerator
```

Audio features (spectral centroid, RMS, flux, etc.) drive spatial distribution — no manual positioning.

### Core Library (`core/`)

Platform-independent C++17 static library (`zerr_core`). Three subsystems:

- **Modules** (`core/include/modules/`): High-level processing — SpeakerManager, FeatureBank, FeatureProcessor, EnvelopeGenerator, EnvelopeCombinator, AudioDisperser
- **Features** (`core/include/features/`): Spectral (Centroid, Rolloff, Flatness, Flux) and temporal (RMS, ZeroCrossingRate, CrestFactor, ZeroCrossings) extractors, all inheriting from `FeatureExtractor` base class
- **Utils** (`core/include/utils/`): Types (`types.h`), configs (`configs.h`), FrequencyTransformer (FFTW3 wrapper), RingBuffer, OnsetDetector, LinearInterpolator

### Platform Wrappers

- **PureData** (`puredata/`): 4 externals (`zerr_features~`, `zerr_envelopes~`, `zerr_combinator~`, `zerr_disperser~`). Built with pd-lib-builder (git submodule) + Makefile.
- **Max/MSP** (`maxmsp/`): 5 externals (mc.zerr.* variants). Built with Min-DevKit (git submodule) + CMake.
- **JACK** (`jack/`): Standalone client, in development. Uses Meson.

### Speaker Configurations (`configs/`)

YAML files defining speaker positions in Cartesian coordinates (x, y, z). Examples: `quad_4.yaml`, `ring_8.yaml`, `line_16.yaml`, `ambisonic_21.yaml`.

## Code Conventions

- **Namespace**: `zerr`, features in `zerr::feature`
- **Custom types** (`types.h`): `Sample` = double, `Param` = float, `Index` = int
- **Naming**: PascalCase classes, camelCase methods, `_prefixed` private methods
- **Dependencies**: Conan 2.x manages fftw3 and yaml-cpp, declared once in the root `conanfile.txt`

## Platform Notes

- **macOS**: Supports Intel + Apple Silicon (fat binaries with Xcode 12+)
- **Linux**: Requires `-fPIC` for static library builds
- **Windows**: MinGW-w64 gcc 13 for PureData; uses the `profiles/mingw` Conan profile. `build.sh`
  runs from an MSYS2 MINGW64 / Git Bash shell — it pins the `MinGW Makefiles` generator and prefers
  `mingw32-make`. Max/MSP is not buildable there (`.mxe64` needs MSVC, which cannot link a
  MinGW-built core); `build.sh maxmsp` refuses with a message

Before changing any `find_package` call, the `CONAN_*` variables in `puredata/Makefile`, or
`conanfile.txt`, read `docs/design/dependency-fallbacks.md` — it records which non-conan resolve
routes are viable and what each one breaks (target names, static/shared, deployment target, CMake 4).
