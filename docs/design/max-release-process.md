# Max package: release process

_Maintainer steps for shipping `maxmsp/` as a Max package. User-facing instructions live in
`maxmsp/README.md`; this file is about producing the zip and checking it before it leaves._

## 1. Before building

- The release notes must open with a **Breaking** section when anything below applies. Pending as
  of 2026-10-12: the Max objects lost their `mc.` prefix (PR #21), and `root` combination mode now
  computes the geometric mean instead of behaving like `max` (PR #19).
- Bump `version` in `maxmsp/package-info.json`. It is the only place the version lives: CMake reads
  it, every external is compiled with it as `ZERR_MAX_VERSION`, and the zip name carries it.
- Restyle `maxmsp/extras/Zerr_Overview.maxpat` in Max if it has not been done since the last text
  change; commit the re-saved file as-is.
- `maxmsp/README.md` describes the objects as they ship. Re-read the object reference against the
  current `class_new` / message tables after any wrapper change (argument defaults, error wording,
  accepted message forms).

## 2. Build and assemble

```bash
./build.sh -c maxmsp            # drops build/, externals/, dist/ and stray help/*.mxo
./build.sh core
./build.sh -p maxmsp            # builds, then cmake --install → maxmsp/dist/Zerr + ditto zip
```

Expected: `maxmsp/dist/Zerr/externals/` holds exactly `zerr.combinator~.mxo`, `zerr.disperser~.mxo`,
`zerr.envelopes~.mxo`, `zerr.features~.mxo`; `dist/` holds `Zerr-<version>-macos-arm64.zip` with a
top-level `Zerr/` folder. CMake warns if hand-copied bundles are left in `maxmsp/help/`.

Checks:

```bash
codesign --verify --deep --strict maxmsp/dist/Zerr/externals/*.mxo
lipo -archs 'maxmsp/dist/Zerr/externals/zerr.features~.mxo/Contents/MacOS/zerr.features~'   # arm64
unzip -l maxmsp/dist/Zerr-*.zip | head
```

CI (`maxmsp (macos)` job) performs the same assembly and uploads the zip as `zerr-max-macos-arm64`;
the artifact is a zip containing the release zip, because upload-artifact does not keep file
permissions.

## 3. Test the package, not the tree

Max loads whatever it finds first on its search path. Test from the assembled package only:

1. Quit Max.
2. Remove any older `~/Documents/Max 9/Packages/{Zerr,zerr,maxmsp}`.
3. `./build.sh -l maxmsp` (symlink) or `./build.sh -i maxmsp` (copy) to put it into `~/Documents/Max 9/Packages/`.
4. Start Max; in each help patch send `bang` to the object and confirm the Console shows the new
   version string. That is the stale-binary detector.
5. Once per release, download the CI artifact through a browser, unzip, confirm `xattr -l` shows
   `com.apple.quarantine` on the bundles, and confirm the README's `xattr -dr` line makes them load.

Then run the scenario checklist in `docs/design/max-health-check-plan-2026-10-11.md`
(§ Verification checklist). The combinator mode switch, the envelopes layout lookup, and the plain
default-argument chain are the items most likely to regress.

## 4. Ship

```bash
git tag -a v<version> -m "Zerr* <version>"
git push origin v<version>
gh release create v<version> maxmsp/dist/Zerr-<version>-macos-arm64.zip --notes-file <notes>
```

Attach the PureData zips to the same release if they changed. Send the composer the *Install* and
*Troubleshooting* sections of `maxmsp/README.md` with the link.

## 5. After shipping

- Drain the "Breaking" items above that went out.
- Move the Zerr* Tasks board rows that the release closed to Done, with the tag as evidence.
