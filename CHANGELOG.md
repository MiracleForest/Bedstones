# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Support for **Minecraft 1.26.60** with LeviLamina 26.60.x, on the dedicated server and on the client, which is now the version Bedstones is installed as by default. Minecraft 1.26.51 with LeviLamina 26.51.x is still supported and gets its own builds of every release, so nothing is lost by updating.
  - Bedstones is compiled against one LeviLamina line at a time, so each release now ships one download per line and side: `bedstones-server-win-x64-ll26.60.zip`, `bedstones-client-win-x64-ll26.60.zip`, `bedstones-server-win-x64-ll26.51.zip` and `bedstones-client-win-x64-ll26.51.zip`.

### Changed

- The package is now described by `lip.toml` of format 4 instead of `tooth.json`, so installing it needs [lip](https://github.com/LiteLDev/lip) **0.40.0** or later, or a LeviLauncher that reads it. Older releases stay installable with older lip.
- Each supported LeviLamina line is a variant named after it, with a server and a client build: `26_60` for 26.60.x and `26_51` for 26.51.x. The default variant, which servers were installed as, and `client`, which clients were installed as, are the newest line, so `lip update` and LeviLauncher keep working and simply move you to 26.60.
  - To stay on Minecraft 1.26.51, install `github.com/MiracleForest/Bedstones#26_51` (on a server or a client), or pick the `26_51` variant on the mod's page in LeviLauncher. lip treats it as another package, so uninstall the one you have first (`lip uninstall github.com/MiracleForest/Bedstones`, with `#client` on a client); your settings stay.
  - Your settings in the mod's `config/` folder are kept either way. A LeviLamina 26.60 server loads mods from `mods/` instead of `plugins/`, so when a server moves to 26.60, Bedstones moves to `mods/Bedstones/` with it: copy your settings from `plugins/Bedstones/config/` to `mods/Bedstones/config/`.
- The downloads are now lip mod archives: the files sit at the root of the zip instead of inside a `Bedstones/` folder. Extract one into `mods/Bedstones/` (`plugins/Bedstones/` on a LeviLamina 26.51 server), not into `mods/`.

## [0.1.2] - 2026-11-01

### Changed

- The repository moved to [MiracleForest/Bedstones](https://github.com/MiracleForest/Bedstones); links point there now.
- The lip package is now `github.com/MiracleForest/Bedstones`, and the [LeviMC Mods page](https://levimc.org/mods/github.com/MiracleForest/Bedstones/) moves with it. With lip 0.36.0 or newer, `lip update` moves an existing install to the new path. With an older lip, run `lip uninstall github.com/LiteLDev/Bedstones` and then `lip install github.com/MiracleForest/Bedstones` (add `#client` to both on a client). The config in `plugins/Bedstones/config/` (`mods/Bedstones/config/` on a client) is kept. Older versions stay installable at `github.com/LiteLDev/Bedstones`. Installing them by the new path needs lip 0.35.0 or newer.

## [0.1.1] - 2026-10-24

### Fixed

- Commands that take a position (such as `/waystones activate <player> <x y z>`) found the wrong block at negative coordinates and answered "Waystone not found at this position".

### Added

- A logo, and screenshots and a video of waystones, sharestones, warp plates and teleporting in the [LeviMC Mods gallery](https://levimc.org/mods/github.com/LiteLDev/Bedstones/?tab=gallery).

### Changed

- A shorter README written for players. Notes for developers moved to [CONTRIBUTING.md](CONTRIBUTING.md).

## [0.1.0] - 2026-10-24

First release. Bedstones brings the behaviour of the Java mod Waystones 21.1.46 to Minecraft Bedrock Edition 1.26.51 with LeviLamina 26.51.x. It runs on the dedicated server and on the client.

### Added

- **Waystones** in 10 styles: waystone, mossy, sandy, deepslate, black, ender, nether, purpur, prismarine and muddy.
  - Activate one by right-clicking it, then warp between activated waystones.
  - Name a waystone when you place it. Its visibility can be activation, team or global.
  - Owner and edit permissions follow Waystones' rules.
  - Glowing runes and particles appear only for players who activated the waystone.
- **Sharestones** in 15 colours. Each one links to every sharestone of its colour, with no activation needed.
- **Portstones** in 16 colours. They open a selection menu but are not destinations themselves.
- **Warp plates** with dormant, attuned, crumbling and deepslate shards.
- **Warp portals**, opened by the portal scroll.
- **Fleeting memorials**, left by the Epitaph.
- **Warp modifiers**: a 4-slot modifier container on waystones and warp plates, with every Waystones modifier.
- **Items:**
  - warp stone;
  - warp, return, blank, bound and portal scrolls;
  - warp dust;
  - Twinbound Feather;
  - Epitaph.

  Recipes, durability and charge times match Waystones.
- **Warp requirements**: XP costs and cooldowns through Waystones' rule language (`warpRequirements`), with the same defaults. The selection list shows each destination's cost.
- **Teleport rules**: pets, leashed mobs and the entity deny list.
- **Screens**, drawn natively with the Java look by the client mod:
  - waystone selection, with sorting, groups and favorites;
  - Manage Waystones;
  - Personal Settings;
  - Manage Groups and Edit Group;
  - Edit Waystone;
  - Warp Modifiers;
  - the inventory button.
- **Form screens** for players without the client mod, so vanilla clients can join a Bedstones server and use every feature.
- **World generation**:
  - wild waystones, styled by biome;
  - village waystones;
  - generated names (`{MrPork}` template and presets).
- **Commands**: `/waystones activate | forget | place | count | list | gui | twinbound | cooldown | reload`.
- **Configuration** with Waystones' option names, as one JSON file per section in the mod's `config/` folder.
- **Translations**: English and Simplified Chinese.
- **Resource pack on servers**: a dedicated server adds the behavior pack and resource pack to the world, and joining players download the textures.

[Unreleased]: https://github.com/MiracleForest/Bedstones/compare/v0.1.2...HEAD
[0.1.2]: https://github.com/MiracleForest/Bedstones/compare/v0.1.1...v0.1.2
[0.1.1]: https://github.com/MiracleForest/Bedstones/compare/v0.1.0...v0.1.1
[0.1.0]: https://github.com/MiracleForest/Bedstones/releases/tag/v0.1.0
