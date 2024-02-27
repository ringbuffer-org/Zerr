# Zerr*

## Introduction

**Zerr*** is a channel-based spatialization concept for an arbitrary audio source. It distributes audio signals to multiple loudspeakers, based only on the signals’ inherent features as well as the properties of the speaker array setup. Although various aspects of the algorithm can be tuned, also during runtime, the audio signal itself defines the spatial distribution. Depending on the parametrization, the approach can alter the original signal significantly, similar to modulation and distortion effects. Please check on the [conference paper][https://ieeexplore.ieee.org/abstract/document/10289141], "Autogenous Spatialization for Arbitrary Loudspeaker Setups" for design details. 

The Zerr* system was meant to be available in different kinds of creative coding environment and different computer systems . The supported environment for now listed below:

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

### Build

Otherwise, you can also build it for your own machine using the **build.sh** script. 

#### For Puredata

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

