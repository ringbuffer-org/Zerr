# GitHub Pages Deployment — Deliberately Not Enabled

_Decision recorded 2026-08-19 · applies to `ringbuffer-org/Zerr`_

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

1. **Pages has never been enabled.** `GET /repos/ringbuffer-org/Zerr/pages` returns `404`.
2. **`actions/configure-pages` defaults to `enablement: false`**, which only *queries* for an
   existing site. With no site it fails:
   `Get Pages site failed ... HttpError: Not Found`.
3. **`enablement: true` also fails.** Creating a site hits `POST /repos/{owner}/{repo}/pages`,
   which requires **admin** repository permission — and `administration` is not among the scopes a
   workflow's `permissions:` block can grant to `GITHUB_TOKEN`. Verified in
   [run 32303359056](https://github.com/ringbuffer-org/Zerr/actions/runs/32303359056):
   `Create Pages site failed ... HttpError: Resource not accessible by integration`.

The `enablement: true` currently in `deploy-doxygen.yml` is therefore inert. It is harmless — once a
site exists the action's initial `Get` succeeds and it never reaches `Create` — and is left in place
so the workflow needs no edit if Pages is enabled later.

**No workflow-side change can resolve this.** The token category is wrong for the operation.

## How to enable it, once approved

Either one, then re-run the workflow:

- **Repository settings** — Settings → Pages → Source: **GitHub Actions**
- **API, with an admin-scoped token** —
  `gh api -X POST repos/ringbuffer-org/Zerr/pages -f build_type=workflow`

The workflow needs no changes; it will pass as written. Note that its `push` trigger is filtered to
`core/include/**` and `docs/**`, so a config-only change will not re-trigger it — dispatch it
manually (`gh workflow run "Deploy Doxygen documentation" --ref main`) to confirm.

The published site would be `https://ringbuffer-org.github.io/Zerr/`.
