# Zerr* for Max

Zerr* is a channel-based spatialization toolbox: it distributes an audio signal over any
loudspeaker layout, driven by features extracted from the signal itself rather than by manual
positioning. The design is described in Z. Yang and H. von Coler, *Autogenous Spatialization for
Arbitrary Loudspeaker Setups* (I3DA 2023, [paper](https://ieeexplore.ieee.org/abstract/document/10289141)).

The package contains four objects that form a chain:

```
source ──▶ zerr.features~ ──▶ zerr.envelopes~ ──▶ zerr.combinator~ ──▶ zerr.disperser~ ──▶ mc.dac~
```

| Object | Role |
| --- | --- |
| `zerr.features~` | extracts audio features (loudness, brightness, flux…) as audio-rate control signals |
| `zerr.envelopes~` | generates one envelope per speaker from a control signal, by trajectory or by trigger |
| `zerr.combinator~` | combines several envelope sets into one: sum, geometric mean or maximum per channel, switchable live |
| `zerr.disperser~` | disperses a mono source over the speakers, each channel shaped by its envelope |

Every object has a help patch. The package's overview (Package Manager → Zerr → *Launch*) links to
all of them.

## Requirements

- Max 9 on an Apple Silicon Mac, running natively. The externals are arm64 only; they do not load
  in Max running under Rosetta or on an Intel Mac.
- macOS 11 or later.

## Install

1. Unzip `zerr-max-<version>-macos-arm64.zip`. You get a folder named `Zerr`.
2. Move the `Zerr` folder into `~/Documents/Max 9/Packages/`.
3. Clear the quarantine flag macOS puts on downloaded files. The externals are signed ad hoc, not
   notarized, so without this step Max refuses to load them:

   ```bash
   xattr -dr com.apple.quarantine ~/Documents/Max\ 9/Packages/Zerr
   ```

4. Start Max (or restart it if it was running). In a new patcher, type `zerr.features~ rms`: the
   box should turn solid. Open the Package Manager, find *Zerr*, and press *Launch* for the overview.

## Troubleshooting

- **The object box stays red, and the Max Console says "unable to load object bundle executable"**
  or mentions a signature or malware check. The quarantine flag is still set. Run the `xattr`
  command above on the whole `Zerr` folder and restart Max.
- **"zerr.features~: No such object".** The folder must be named exactly `Zerr` and sit directly
  under `Packages`, with `externals/` inside it. Restart Max after moving it.
- **"cannot find speaker layout '…'"** from `zerr.envelopes~`, and the box stays dashed. The
  layout name is looked up on Max's search path, as given and with `.yaml` or `.yml` added. The
  package's `misc/` folder (on the search path once the package is installed) holds `quad_4`,
  `ring_8`, `line_16` and `ambisonic_21`. For your own layout, put the `.yaml` next to your
  patch, anywhere in the search path, or give an absolute path; then retype the box.
- **Everything instantiates but there is no sound from `zerr.disperser~`.** Its first inlet must
  receive a mono signal. A multichannel source silences the output.

## Speaker layouts

A layout is a YAML file listing each loudspeaker under `standard:` with a position, either
Cartesian (metres) or spherical (degrees and metres). The missing form is derived automatically.
Speaker numbers are the output channel order of `zerr.envelopes~`.

```yaml
standard:
  1:
    position:
      cartesian: { x: 3.265, y: -1.065, z: 0.410 }
  2:
    position:
      spherical: { azimuth: 45.0, elevation: 0.0, distance: 1.0 }
```

## Object reference

Each object also has a reference page inside Max: right-click the object and choose *Open
Reference*, or use the sidebar of its help patch. The pages say the same as this section.

### `zerr.features~ [<feature> …]`

Extracts audio features from a mono signal, at audio rate. With no argument it extracts `rms`.
**The features are not normalised**: they are control signals with very different ranges, never
audio. Do not connect an outlet to `dac~`; scale it and send it to `zerr.envelopes~`.

| name | feature | | name | feature |
| --- | --- | --- | --- | --- |
| `rms` | root mean square (loudness) | | `ctd` | spectral centroid (brightness) |
| `zcr` | zero-crossing rate | | `rlf` | spectral rolloff |
| `flx` | spectral flux (change) | | `flt` | spectral flatness |
| `cf` | crest factor | | `zc` | zero crossings |

- Inlet: the mono source.
- Outlets: one signal per feature argument, in argument order.
- `bang`: posts version, the features in outlet order, block size and sample rate.

All features except `zc` are measured over the last 2048 samples and updated once per signal
vector, ramped across it (about 46 ms lag at 44.1 kHz). `zc` is a sample-accurate pulse that
touches 1 on every zero crossing: a ready trigger for `zerr.envelopes~ trigger`, with `interval`
set. `rms`, `zcr`, `flt` are 0 to 1; `ctd`, `rlf` in Hz; `cf` 1 and up; `flx` 0 and up. Level
only affects `rms` and `flx`.

### `zerr.envelopes~ [trigger|trajectory] [<layout>]`

Generates one envelope per loudspeaker. The first argument is the mode (default `trajectory`),
the second the name of a layout YAML on the search path (`.yaml` may be omitted). Without a
layout the object uses a built-in ring of eight speakers, the same positions as `ring_8`.

- **trajectory**: the control signal (0 to 1) is a position along a chain of speakers; the envelope
  crossfades between neighbours along that chain.
- **trigger**: every time the control signal reaches 1, the envelope jumps to a speaker
  connected to the current one (at start: any active speaker, itself included); the spread inlet
  widens the jump over nearby speakers. The pulse must touch 1.0 exactly, and a signal held at 1
  keeps jumping, so send pulses and cap the rate with `interval`.

Inlets (all signals):

1. control signal: 0 to 1 position (trajectory) or pulses (trigger)
2. spread, 0 to 1 (trigger mode only)
3. gain, applied to every envelope (1 when nothing is connected)

Outlet: multichannel signal, one channel per speaker, in the layout's numbering.

Messages (speaker numbers as in the layout file):

| message | effect |
| --- | --- |
| `active set <n> …` / `active add <n> …` / `active del <n> …` | which speakers take part |
| `traj <n> <n> …` | the chain the trajectory mode pans along |
| `topo set <from> <n> …` / `topo add …` / `topo del …` | which speakers a trigger may jump to from `<from>` |
| `curr <n>` | set the current speaker (trigger mode) |
| `interval <ms>` | minimum time between two jumps (trigger mode) |
| `print` | post the active speakers, the trajectory and the topology (from \| to …) |
| `bang` | post version, mode, speaker count, layout, block size and sample rate |

Only active speakers may appear in `traj`, `topo` and `curr`: a `traj` or `topo set` with an
inactive number is refused, `topo add` and `topo del` skip it. `active set` replaces the list and
resets the trajectory (numeric order) and the topology (fully connected). `active add` only
activates; the new speaker joins no trajectory or topology until you put it there. `active del`
also drops the speakers from the trajectory and the topology, is refused if it would leave no
active speaker or an empty trajectory, and moves the sound to the first speaker still active if
the current one goes. A speaker with no links left keeps the trigger on itself.

### `zerr.combinator~ [<sets>] [add|root|max]`

Combines several envelope sets into one, channel by channel. The first argument is the number of
sets (default 2); the object gets one multichannel inlet per set, and every inlet must carry the same
channel count (otherwise it reports an error and outputs silence). The mode defaults to `max`.

- `add`: sum of the sets
- `root`: geometric mean, the N-th root of the product of N sets (a square root for two)
- `max`: the largest envelope in each channel

The mode can be changed while audio runs: `mode root` as a message, `@mode root` as an attribute,
or from the inspector. `chans` (read-only attribute) reports the channel count; `bang` posts the
current settings.

Think of `root` as "and" and `max` as "or": root plays a speaker only where every feature wants
it, max lets any feature claim one. `add` can exceed 1 and `zerr.disperser~` does not limit, so
scale it down, for instance with the gain inlet of each `zerr.envelopes~` or an `mc.*~` after the
combinator.

### `zerr.disperser~`

Disperses a mono source over the speakers by multiplying it with a set of envelopes. No arguments.

- Inlet 1: the mono source. A multichannel signal here silences the output: sum or pick a channel first.
- Inlet 2: multichannel envelopes from `zerr.envelopes~` or `zerr.combinator~`.
- Outlet: multichannel signal, one channel per envelope. Connect it to `mc.dac~` or route it on.

The channel count follows the envelopes, not a layout file. The envelopes are plain gains: values
above 1 boost, negative values flip the phase.

## Building from source

See the repository's [README](https://github.com/ringbuffer-org/Zerr#readme). In short:
`./build.sh -p maxmsp` builds the externals and assembles this package in `maxmsp/dist/Zerr`,
zipped alongside; `./build.sh -i maxmsp` also copies it into `~/Documents/Max 9/Packages`, and
`./build.sh -l maxmsp` symlinks it there instead, so every rebuild is what Max loads after a
restart.

## License

MIT, see `LICENSE`. The externals are built with Cycling '74's Min-DevKit (MIT) and link
[yaml-cpp](https://github.com/jbeder/yaml-cpp) (MIT) and [FFTW3](https://www.fftw.org/) (GPL)
statically.
