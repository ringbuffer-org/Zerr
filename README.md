# Zerr*

## Introduction

**Zerr*** is a channel-based spatialization concept for an arbitrary audio source. It distributes audio signals to multiple loudspeakers, based only on the signals’ inherent features as well as the properties of the speaker array setup. Although various aspects of the algorithm can be tuned, also during runtime, the audio signal itself defines the spatial distribution. Depending on the parametrization, the approach can alter the original signal significantly, similar to modulation and distortion effects. Please check on the [conference paper](https://ieeexplore.ieee.org/abstract/document/10289141), "Autogenous Spatialization for Arbitrary Loudspeaker Setups" for design details. 

 Zerr* was meant to be available in different kinds of creative coding environment and different computer systems. The supported environment for now listed below:

|                   | Linux | MacOS(M1) | MacOS(Intel) | Windows |
| ----------------- | ----- | --------- | ------------ | ------- |
| **Puredata**      | ✅     | ✅         | ✅            | 🚧       |
| **Jack**          | 🚧     | ✅         | 🚧            | 🚧       |
| **Supercollider** | ❌     | ❌         | ❌            | ❌       |
| **Max/MSP**       | ❌     | ❌         | ❌            | ❌       |

✅: Support

🚧: Under development

❌: For future

## Installation

### Releases

You can download the precompiled version of each environment and system from Releases. Please check the readme of each environment for detail instructions.

### How to Build

Otherwise, you can also build it for your own machine. 

#### For Puredata

- Clone this repo and initialize the **[pd-lib-builder](https://github.com/pure-data/pd-lib-builder)** submodule

  ```bash
  git clone git@github.com:ringbuffer-org/Zerr.git
  cd Zerr
  git submodule init
  ```

- Make sure you have installed [Puredata](https://puredata.info/downloads/pure-data). Building pd externals depends on the Puredata-API (m_pd.h)

- Make sure you have **Make** installed :)
- Zerr* depends on [**yaml-cpp**](https://github.com/jbeder/yaml-cpp) and [**fftw3**](https://www.fftw.org/), the default dependency path is the the dependencies folder inside repo. You can change the paths in puredata/Makefie to point to your own install paths.

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

