# Zerr* PureData Externals

Zerr* can be compiled into a PureData library. This directory contains PureData external wrappers for each Zerr module, as described in the paper [Autogenous Spatialization for Arbitrary Loudspeaker Setups](https://ieeexplore.ieee.org/abstract/document/10289141).

The following externals can be built from scratch:

- **zerr_features~**
- **zerr_envelopes~**
- **zerr_combinator~**
- **zerr_disperser~**

## Structure

### configs

YAML configuration files used by the Zerr system. The **speakers** folder contains configurations for various speaker array setups. The **presets** folder contains configurations for the Zerr system.

### dependencies

Default path for project dependencies. It contains **lib** and **include** folders where you should place external library files.

### externals

The folder where built externals are saved. PureData patches for testing each external and the combination of all externals are included.

### src

Zerr system source files, same as the **src** folder in the main branch.

### include

Zerr system headers, same as the **include** folder in the main branch.

### puredata

PureData external wrappers for the Zerr modules. Each folder corresponds to one external.

### build.sh

Script that builds all externals with one command. Tested on macOS only; compatibility with other systems is not guaranteed.

## How to Build

Make sure you have **Make** installed.

### 1. Dependencies

Zerr depends on [fftw3](http://fftw.org/) and [yaml-cpp](https://github.com/jbeder/yaml-cpp). Please follow the instructions on each project's page for installation. The default location for headers and libraries is the **lib** and **include** folders inside the **dependencies** folder. You can copy the files there, or edit the dependency paths in the Makefile.

### 2. Build Separately

Navigate (`cd`) into the external folder inside **puredata**. Run the **make.sh** file to start the build. This script will automatically detect and start the PureData patch in the build directory after building. The **PureData API (m_pd.h)** and **pd-lib-builder** are included.

> **Note:** The **zerr_envelopes~** external may fail to be created when PureData is first started. Simply re-create it (edit and save) to fix this.

### 3. Build All

You can use the **build.sh** script to build all externals together. The built externals can be found in the **externals** folder. The PureData patch for testing all externals will also be opened automatically.

## Usage

### zerr_features~

The **zerr_features~** external calculates different audio features from a mono audio input. Use feature names in the PureData object arguments to specify which features to extract. The outlets send the extracted audio features at audio rate in the same order as the feature names in the arguments.

**Valid feature names:**

- RootMeanSquare
- ZeroCrossingRate
- Flux
- Centroid
- Rolloff
- CrestFactor
- Flatness
- ZeroCrossings
- (and more)

### zerr_envelopes~

The **zerr_envelopes~** external creates envelopes according to the incoming control signal and speaker configuration. The first argument specifies the envelope generation mode (trajectory/trigger). The second argument is the path to the speaker array configuration file. Relative paths are supported.

### zerr_combinator~

The **zerr_combinator~** external combines multi-channel envelopes from different generators. The first argument specifies the number of generators, and the second specifies the channel number for each generator. Only generators with the same channel number can be combined. The inlet count equals the number of sources multiplied by the channel number, with inlets grouped by source. For example, in the demo patch with 16 inlets: inlets 1-8 are the channels from the first input source, and inlets 9-16 are from the second.

### zerr_disperser~

The **zerr_disperser~** external distributes mono source audio to multiple channels by multiplying it with a multi-channel envelope. The only argument specifies the channel number. The first inlet receives the mono source audio, and the remaining inlets receive the envelopes.
