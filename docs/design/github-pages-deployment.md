# GitHub Pages Deployment — Deliberately Not Enabled

_Decision recorded 2026-08-19 · revised 2026-08-25 after Pages was briefly enabled and turned back
off · applies to `ringbuffer-org/Zerr`_

## Decision

**GitHub Pages is not enabled for this repository, and the failing `Deploy Doxygen documentation`
workflow is accepted rather than fixed.**

Enabling Pages publishes a new page under the organization's website
(`ringbuffer-org.github.io/Zerr`). That is the organization owner's call, and it should not happen
as a side effect of a CI change. Enabling it requires notifying the owner first.

Do not "fix" the red Doxygen job without that approval.

## Current behaviour

`Deploy Doxygen documentation` fails on every push to `main` touching `core/include/**` or
`docs/**`, so `main` shows a recurring red X.

Only the deploy fails. Doxygen itself is healthy:

| Step | Result |
| --- | --- |
| Install Doxygen | success |
| Generate documentation | **success** |
| Configure Pages | **failure** |
| Upload Pages artifact | skipped |
| Deploy to GitHub Pages | skipped |

The documentation builds correctly and is then discarded. Nothing is broken behind the failure —
running `doxygen docs/Doxyfile` locally produces the full HTML under `docs/build/html`.

## Why the workflow cannot fix this itself

This was attempted and does not work. The findings, so nobody re-derives them:

1. **Pages is not enabled.** `GET /repos/ringbuffer-org/Zerr/pages` returns `404`. (It was enabled
   between 2026-08-19 and 2026-08-25 — see the postmortem below — and has since been turned off
   again, restoring the `404`.)
2. **`actions/configure-pages` defaults to `enablement: false`**, which only *queries* for an
   existing site. With no site it fails:
   `Get Pages site failed ... HttpError: Not Found`.
3. **`enablement: true` also fails.** Creating a site hits `POST /repos/{owner}/{repo}/pages`,
   which requires **admin** repository permission — and `administration` is not among the scopes a
   workflow's `permissions:` block can grant to `GITHUB_TOKEN`. Verified in
   [run 32303359056](https://github.com/ringbuffer-org/Zerr/actions/runs/32303359056):
   `Create Pages site failed ... HttpError: Resource not accessible by integration`.

**No workflow-side change can resolve this.** The token category is wrong for the operation.

The `enablement: true` currently in `deploy-doxygen.yml` is inert, and is left in place so the
workflow needs no edit once Pages is enabled out-of-band. It is *not*, however, a safety net: see
the next section for what it fails to do.

## Postmortem — the 2026-08-24 green run that served the README

Between 2026-08-19 21:20 UTC and 23:36 UTC, Pages was enabled by hand via **Settings → Pages →
Source: "Deploy from a branch"**. Nothing in this repository requested that; it was an out-of-band
click. Two things followed, and neither was what it looked like.

### The workflow went green without being fixed

[Run 32789021466](https://github.com/ringbuffer-org/Zerr/actions/runs/32789021466) (push of
`87d6bf0`, the #18 merge) passed end to end. `Configure Pages` logged no warning and no error.

The workflow had not changed. `configure-pages` *queries* for a site before it tries to create one;
once the site existed the query succeeded, so the admin-only `POST` — the call that had been
failing — was simply never reached. **The precondition changed, not the workflow.** Point 3 above
still stands unaltered.

It surfaced on 08-24 rather than 08-19 only because the workflow's `paths:` filter needs
`core/include/**` or `docs/**`, and `87d6bf0` was the first qualifying push afterwards.

### The published site was the repository README, not the Doxygen output

Because the source was **"Deploy from a branch"**, GitHub also runs its own legacy Jekyll builder
(`pages-build-deployment`, `actions/jekyll-build-pages@v1`) on every push to `main`. On `87d6bf0`
both ran concurrently and raced for the same `github-pages` environment:

| Time (UTC) | Event |
| --- | --- |
| 23:22:15 | `Deploy Doxygen documentation` **and** `pages-build-deployment` both start |
| 23:22:44 | Doxygen `deploy-pages` reports success |
| 23:22:47 | Doxygen deployment goes **live** |
| 23:22:54–23:23:02 | legacy Jekyll `deploy` job runs |
| 23:23:03 | Jekyll deployment goes **live** |
| 23:23:04 | the earlier deployment is marked **inactive** |

The Doxygen deploy was correct — its artifact held 213 files (`annotated.html`, `hierarchy.html`,
`files.html`, every `classzerr_1_1*` page). **It lost the race by 16 seconds and was deactivated.**

The winner published the raw repository root: `index.html` rendered from `README.md`, plus
`CLAUDE.html`, `build.sh`, `conanfile.txt`, `configs/*.yaml`, and every submodule's documentation
(`puredata/pd-lib-builder/tests/*/index.html`, `maxmsp/source/min-api/doc/*.html`).

### What this adds to point 3

`enablement: true` does not merely fail to *create* a site. It also fails to switch an existing
site's `build_type` from `legacy` to `workflow` — that `PUT` needs admin as well — which is exactly
why the Jekyll builder kept running alongside the Doxygen deploy. The setting neither provisions nor
configures; it only stops erroring once a human has done the work in Settings.

So **"enable Pages" is not sufficient. The source must be "GitHub Actions".** Enabling it in branch
mode produces a green workflow and a wrong website, which is worse than the honest red X.

### Diagnostic trap: the Doxygen landing page also looks like the README

`docs/Doxyfile` sets `INPUT = core/include/ README.md` and `USE_MDFILE_AS_MAINPAGE = README.md`, and
the generated artifact contains no `md_README.html` — the README is consumed *as* the Doxygen main
page and rendered into `index.html`. So README prose on the landing page does **not** by itself mean
the wrong builder won.

The tell is the chrome: the Doxygen page carries the treeview sidebar and links to `annotated.html`;
the Jekyll page is bare primer-themed markdown with no class documentation anywhere.

### Ruled out

The green run was **not** an approval by an organization admin. The `github-pages` environment has a
single `branch_policy` protection rule and **no required reviewers**, its `approvals` endpoint
returns `[]`, and the run went from `created_at` to `updated_at` in 32 seconds without ever parking.
Decisively, the earlier failures did not stall awaiting anyone — they executed and failed *inside*
the job with an API error, which an approval gate cannot produce.

Nor did it need an owner's intervention: the maintainer already holds `admin: true` on this
repository, so the Settings click was directly available. Who made it could not be established — the
`dynamic` builder runs are attributed to the pusher of the triggering commit, not to whoever changed
the setting, and `orgs/ringbuffer-org/audit-log` returns `404` (it needs both an Enterprise-plan
organization and a `read:audit_log` token), so that check could not be performed.

## How to enable it, once approved

Either one, then re-run the workflow:

- **Repository settings** — Settings → Pages → Source: **GitHub Actions**
- **API, with an admin-scoped token** —
  `gh api -X POST repos/ringbuffer-org/Zerr/pages -f build_type=workflow`

> **The source must be "GitHub Actions", not "Deploy from a branch".** Branch mode leaves GitHub's
> legacy Jekyll builder running, which will race this workflow and overwrite the documentation with
> a rendering of the repository root. See the postmortem above.

The workflow needs no changes; it will pass as written. Note that its `push` trigger is filtered to
`core/include/**` and `docs/**`, so a config-only change will not re-trigger it — dispatch it
manually (`gh workflow run "Deploy Doxygen documentation" --ref main`) to confirm.

Verify afterwards that no `pages-build-deployment` run appears alongside it; one appearing means the
source is still branch mode.

```bash
gh api "repos/ringbuffer-org/Zerr/actions/runs?event=dynamic" --jq '.total_count'   # expect 0 new
```

The published site would be `https://ringbuffer-org.github.io/Zerr/`.
