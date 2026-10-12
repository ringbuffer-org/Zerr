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
| `zerr.features~` | extracts audio features (loudness, brightness, flux…) as audio-rate signals |
| `zerr.envelopes~` | turns a control signal plus a speaker layout into one envelope per loudspeaker |
| `zerr.combinator~` | merges several envelope sets into one (add, root, max), switchable live |
| `zerr.disperser~` | multiplies a mono source by the envelopes: one output channel per speaker |

Every object has a help patch. The package's overview (Package Manager → Zerr → *Launch*) links to
all of them.

## Requirements

- Max 9 on an Apple Silicon Mac, running natively. The externals are arm64 only; they do not load
  in Max running under Rosetta or on an Intel Mac.
- macOS 11 or later.

## Install

1. Unzip `Zerr-<version>-macos-arm64.zip`. You get a folder named `Zerr`.
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
- **"Cannot find YAML file: …"** from `zerr.envelopes~`. The layout name is looked up on Max's
  search path. The package's `misc/` folder (on the search path once the package is installed)
  holds `quad_4`, `ring_8`, `line_16` and `ambisonic_21`; `circulation_8` sits beside the help
  patches. For your own layout, put the `.yaml` next to your patch, anywhere in the search path, or
  give an absolute path.
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

### `zerr.features~ <feature> [<feature> …]`

Extracts audio features from a mono signal, at audio rate. At least one feature name is required.

| name | feature | | name | feature |
| --- | --- | --- | --- | --- |
| `rms` | root mean square (loudness) | | `ctd` | spectral centroid (brightness) |
| `zcr` | zero-crossing rate | | `rlf` | spectral rolloff |
| `flx` | spectral flux (change) | | `flt` | spectral flatness |
| `cf` | crest factor | | `zc` | zero crossings |

- Inlet: the mono source.
- Outlets: one signal per feature argument, in argument order.
- `bang`: posts the outlet count to the Console.

### `zerr.envelopes~ <trigger|trajectory> <layout>`

Generates one envelope per loudspeaker. Both arguments are required: the mode and the name of a
layout YAML on the search path (`.yaml` may be omitted).

- **trajectory**: the control signal (0 to 1) is a position along a chain of speakers; the envelope
  crossfades between neighbours along that chain.
- **trigger**: every time the control signal reaches 1, the envelope jumps to another speaker
  connected to the current one; the spread inlet widens the jump over nearby speakers.

Inlets (all signals):

1. control signal: 0 to 1 position (trajectory) or pulses (trigger)
2. spread, 0 to 1 (trigger mode only)
3. gain, applied to every envelope

Outlet: multichannel signal, one channel per speaker, in the layout's numbering.

Messages (speaker numbers as in the layout file):

| message | effect |
| --- | --- |
| `active set <n> …` / `active add <n> …` / `active del <n> …` | which speakers take part |
| `traj <n> <n> …` | the chain the trajectory mode pans along |
| `topo set <from> <n> …` / `topo add …` / `topo del …` | which speakers a trigger may jump to from `<from>` |
| `curr <n>` | set the current speaker (trigger mode) |
| `interval <ms>` | minimum time between two jumps (trigger mode) |
| `print` | post the current speaker setup |
| `bang` | post the outlet count |

A speaker must be active to appear in `traj`, `topo` or `curr`; deactivate it only after taking it
out of those lists.

### `zerr.combinator~ <sets> [add|root|max]`

Merges several envelope sets into one. The first argument is the number of sets; the object gets
one multichannel inlet per set, and every inlet must carry the same channel count (otherwise it
reports an error and outputs silence). The mode defaults to `max`.

- `add`: sum of the sets
- `root`: geometric mean, the N-th root of the product of N sets
- `max`: the largest envelope in each channel

The mode can be changed while audio runs: `mode root` as a message, `@mode root` as an attribute,
or from the inspector. `chans` (read-only attribute) reports the channel count; `bang` posts the
current settings.

### `zerr.disperser~`

Multiplies a mono source by a set of envelopes. No arguments.

- Inlet 1: the mono source.
- Inlet 2: multichannel envelopes from `zerr.envelopes~` or `zerr.combinator~`.
- Outlet: multichannel signal, one channel per envelope. Connect it to `mc.dac~` or route it on.

## Building from source

See the repository's [README](https://github.com/ringbuffer-org/Zerr#readme). In short:
`./build.sh core && ./build.sh -i maxmsp` builds the externals and assembles this package in
`maxmsp/dist/Zerr`, zipped alongside. For development, symlink that folder into `Packages`
instead of copying it.

## License

MIT, see `LICENSE`. The externals are built with Cycling '74's Min-DevKit (MIT) and link
[yaml-cpp](https://github.com/jbeder/yaml-cpp) (MIT) and [FFTW3](https://www.fftw.org/) (GPL)
statically.
