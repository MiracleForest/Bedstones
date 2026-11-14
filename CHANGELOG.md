# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.0] - 2026-11-14

First release. Bedstones adds waystones, sharestones, warp plates and warp items to Minecraft: Bedrock Edition for Windows (x64). It runs on the dedicated server and on the client, with LeviLamina 26.60.x (Minecraft 1.26.60, the default variant `26_60`) or LeviLamina 26.51.x (Minecraft 1.26.51, variant `26_51`).

### Added

- **Waystones** in 10 styles: waystone, mossy, sandy, deepslate, black, ender, nether, purpur, prismarine and muddy.
  - Name a waystone when you place it; shift + right-click it to rename it, change its visibility (activation, team or global) or open its warp modifiers.
  - Activate one by right-clicking it, then warp between activated waystones. Glowing runes and particles appear only for players who activated it.
  - Owners and permissions decide who may edit a waystone.
- **Sharestones** in 15 colours. Each one links to every sharestone of its colour, with no activation needed.
- **Portstones** in 16 colours. They open the waystone menu but are not destinations themselves.
- **Warp plates** with dormant, attuned, crumbling and deepslate shards.
- **Warp portals**, opened by the portal scroll, that carry everyone through to one destination for 5 minutes.
- **Fleeting memorials**, rounded headstones left where you die while carrying an Epitaph, which you can warp back to once.
- **Warp modifiers**: up to 4 on each waystone and warp plate, affecting whoever arrives.
- **Items:** warp stone; warp, return, blank, bound and portal scrolls; warp dust; Twinbound Feather; Epitaph.
- **Warp requirements**: XP costs and cooldowns through a configurable rule language (`warpRequirements`), with sensible defaults. The menu shows each destination's distance and cost.
- **Teleport rules**: pets, leashed mobs and an entity deny list.
- **Native screens** with a Java Edition look, drawn by the client mod: the waystone menu (search, sorting, groups and favourites), Manage Waystones, Personal Settings, Manage Groups and Edit Group, Edit Waystone, Warp Modifiers and the inventory button.
- **Form screens** for players without the client mod, so vanilla clients can join a Bedstones server and use every feature.
- **World generation**: wild waystones styled by biome, village waystones, and generated names.
- **Commands** under `/bedstones`: `activate`, `forget`, `place`, `count`, `list`, `gui`, `twinbound`, `cooldown` and `reload` for operators; `inventorybutton` and `companion` for everyone.
- **Settings** as one JSON file per section in the mod's `config/` folder, reloadable with `/bedstones reload`.
- **Translations**: English and Simplified Chinese.
- **Resource pack on servers**: a dedicated server adds the behavior pack and resource pack to the world, and joining players download the textures.
- **Downloads** for every supported line and side, installable with [lip](https://github.com/LiteLDev/lip) 0.40.0 or later on a server and with LeviLauncher on a client: `bedstones-server-win-x64-ll26.60.zip`, `bedstones-client-win-x64-ll26.60.zip`, `bedstones-server-win-x64-ll26.51.zip` and `bedstones-client-win-x64-ll26.51.zip`.

[Unreleased]: https://github.com/MiracleForest/Bedstones/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/MiracleForest/Bedstones/releases/tag/v0.1.0
