# Releases: process

_Maintainer steps. User-facing install instructions live in `maxmsp/README.md` and
`puredata/README.md`; this file is about producing a release and checking it before it leaves._

## Rules (both hosts)

1. **A release is per host.** Tag `max-v<ver>` or `pd-v<ver>`. One GitHub release per tag, titled
   "Zerr* for Max <ver>" / "Zerr* for Pure Data <ver>", carrying only that host's zips and notes.
   Core changes are described, in user terms, in whichever host release ships them. GitHub's
   "Latest" badge is whichever host shipped last; always link by tag.
2. **Each host owns its version**, where its ecosystem expects it: Max in `maxmsp/package-info.json`
   (Max reads it; CMake compiles it in as `ZERR_MAX_VERSION`), Pd in `puredata/zerr-meta.pd` (the
   libdir/deken meta file; the Makefile compiles it in as `ZERR_PD_VERSION`). The numbers may
   diverge; a host with no changes gets no release. `core/CMakeLists.txt` keeps an unused 0.1.0:
   the core is not released on its own (rule 7).
3. **Zip names say the host**: `zerr-max-<ver>-macos-arm64.zip`, `zerr-pd-<ver>-<os>-<arch>.zip`
   (`macos-arm64`, `linux-x86_64`, `windows-x86_64`). `./build.sh -p <host>` makes the same files
   CI does. The folder inside the Max zip stays `Zerr`.
4. **CI makes the assets, a human publishes.** Pushing a release tag runs CI for that host only,
   checks the tag against the host's version file, and creates a **draft** release with the zips
   attached and the notes pre-filled (the `release` job in `.github/workflows/ci.yml`). Read the
   draft on GitHub, then publish it. A tag with a suffix (`max-v0.3.0-rc1`) becomes a prerelease
   draft: that is the dry run. `gh release create` refuses an existing release, so a re-run after a
   failure needs `gh release delete <tag> --yes` first.
5. **Notes live in the repo and stay**: `docs/releases/<tag>.md`, written before tagging, kept
   afterwards as the changelog. No H1 (the release title supplies it). `## Breaking` first when
   anything applies; drain the pending items listed under the host below.
6. **The version file, the notes and the tag name agree**, or the `release` job fails on purpose.
7. **The core has no version; core changes reach users through host releases, and are written
   down when they land.** The core is a static library compiled into each host's externals, so a
   user can only receive it inside a host package, and a host tag already pins the exact core
   commit. A core number nobody can download would be a third thing to keep in sync by hand.
   Instead:
   - A PR that changes `core/` says, in its body, what a user of each host will notice: one line
     for Max, one for Pd, or "none" (a refactor). The same change can be a fix for one host and a
     new feature for the other; the prepare contract (#37–#42) fixed a `[block~]` crash in Pd and
     a DSP-chain race in Max.
   - Each host's version bump follows that line, ordinary semver per host: patch for a fix, minor
     for a new feature, message or attribute, major when existing patches would break (object
     renamed, argument meaning changed, outlet order).
   - The same PR adds that line to the host's pending notes file, `docs/releases/<host>-next.md`,
     under `## Breaking`, `## Changed` or `## Fixed`. Once the host's version number is decided
     (the version file is bumped), the file is renamed to `docs/releases/<host>-v<ver>.md`, which
     is the file the `release` job reads. A core change therefore cannot be forgotten by a host:
     it was recorded when it landed, and the Pd file keeps accumulating while only Max ships.
   - Before tagging, check the notes against git rather than memory. `<last>` is the host's
     previous tag (`v0.2` for Pd until the first `pd-v` tag exists):
     ```bash
     git log <host>-v<last>..HEAD --oneline -- core <maxmsp|puredata>
     ```
     Every user-visible commit in that list has a line in the notes, or the notes are not done.
   - Nothing forces lockstep. A host whose pending file is empty gets no release, however much
     the core moved for the other host.

### Ship (either host)

```bash
# <host> is max or pd; the version file already says <ver>; docs/releases/<host>-v<ver>.md is on main
git checkout main && git pull
git tag -a <host>-v<ver> -m "Zerr* for <Max|Pure Data> <ver>"
git push origin <host>-v<ver>
gh run watch                                  # core, the host's job, release
gh release view <host>-v<ver>                 # draft: assets and notes
gh release edit <host>-v<ver> --draft=false   # after reading it on GitHub
```

Dry run first when anything in the release path changed:

```bash
git tag <host>-v<ver>-rc1 && git push origin <host>-v<ver>-rc1
gh release view <host>-v<ver>-rc1 --json isDraft,isPrerelease,assets
# download an asset through a browser and test it like a user would, then:
gh release delete <host>-v<ver>-rc1 --yes --cleanup-tag && git tag -d <host>-v<ver>-rc1
```

### After shipping (either host)

- Drain the "Breaking" items that went out from the host's checklist below.
- Move the Zerr* Tasks board rows that the release closed to Done, with the tag as evidence.
- Send the users the *Install* and *Troubleshooting* sections of the host README with the link.

## Max checklist

### Before building

- The release notes must open with a **Breaking** section when anything below applies. Pending as
  of 2026-10-12: the Max objects lost their `mc.` prefix (PR #21), and `root` combination mode now
  computes the geometric mean instead of behaving like `max` (PR #19).
- Keep `"x64"` in `os.macintosh.platform` of `package-info.json`, whatever the docs say about it
  meaning Intel. Max 9 looks up the block for the running OS and requires `x64` there, otherwise it
  refuses the whole package with "Could not load package 'Zerr', 64-bit architecture unsupported"
  (seen 2026-10-12 after #23 listed only `aarch64`). The Max binary knows the tokens `x64`, `ia32`
  and `arm64`; `aarch64` does not occur in it, so it is ignored. The `windows` block is not consulted
  on macOS; it is kept so the file is ready for a future Windows build. The README states the real
  requirement (Apple Silicon, native).
- Bump `version` in `maxmsp/package-info.json`. It is the only place the Max version lives: CMake
  reads it, every external is compiled with it as `ZERR_MAX_VERSION`, and the zip name carries it.
- Restyle `maxmsp/extras/Zerr_Overview.maxpat` in Max if it has not been done since the last text
  change; commit the re-saved file as-is.
- `maxmsp/README.md` describes the objects as they ship. Re-read the object reference against the
  current `class_new` / message tables after any wrapper change (argument defaults, error wording,
  accepted message forms).
- The four reference pages in `maxmsp/docs/*.maxref.xml` say the same as the README reference.
  They are hand-written, including `zerr.disperser~`'s (Min-API generation is switched off for it
  with `MIN_FLAGS`), so a wrapper change means editing the page too.
- `docs/releases/max-v<ver>.md` covers every user-visible commit in
  `git log max-v<last>..HEAD --oneline -- core maxmsp` (rule 7). For 0.3.0 there is no earlier
  `max-v` tag: the notes cover everything since the Max package was created.

### Build and assemble

```bash
./build.sh -c maxmsp            # drops build/, externals/, dist/ and stray help/*.mxo
./build.sh core
./build.sh -p maxmsp            # builds, then cmake --install → maxmsp/dist/Zerr + ditto zip
```

Expected: `maxmsp/dist/Zerr/externals/` holds exactly `zerr.combinator~.mxo`, `zerr.disperser~.mxo`,
`zerr.envelopes~.mxo`, `zerr.features~.mxo`; `dist/` holds `zerr-max-<version>-macos-arm64.zip` with a
top-level `Zerr/` folder. CMake warns if hand-copied bundles are left in `maxmsp/help/`.

Checks:

```bash
codesign --verify --deep --strict maxmsp/dist/Zerr/externals/*.mxo
lipo -archs 'maxmsp/dist/Zerr/externals/zerr.features~.mxo/Contents/MacOS/zerr.features~'   # arm64
unzip -l maxmsp/dist/zerr-max-*.zip | head
```

CI (`maxmsp (macos)` job) performs the same assembly and uploads the zip as `zerr-max-macos-arm64`;
the artifact is a zip containing the release zip, because upload-artifact does not keep file
permissions.

### Test the package, not the tree

Max loads whatever it finds first on its search path. Test from the assembled package only:

1. Quit Max.
2. Remove any older `~/Documents/Max 9/Packages/Zerr`.
3. `./build.sh -l maxmsp` (symlink) or `./build.sh -i maxmsp` (copy) to put it into `~/Documents/Max 9/Packages/`.
4. Start Max; in each help patch send `bang` to the object and confirm the Console shows the new
   version string. That is the stale-binary detector.
5. Once per release, download the CI artifact through a browser, unzip, confirm `xattr -l` shows
   `com.apple.quarantine` on the bundles, and confirm the README's `xattr -dr` line makes them load.

Then run the scenario checklist in `docs/design/max-health-check-plan-2026-10-11.md`
(§ Verification checklist). The combinator mode switch, the envelopes layout lookup, and the plain
default-argument chain are the items most likely to regress.

Then tag `max-v<ver>` as under *Ship* above.

## Pure Data checklist

### Before building

- Pending "Breaking" items for the next Pd release: none recorded. (The `mc.` rename and the
  `root` mode change were Max-only.)
- Rename `docs/releases/pd-next.md` to `docs/releases/pd-v<ver>.md` and check it against
  `git log v0.2..HEAD --oneline -- core puredata` (rule 7; `v0.2` is the last Pd tag).
- Bump `VERSION` in `puredata/zerr-meta.pd`. It is the only place the Pd version lives: the
  Makefile compiles it in as `ZERR_PD_VERSION`, pd-lib-builder reads it as `lib.version`, and the
  zip name carries it.
- `puredata/README.md` ships inside the package. Re-read it against the objects as they are.
- Re-read the four help patches in `puredata/help/` after any wrapper change.

### Build and assemble

```bash
./build.sh -c puredata
./build.sh core
./build.sh -p puredata     # make install into puredata/dist/zerr, then zip
```

Expected: `puredata/dist/zerr/` holds the four `zerr_*~.pd_darwin` (or `.pd_linux` / `.dll`),
the four help patches, `circulation_8.yaml`, `zerr-meta.pd`, `LICENSE`, `README.md`; `dist/` holds
`zerr-pd-<version>-<os>-<arch>.zip`. Linux and Windows zips come from CI (`zerr-pd-linux`,
`zerr-pd-windows-mingw` artifacts on the same commit), not from this machine.

### Test the package, not the tree

1. Quit Pd. Remove any older `zerr` folder from Pd's externals path.
2. `./build.sh -i puredata` (copy) or `-l puredata` (symlink).
3. Start Pd, open each help patch from the installed folder, turn DSP on. Once the Pd `bang`
   version report exists, send `bang` to each object and confirm the console shows the new
   version; until then, check the console for load errors and the help patches for sound.
4. Once per release, download the macOS CI artifact through a browser, unzip, and confirm the
   README's quarantine step makes the externals load.

Then tag `pd-v<ver>` as under *Ship* above.
