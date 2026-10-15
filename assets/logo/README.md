# Zerr* logo

`zerr-logo-paper.png` is the original drawing (charcoal on paper, 1024 px). Everything else in
this folder is generated from it by `tools/make-logo-variants.sh` with ffmpeg only; do not edit
the generated files by hand, rerun the script instead.

| File | Use |
| --- | --- |
| `zerr-logo-black-transparent.png` | default on light surfaces (README, docs) |
| `zerr-logo-white-transparent.png` | dark surfaces; the README switches to it in dark mode |
| `zerr-logo-black-on-white.png`, `zerr-logo-white-on-black.png` | opaque versions for places that cannot composite alpha |
| `zerr-logo-grey-transparent.png` | neutral ink for watermarks or surfaces of unknown brightness |
| `zerr-logo-avatar-*.png` | disc-masked, padded 1216 px, for GitHub or chat avatars |
| `zerr-logo.ico` | favicon, 16 to 256 px |
| `sizes/` | 512 to 32 px copies of the two transparent variants |
| `trimmed/` | cropped to the ink bounding box plus a 24 px margin |

`maxmsp/media/zerr_logo.png` is the 256 px black-on-transparent copy the Max help patches load;
the script overwrites it too.
