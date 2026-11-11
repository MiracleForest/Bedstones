# Contributing to Bedstones

Thanks for helping! Bug reports, ideas and pull requests are all welcome.

## Reporting bugs and requesting features

Use the [issue templates](https://github.com/MiracleForest/Bedstones/issues/new/choose). For bugs, include:

- the Bedstones version;
- the LeviLamina version;
- the Minecraft version;
- whether it happens on a dedicated server, in a local world, or with a vanilla client;
- steps to reproduce;
- the relevant part of the server console or `logs/latest.log`.

## Development setup

See [Development](#development) below. [AGENTS.md](AGENTS.md) is the developer guide. It covers the layout of the code, the feature units and the contracts between them, the registration macros and the rules for working with Bedrock internals. Please read it before changing anything under `src/`.

## Pull requests

1. Fork the repository and create a branch from `main`.
2. Keep each pull request focused on one change. Describe what it does and how you tested it.
3. Follow the existing style: C++20, the provided `.clang-format` and `.editorconfig`, English in code and comments.
4. User-facing text goes through `bs::tr` / `bs::trFor` / `bs::client::ctr` with entries in **both** `assets/lang/en_US/` and `assets/lang/zh_CN/`.
5. Logic in `src/core` must stay free of game headers and come with unit tests in `tests/`:
   `xmake build -y bs-tests; xmake run bs-tests`.
6. Gameplay changes come with selftests (`BS_REGISTER_SELFTEST`) and an end-to-end scenario in [tools/e2e](tools/e2e/README.md). Run the relevant scenarios, and `tools/e2e/server/run.ps1 -Scenario full` for anything that touches shared code.
7. Both platforms must build: `pwsh tools/build.ps1 -Target all -Tests`. CI must pass before a pull request is merged.
8. Never copy code or assets from the Java Waystones repository. It is a behaviour reference only.

Commit messages use the imperative mood ("Add warp plate cooldown", not "Added …"). Update [CHANGELOG.md](CHANGELOG.md) under *Unreleased* for user-visible changes.

## Development

### Building

You need Windows x64, [xmake](https://xmake.io), Visual Studio Build Tools with MSVC 14.4x or newer, and LLVM (`clang-cl`).

```powershell
git config --global core.longpaths true
pwsh tools/build.ps1 -Target all -Tests     # bin/server/Bedstones/ and bin/client/Bedstones/, plus unit tests
pwsh tools/build.ps1 -Target client         # one platform only
pwsh tools/build.ps1 -LeviLamina 26.51.7    # another LeviLamina line (the default is the newest)
```

One code base builds both the dedicated-server mod and the client mod (`--target_type=server|client`). The mod version is the `version` in [manifest.json](manifest.json).

Bedstones is also compiled against one LeviLamina line at a time, chosen with `--levilamina` (`-LeviLamina` of `tools/build.ps1`). Each supported line has a lip variant of the same name and a lock file in `locks/`, which pins the packages of that line; `tools/build.ps1` copies it to `xmake-requires.lock` (which is generated, and not tracked) before configuring:

| Line | Variant | Lock file | Configure with |
| --- | --- | --- | --- |
| LeviLamina 26.60.x (Minecraft 1.26.60) | `26_60` | `locks/26_60.lock` | `--levilamina=26.60.6` (the default) |
| LeviLamina 26.51.x (Minecraft 1.26.51) | `26_51` | `locks/26_51.lock` | `--levilamina=26.51.7` |

If you configure with `xmake f` yourself, copy the line's lock file to `xmake-requires.lock` first; otherwise the packages are resolved unpinned (configuring warns when there is no `xmake-requires.lock`), and the generated lock stays until you replace it.

The build defines `BS_LL_VERSION` as the line it is building for (`2651`, `2660`). Where a LeviLamina API differs between lines, guard both paths with it rather than dropping the older one, and keep the guard as small as possible:

```cpp
#if BS_LL_VERSION >= 2660
    // 26.60 and later
#else
    // 26.51
#endif
```

To add a line, add its row above: a `locks/<variant>.lock` (configure once with the new `--levilamina` and copy the generated `xmake-requires.lock`), a variant in `lip.toml`, a matrix entry in `.github/workflows/build.yml` and `.github/workflows/release.yml`, and its files in the release step of `release.yml`.

### Tests

- Unit tests (`src/core`): `xmake build -y bs-tests; xmake run bs-tests`.
- In-game selftests: `/waystones selftest ["glob"]` (operator or server console).
- End-to-end: [tools/e2e](tools/e2e/README.md) runs the real server and client, headless through [LeviHeadlessInput](https://github.com/LiteLDev/BeDD). The `showcase` client scenario re-shoots the screenshots and videos in [docs/showcase](docs/showcase/) (listed in `showcase.json` for the LeviMC Mods gallery). It saves PNGs and the teleport clip's frames (with their real durations) to `tools/e2e/client/out/showcase/`; encode them with ffmpeg, for example:

  ```powershell
  ffmpeg -i waystone.png -c:v libwebp -quality 85 docs/showcase/waystone.webp
  cd frames-teleport
  ffmpeg -f concat -safe 0 -i frames.txt -vf "fps=30,format=yuv420p" -c:v libx264 -crf 26 -movflags +faststart -an teleport.mp4
  ffmpeg -f concat -safe 0 -i frames.txt -vf "trim=0.3:7.8,setpts=PTS-STARTPTS,fps=12,scale=720:405" -c:v libwebp -quality 42 -loop 0 teleport.webp   # README
  ```

### How it works

- [AGENTS.md](AGENTS.md) is the developer guide: the layout of the code, the feature units and the contracts between them, the registration macros and the rules for working with Bedrock internals. Please read it before changing anything under `src/`.
- [docs/SPEC.md](docs/SPEC.md) describes the Waystones behaviour Bedstones reproduces (blocks, items, mechanics, every screen, configuration, commands, strings).
- Bedstones copies no code or assets from Waystones. Its textures are original pixel art drawn by `tools/textures/*.ps1`.

### Maintainers

[@futrime (Zijian Zhang)](https://github.com/futrime)

## Releasing (maintainers)

1. In [CHANGELOG.md](CHANGELOG.md), move the *Unreleased* entries under a new `## [x.y.z] - YYYY-MM-DD` section and update the links at the bottom. Set `version` in [lip.toml](lip.toml) and in [manifest.json](manifest.json) to `x.y.z`, and commit it as `Release x.y.z`.
2. Merge that into `main`. Then run every server and client e2e scenario against `main`.
3. Tag the merge commit `vx.y.z` and push the tag. The *Release* workflow:
   - builds both platforms for every supported LeviLamina line and runs the unit tests;
   - checks that the mod manifest and `lip.toml` carry the tag's version;
   - publishes `bedstones-<server|client>-win-x64-ll<line>.zip` for every line, packed the way lip's `mcmod` asset wants it (`manifest.json` at the root of the zip), with that version's CHANGELOG section as the release notes. A version with a `-` in it, such as `0.2.0-beta.1`, is published as a prerelease.

The behavior and resource packs only carry `[major, minor, patch]`, so a prerelease and the release after it (`0.2.0-beta.1` and `0.2.0`) give the packs the same version. Clients cache a server's resource pack by uuid and version, so a client that joined a server running the prerelease could keep its old textures. If the packs changed since the last prerelease, release the next patch version instead (`0.2.1`, not `0.2.0`).

## License

By contributing you agree that your contributions are licensed under the [LGPL-3.0-or-later](COPYING.LESSER), the license of this project.
