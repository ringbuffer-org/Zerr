# Zerr*

**Zerr*** is a channel-based spatialization concept for an arbitrary audio source. It distributes audio signals to multiple loudspeakers, based only on the signals’ inherent features as well as the properties of the speaker array setup. Although various aspects of the algorithm can be tuned, also during runtime, the audio signal itself defines the spatial distribution. Depending on the parametrization, the approach can alter the original signal significantly, similar to modulation and distortion effects.

The main branch contains only the source code and configuration files of Zerr* system so that cannot be used directly. Please check other branches for details.

## Branches

### PD_modules

**PD_modules** branch contains the implementation of Zerr* in [PureData](https://puredata.info/) the graphic audio coding environment created by [Miller Puckette](http://msp.ucsd.edu/). You can build your own Zerr* PureData external from scratch using this branch.

### PD_releases

**PD_releases** branch contains the already built PureData externals for Zerr*. You can build them by yourself if there is no pre-built for your operation system.

### JACK

**JACK** branch contains the implementation of Zerr* using [JACK Audio Connection Kit](https://jackaudio.org/). This branch is now obsolete.

### Tests

**Tests** branch  is used for unit testing and debugging of each Zerr* module. This means you don't need to care about this branch :)

