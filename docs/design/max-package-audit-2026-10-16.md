# Max package audit: what is missing before a Package Manager submission

_Audit of the staged Max package (`maxmsp/dist/Zerr`, built from the 2026-10-16 tree) against
Cycling '74's package format and submission process. Nothing was changed by the audit; every gap
is a task on the Zerr* Tasks board (parent ID 176, sub-items 178 to 184; ID 177 under the release
gate ID 171). Submission comes after `max-v0.3.0` ships._

## Sources

- Submission form: https://cycling74.com/support/submit-packages (author, e-mail, website, a link
  to a downloadable package, a summary, a readiness date; "submitting a package does not guarantee
  inclusion … and will likely require some follow-up communications"; questions to
  packages@cycling74.com). Support article: https://support.cycling74.com/hc/en-us/articles/360049933214-Submitting-Projects-Packages
- Package authoring tutorials the form points to:
  https://cycling74.com/tutorials/package-authoring-an-introduction,
  https://cycling74.com/tutorials/package-authoring-the-finer-details ("you will need to have both
  a ReadMe and a License in your package"),
  https://cycling74.com/tutorials/package-authoring-writing-documentation,
  https://cycling74.com/tutorials/writing-reference-pages
- Package format: https://docs.cycling74.com/userguide/packages/ (folders, `icon.png` 500x500,
  readme, license, toolbar icon); `package-info.json` keys:
  https://docs.cycling74.com/legacy/max8/vignettes/package_info_json; what the Package Manager
  shows: https://docs.cycling74.com/api/latest/max8/vignettes/package_manager
- Reference packages on this machine: the ones bundled with Max 9.2
  (`/Applications/Max.app/Contents/Resources/C74/packages/*`) and the installed spat5 and
  ABL Effect Modules packages. All of them ship a root `icon.png` (500x500, one 720x720, one
  120x120) and a flat `docs/*.maxref.xml`.

Cycling '74 publishes no acceptance criteria. Nothing official says whether a Git link or a zip is
expected, whether externals must be notarized, which architectures are required, or how long
review takes. Items marked **decision** below are therefore ours to settle, and worth asking
packages@cycling74.com about before submitting.

## Requirements checklist

| Requirement | Source | Zerr status |
| --- | --- | --- |
| `readme.md` at the package root | tutorial part 2 (required) | OK: `maxmsp/README.md` ships, with Requirements, Install, Troubleshooting, Object reference |
| `license.md`/`LICENSE` at the root | tutorial part 2 (required) | OK: MIT, plus the Min-DevKit MIT notice |
| `icon.png`, 500x500, at the root | user guide | **gap**: none (only the two logos in `media/`) |
| `package-info.json` at the root | user guide | OK |
| `name` equals the folder name | vignette + tutorial comments | OK: `Zerr` / `Zerr/` |
| `displayname` | vignette | **gap**: absent (Package Manager shows `name`) |
| `version` semver | vignette | OK: `0.3.0`, the single Max version source |
| `author`, `description`, `website`, `tags` | vignette | OK |
| `max_version_min` / `max_version_max` | vignette | OK: `9.0` / `none` |
| `os.macintosh.platform`, `min_version` | vignette; Package Manager offers the package by this | **decision**: lists `x64` and `aarch64`, binaries are arm64 only; `x64` is the known Max 9 workaround (`docs/design/release-process.md`, Max checklist) |
| `os.windows` | vignette | **decision**: block present, no Windows Max build exists (`build.sh` refuses, CI Max job is macOS only) |
| no generated keys (`filelist`, `c74install`, `installdate`) | vignette | OK |
| `homepatcher` and the Launch button | vignette, Package Manager | OK: `Zerr_Overview.maxpat` in `extras/`. The vignette says `patchers/`, but every package on this machine that sets the key keeps it in `extras/` (RNBO, Vizzie, jit.mo, ableton-dsp, Node for Max bundled with Max 9.2; ABL Effect Modules third-party), and both folders are on the search path. Keep it in `extras/`; it is also what puts it in the Extras menu. Verified 2026-10-16 |
| `examples/` vs `patchers/` | user guide (`patchers`: abstractions on the search path; `examples`: example patchers and material) | OK: example patches belong in `examples/`, as in RNBO, mira, max-mxj, Node for Max, spat5 and ABL. `patchers/` is for abstractions users instantiate by name; Zerr ships none. The overview's `load <name>.maxpat` finds examples through the search path |
| `externals/` | user guide | OK: the four `.mxo` |
| `help/` one help patch per object | tutorial part 3 | OK: four, all referenced files present |
| `docs/` reference pages | tutorial part 3 | OK: four hand-written `.maxref.xml` (2026-10-16) |
| `extras/`, `examples/`, `media/`, `misc/` | user guide | OK, but three of the seven examples are placeholders (see ID 171 sub-item) |
| Binaries: architectures | no official rule | **decision**: arm64 only (`lipo`), see platform row |
| Binaries: signing | no official rule | **decision**: ad-hoc (`codesign` identity `-`, no runtime option, no timestamp); `spctl -a -t install` says *rejected*; users need the README's `xattr` step |
| Binaries: notarization | no official rule | **decision**: none (`stapler validate`: no ticket) |
| Bundle `Info.plist` | general macOS hygiene | **gap**: `CFBundleIdentifier` is the literal `.${PRODUCT_NAME:rfc1034identifier}`, `CFBundleShortVersionString`, `CFBundleVersion`, `NSHumanReadableCopyright` empty, no `LSMinimumSystemVersion` |
| Release zip | the form wants a download link | **gap**: `__MACOSX/._*` AppleDouble tree and `Zerr/.DS_Store` ship inside the zip; a stale `Zerr-0.3.0-macos-arm64.zip` lingers in `maxmsp/dist/` |
| Release notes | our rule 5 in `release-process.md` | OK: `docs/releases/max-v0.3.0.md`; its *Known limitations* must follow the decisions below |
| Toolbar icon, object icons, `source/` | user guide (optional) | nice to have: none shipped |

## Gaps and where they live

### Must fix before submitting

1. **Package icon.** No `icon.png`. Make a 500x500 PNG from `assets/logo/` (the avatar variants are
   disc-masked already; `tools/make-logo-variants.sh` can emit it) into `maxmsp/icon.png`, and add it
   to the `install(FILES …)` line in `maxmsp/CMakeLists.txt`. Add `"displayname": "Zerr*"` to
   `maxmsp/package-info.json`. Board: **ID 178**.
2. **Platform claims.** `package-info.json` promises Intel macOS and Windows. Either build them or
   stop promising: a universal build needs universal `fftw`/`yaml-cpp` (two Conan profiles and
   `lipo`, `maxmsp/CMakeLists.txt:18-31` only goes fat under the Xcode generator) and the core
   library built twice; Windows needs the MSVC profile `ci.yml:252-255` describes. Until then,
   remove the `windows` block, keep `x64` only because Max 9 refuses the package without it, and
   state arm64-only in the description and README (already there). Board: **ID 179**.
3. **Bundle metadata.** `max-sdk-base/script/max-package.cmake:17-55` fills the plist variables only
   when `package-info.json.in` exists, which 237eb00 removed. Set `AUTHOR_DOMAIN`,
   `PACKAGE_VERSION`, `COPYRIGHT_STRING` and a real `BUNDLE_IDENTIFIER` in `maxmsp/CMakeLists.txt`
   before the per-external targets (the version is already read as `ZERR_MAX_PACKAGE_VERSION`), or
   ship an own `Info.plist.in`; add `LSMinimumSystemVersion` from the deployment target.
   Board: **ID 180**.
4. **Zip hygiene.** `build.sh:453-460` and `ci.yml:307` use `ditto -c -k --sequesterRsrc`; drop
   `--sequesterRsrc` (or use `--norsrc`) so no `__MACOSX` tree is written, delete `.DS_Store`
   files before zipping, and make `build.sh -c maxmsp` remove every zip in `maxmsp/dist/`, not only
   `zerr-max-*.zip`. Board: **ID 182**.

### Decisions

5. **Signing and notarization.** Ad-hoc signed externals fail Gatekeeper, so every user runs the
   quarantine command. Developer ID signing (`MAX_SDK_CODESIGN_IDENTITY`, `--options runtime`,
   `--timestamp`) plus notarizing the release zip with `notarytool` removes that step but needs an
   Apple Developer account and CI secrets. Decide, then align the README's Install step and the
   release notes. Board: **ID 181**.
6. **Universal build and Windows** are part of item 2 (ID 179).

### Nice to have

7. Toolbar icon (`toolbar_icon` key or `misc/Zerr_toolbar.svg`, Max 9), `object-icons/<object>.svg`
   for the Object Browser, and whether to ship `source/` in the package (the repo is public anyway).
   Board: **ID 184**.

### Before the tag, not before submission

8. Three example patches are placeholders (`01-quad-basic`, `02-ring-8-trigger`,
   `03-custom-layout`); the package must not ship placeholders. Board: **ID 177**, under ID 171.

## Submission steps

1. Finish ID 171 (Max checklist, overview restyle, examples) and publish `max-v0.3.0` as in
   `docs/design/release-process.md`.
2. Close IDs 178 to 182 under parent 176; settle the decisions; keep *Known limitations* in the
   release notes true.
3. E-mail packages@cycling74.com with the questions the docs leave open: is ad-hoc signing
   acceptable, is arm64-only acceptable, zip or Git link. Board: **ID 183**.
4. Fill the form at https://cycling74.com/support/submit-packages: author, e-mail, website
   (`https://github.com/ringbuffer-org/Zerr`), the GitHub release asset URL of the Max zip, a
   summary (the package-info description plus the object list), and the readiness date.

## Housekeeping note

`docs/design/max-health-check-plan-2026-10-11.md`, `docs/design/paper-gap-audit-2026-09-04.md` and
this file are untracked in the working copy although `release-process.md` links the first one.
Whether to commit them is the owner's call.
