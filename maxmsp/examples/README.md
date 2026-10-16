# Zerr* examples

Complete patches, one scenario each, meant to be copied and adapted. They ship in the package's
`examples/` folder (Max adds it to the search path) and open from the Package Manager's Examples
list or from the Examples row of the overview patch. The help patches document the objects; these
show them working together.

| Patch | Feature | Mode, layout | Shows |
| --- | --- | --- | --- |
| `full-chain-quad.maxpat` | rms and ctd | trajectory, `quad_4` | the whole chain: two features, two envelope sets, `zerr.combinator~` in root mode, four speakers. Start here |
| `spectral-centroid-panning.maxpat` | spectral centroid | trajectory, `ring_8` | panning around a ring |
| `spectral-flatness-breath.maxpat` | spectral flatness | trigger, `line_16` | the spread inlet widening the sound around one fixed speaker |
| `zerr-cross-jump.maxpat` | zero crossings | trigger, `ambisonic_21` | jumps through a hand-made speaker topology on the dome |
| `custom-layout.maxpat` | none | trajectory, `my-room-6` | loading your own layout: `my-room-6.yaml` next to the patch, what the coordinates mean |

The centroid, flatness and zero-crossing examples are ports of the Pure Data examples in
`puredata/examples/`; the Pd versions use an 11-speaker line, the Max package ships `line_16`.

Every example starts with the master gain at 0: turn audio on, then raise it.
