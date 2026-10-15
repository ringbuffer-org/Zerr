#!/usr/bin/env bash
# Derive every Zerr* logo variant from the original drawing with ffmpeg only.
#
# The source is charcoal ink on textured paper. One luminance stretch turns it into an
# ink-coverage mask (paper -> 0, solid ink -> 255, charcoal grain in between); every variant
# is that mask composited over a colour. The drawing itself is never edited.
#
#   tools/make-logo-variants.sh            # regenerate assets/logo/ and maxmsp/media/zerr_logo.png
#   W=0.83 B=0.14 tools/make-logo-variants.sh   # retune the levels
#
# Tunables (measured on the 1024 px source with ffmpeg's histogram filter, 2026-10-15):
# paper sits at luminance 208-239, solid ink at 0-63, charcoal grain in between.
set -euo pipefail
cd "$(dirname "$0")/.."

SRC=${SRC:-assets/logo/zerr-logo-paper.png}
OUT=${OUT:-assets/logo}
W=${W:-0.81}  # white point: luminance at or above W*255 is background
B=${B:-0.16}  # black point: luminance at or below B*255 is solid ink
# Ink bounding box plus a 24 px margin (alpha > 128, at least 3 px per row/column).
TRIM=${TRIM:-crop=877:873:84:83}
SIZES=${SIZES:-"512 256 128 64 32"}
ICO_SIZES="16 32 48 64 128 256"
AVATAR_PAD=96  # px added on each side before the disc so the outer nodes are not clipped
MAX_MEDIA=maxmsp/media/zerr_logo.png

for tool in ffmpeg ffprobe; do
    command -v "$tool" >/dev/null || { echo "$tool not found" >&2; exit 1; }
done
[ -f "$SRC" ] || { echo "source not found: $SRC" >&2; exit 1; }

IFS=, read -r WIDTH HEIGHT < <(ffprobe -v error -select_streams v:0 -show_entries stream=width,height \
    -of csv=p=0 "$SRC")
SIZE="${WIDTH}x${HEIGHT}"

# Ink coverage. LEVELS leaves ink dark on a white background; MASK (negated) is the alpha plane.
LEVELS="format=gray,colorlevels=rimin=${B}:rimax=${W}:gimin=${B}:gimax=${W}:bimin=${B}:bimax=${W}"
MASK="${LEVELS},negate"

ff() { ffmpeg -v error -y "$@"; }

# ink_on_transparent <colour> <out.png>: solid colour under the mask, transparent elsewhere.
ink_on_transparent() {
    ff -i "$SRC" -f lavfi -i "color=c=$1:s=$SIZE" \
        -filter_complex "[0:v]${MASK}[a];[1:v][a]alphamerge" -frames:v 1 -pix_fmt rgba "$2"
}

mkdir -p "$OUT/sizes" "$OUT/trimmed"

echo "mask: W=$W B=$B on $SRC ($SIZE)"

# The four main variants.
ff -i "$SRC" -vf "${LEVELS},format=rgb24" "$OUT/zerr-logo-black-on-white.png"
ff -i "$SRC" -vf "${MASK},format=rgb24"   "$OUT/zerr-logo-white-on-black.png"
ink_on_transparent black "$OUT/zerr-logo-black-transparent.png"
ink_on_transparent white "$OUT/zerr-logo-white-transparent.png"

# Neutral grey ink for watermarks and surfaces of unknown brightness.
ink_on_transparent 0x808080 "$OUT/zerr-logo-grey-transparent.png"

# Size set of the transparent variants. The colour plane is uniform, so Lanczos on rgba
# cannot produce colour fringes; only the alpha is resampled.
for name in black-transparent white-transparent; do
    for s in $SIZES; do
        ff -i "$OUT/zerr-logo-$name.png" -vf "scale=${s}:${s}:flags=lanczos" \
            "$OUT/sizes/zerr-logo-$name-$s.png"
    done
done

# Multi-size favicon, PNG-compressed entries.
ico_inputs=()
ico_maps=()
i=0
for s in $ICO_SIZES; do
    tmp="$OUT/sizes/.ico-$s.png"
    ff -i "$OUT/zerr-logo-black-transparent.png" -vf "scale=${s}:${s}:flags=lanczos" "$tmp"
    ico_inputs+=(-i "$tmp")
    ico_maps+=(-map "$i")
    i=$((i + 1))
done
ff "${ico_inputs[@]}" "${ico_maps[@]}" -c:v png -f ico "$OUT/zerr-logo.ico"
rm -f "$OUT"/sizes/.ico-*.png

# Trimmed to the ink bounding box.
for name in black-on-white white-on-black black-transparent white-transparent grey-transparent; do
    ff -i "$OUT/zerr-logo-$name.png" -vf "$TRIM" "$OUT/trimmed/zerr-logo-$name.png"
done

# Circle avatars: pad so the outer nodes fit inside the disc, then multiply the alpha by a
# disc with a one-pixel anti-aliased rim.
disc="format=rgba,geq=r='r(X,Y)':g='g(X,Y)':b='b(X,Y)':a='alpha(X,Y)*clip(W/2-hypot(X+0.5-W/2,Y+0.5-H/2),0,1)'"
pad="pad=iw+2*${AVATAR_PAD}:ih+2*${AVATAR_PAD}:${AVATAR_PAD}:${AVATAR_PAD}:color=0x00000000"
for name in black-transparent white-transparent; do
    ff -i "$OUT/zerr-logo-$name.png" -vf "${pad},${disc}" "$OUT/zerr-logo-avatar-$name.png"
done

# The Max package ships a 256 px copy under this fixed name (the .maxhelp files reference it).
ff -i "$OUT/zerr-logo-black-transparent.png" -vf "scale=256:256:flags=lanczos" "$MAX_MEDIA"

echo "written:"
find "$OUT" "$MAX_MEDIA" -type f \( -name '*.png' -o -name '*.ico' \) | sort
