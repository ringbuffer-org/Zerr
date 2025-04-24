# Zerr*

## Introduction

**Zerr*** is a channel-based spatialization concept for an arbitrary audio source. It distributes audio signals to multiple loudspeakers, based only on the signals’ inherent features as well as the properties of the speaker array setup. Although various aspects of the algorithm can be tuned, also during runtime, the audio signal itself defines the spatial distribution. Depending on the parametrization, the approach can alter the original signal significantly, similar to modulation and distortion effects. Please check the [conference paper](https://ieeexplore.ieee.org/abstract/document/10289141), "Autogenous Spatialization for Arbitrary Loudspeaker Setups" for design details. 

**Zerr*** was was intended to be available in various creative coding environments. The currently supported environments are listed below:

|                   | Linux | MacOS(M1) | MacOS(Intel) | Windows |
| ----------------- | ----- | --------- | ------------ | ------- |
| **Puredata**      | ✅     | ✅         | ✅            | 🛠️       |
| **JACK**          | 🛠️     | ✅         | 🛠️            | 🛠️       |
| **SuperCollider** | ⏳     | ⏳         | ⏳            | ⏳       |
| **Max/MSP**       | **➖** | 🛠️         | 🛠️            | 🛠️       |

✅: **Fully Supported**

🛠️: **In Development**

⏳: **Planned**

**➖**: **Not available**

## Installation

### Releases

You can download the precompiled version of each environment and system from Releases. Please check the readme of each environment for detail instructions.

### How to Build

Otherwise, you can also build it for your own machine. 

#### Initialization

- Clone this repo and initialize the submodules( [pd-lib-builder](https://github.com/pure-data/pd-lib-builder), [min-devkit](https://cycling74.github.io/min-devkit/) )

  ```bash
  git clone --recurse-submodules git@github.com:ringbuffer-org/Zerr.git && cd Zerr
  ```

#### Dependencies

- Zerr* uses [Conan](https://docs.conan.io/2/tutorial.html) for dependency management, please refer to its documentation for details
- Zerr* depends only on [yaml-cpp](https://github.com/jbeder/yaml-cpp) and [fftw3](https://www.fftw.org/). Use the following Conan command to install them

```bash
conan install . --output-folder=build --build=missing
```

- You can also edit the paths in MakefIle/CMakeLists.txt to point to your own 

#### For Puredata

- Make sure you have installed [Puredata](https://puredata.info/downloads/pure-data). Building pd externals depends on the Puredata-API (m_pd.h)

- Build/Install with following commands

```bash
# build puredta externals
./build.sh puredata

# build and install puredata package
./build.sh -i puredata
```

#### For Jack

```bash
# build Jack client
./build.sh jack
```

