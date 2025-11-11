# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

## Project Overview

**Zerr*** is a channel-based audio spatialization system for arbitrary loudspeaker configurations. It distributes audio signals to multiple speakers based on audio features and speaker array properties. The system is designed for creative coding environments including Puredata, Max/MSP, and JACK.

## Build System

### Prerequisites

- **Conan** (v2.x): Dependency management
- **CMake** (3.15+): Build system for core library and Max/MSP
- **C++17 compiler**: g++ or clang
- **Puredata API** (for Puredata builds)
- **Xcode 10+** (for macOS builds)

### Dependencies

- `fftw3` (3.3.10): FFT library for spectral analysis
- `yaml-cpp` (0.8.0): Configuration file parsing

### Build Commands

#### Core Library (Static)

```bash
# Install dependencies and build core library
cd core
conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
make install  # Installs to core/lib
```

#### Puredata External

```bash
# Build Puredata externals (automatically builds core if needed)
./build.sh puredata

# Build and install to Puredata packages directory
./build.sh -i puredata
```

#### Max/MSP External

```bash
# Build Max/MSP externals
./build.sh maxmsp

# Build and install
./build.sh -i maxmsp
```

#### JACK Client

```bash
./build.sh jack  # Currently placeholder
```

### Build Script Usage

The `build.sh` script handles multi-target builds:
- Automatically checks and builds core library if not present
- Use `-i` flag to install after building
- Accepts multiple targets: `./build.sh [-i] <puredata|maxmsp|jack>`

## Architecture

### Core Library (`core/`)

The core library (`zerr_core`) is a C++17 static library providing the fundamental audio spatialization algorithms. It is organized into three main subsystems:

#### Modules (`core/include/modules/`)

- **SpeakerManager**: Manages speaker array configurations loaded from YAML files. Handles speaker selection, trajectory-based panning, topology control, and spatial relationships between speakers.
- **FeatureExtractor**: Abstract base class for audio feature extraction algorithms. Defines interface for initialization, processing, and data management.
- **FeatureBank**: Collection of feature extractors working together.
- **FeatureProcessor**: Processes extracted features for spatialization decisions.
- **EnvelopeGenerator**: Generates control envelopes based on audio features.
- **EnvelopeCombinator**: Combines multiple envelopes using various algorithms.
- **AudioDisperser**: Routes and distributes audio signals across multiple output channels.

#### Features (`core/include/features/`)

Implements specific audio feature extractors:
- **Spectral**: Centroid, Rolloff, Flatness, Flux
- **Temporal**: RootMeanSquare, ZeroCrossingRate, CrestFactor, ZeroCrossings

All feature extractors inherit from `FeatureExtractor` base class.

#### Utils (`core/include/utils/`)

- **types.h**: Core type definitions (Sample, Block, Param, Position, etc.)
- **configs.h**: System and speaker configuration structures
- **logger.h**: Logging utilities
- **utils.h**: Helper functions
- **FrequencyTransformer**: FFT/IFFT operations using FFTW3
- **RingBuffer**: Circular buffer for audio processing
- **OnsetDetector**: Temporal event detection
- **LinearInterpolator**: Signal interpolation

### Environment-Specific Wrappers

#### Puredata (`puredata/`)

Provides four Pure Data externals wrapping core functionality:
- `zerr_combinator~`: Envelope combination
- `zerr_features~`: Feature extraction  
- `zerr_envelopes~`: Envelope generation
- `zerr_disperser~`: Audio distribution

Uses `pd-lib-builder` for cross-platform compilation. Each external has corresponding help patches in `puredata/help/`.

#### Max/MSP (`maxmsp/`)

Max/MSP externals built using Min-DevKit:
- `mc.zerr.combinator~`: Multi-channel envelope combinator
- `mc.zerr.features~`: Multi-channel feature extractor
- `mc.zerr.envelopes~`: Multi-channel envelope generator
- `mc.zerr.disperser~`: Multi-channel audio disperser

Projects are in `maxmsp/source/projects/`. Uses CMake build system with Min-API.

#### JACK (`jack/`)

Standalone JACK client (in development).

### Configuration Files

Speaker array configurations are defined in YAML files (`configs/`):
- Specify speaker positions in Cartesian coordinates (x, y, z)
- Support arbitrary speaker arrangements
- Examples: `quad_4.yaml`, `ring_8.yaml`, `line_16.yaml`, `ambisonic_21.yaml`

Format:
```yaml
standard:
  1:
    position:
      cartesian:
        x: 3.265
        y: -1.065
        z: 0.410
```

## Code Style

The project uses clang-format with the following configuration:
- Based on LLVM style
- Indent width: 4 spaces
- Column limit: 100
- Pointer alignment: Left (`int* ptr`)
- Brace breaking: Stroustrup style
- Aligned consecutive assignments

Format code with: `clang-format -i <file>`

## File Organization

- `core/`: Core C++ library (platform-independent)
- `puredata/`: Puredata external wrappers
- `maxmsp/`: Max/MSP external wrappers  
- `jack/`: JACK client (WIP)
- `configs/`: Speaker array YAML configurations
- `.github/workflows/`: CI for automated builds (macOS, Linux, Windows)

## Key Implementation Details

### Speaker Array System

- Speaker positions can be specified in Cartesian or spherical coordinates
- SpeakerManager handles speaker activation/deactivation dynamically
- Supports trajectory-based panning between speaker pairs
- Topology controls for spatial relationships

### Feature Extraction Pipeline

1. Audio input → FeatureExtractor.fetch()
2. Feature calculation → FeatureExtractor.extract()
3. Feature values → FeatureExtractor.send()
4. Features feed into EnvelopeGenerator for spatialization control

### Build Dependencies

The core library must be built first as other targets depend on `libzerr_core.a`. The build script (`build.sh`) automatically handles this dependency chain.

Conan generates platform-specific build configuration:
- `conan_toolchain.cmake`: CMake toolchain file
- `conandeps.mk`: Makefile dependencies (for Puredata)

## Platform-Specific Notes

### macOS
- Supports both Intel and Apple Silicon (M1/M2)
- Fat binaries built when using Xcode 12+
- Min deployment target: macOS 13.3

### Linux
- Requires `-fPIC` flag for static library builds
- Set via `CXXFLAGS` environment variable

### Windows
- Uses Visual Studio compiler
- CMake presets: `conan-default`
- Library output: `build/Release/zerr_core.lib`

## Testing

No formal test suite is currently implemented in the repository. Testing is primarily done through the help patches in each environment (Puredata and Max/MSP).
