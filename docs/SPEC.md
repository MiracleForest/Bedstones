<!-- Behaviour reference. Bedstones mapping notes:
  - ids are `bedstones:<path>` (paths listed in game/Ids.h); integer block states `bedstones:facing` (Java 2D value), `bedstones:upper`, `bedstones:status`.
  - Concepts without a Bedrock equivalent (map mods, stats) are out of scope; see AGENTS.md.
-->
# Bedstones gameplay: feature specification

This document describes the gameplay of Bedstones: behaviour, rules and numbers. All player-facing text in Bedstones is our own wording.

- Bedstones' namespace is `bedstones`. Ids below are written without a namespace.

---

## 1. Blocks

### 1.1 Common properties of all waystone-like blocks
- **Strength:** hardness 5.0, blast resistance 2000. Must be mined with a pickaxe to drop.
- **Sound:** stone. Deepslate, red nether brick and mud brick styles use the matching vanilla sound.
- **Pistons:** cannot move these blocks.
- **Block states:** horizontal facing (opposite to the placer's facing), waterlogged, origin (unknown, wilderness, dungeon, village, player) and, for 2-tall blocks, lower/upper half.
- **Placement:**
  - Needs a replaceable block above, below the build limit.
  - The upper half is placed automatically, waterlogged if placed into water.
  - Neither half survives without the other; breaking either removes both.
- **When a player places one:**
  - A new waystone record is created (new UUID, owner = the placer, origin player).
  - It is activated for the placer.
  - The Edit screen opens. Warp plates and fleeting memorials skip this.
  - If the item came from a silk-touched waystone, the old record is restored instead (name, owner, activations and visibility kept).
- **Breaking:**
  - Survival breaking with a pickaxe drops one item (from the lower half only). Items in the modifier slots drop too. Creative breaking drops nothing.
  - **Without silk touch:** the record is deleted, every player loses it from their list, and everyone is told it was removed.
  - **With silk touch** (waystones and warp plates only): the dropped item remembers the record. The record stays but is hidden from global lists until the block is placed again.
  - Sharestones, portstones and fleeting memorials never keep their record.
- **No owner-based break protection.** Anyone with a pickaxe can break any waystone.
- **Observers:** on activation, adjacent observers facing the block emit a pulse.
- **Interaction:**
  1. **Shift + right-click** opens the Edit screen.
  2. Otherwise the block's normal use runs.
- **Blank Scroll:** using a blank scroll on the block binds the scroll instead of opening a menu.
- **Item tooltip:** a picked-up waystone item shows the waystone's name in aqua.
- **Creative debug helpers:** using a bamboo on a waystone resets it (it re-initialises when next loaded); using a stick prints its server and client UUIDs.

### 1.2 Waystone styles (2 tall)
All styles behave the same; the style only changes the look. Every style has the waystone type.

| Block id | Name | Recipe material "S" | Rune colour |
|---|---|---|---|
| `waystone` | Waystone | stone_bricks | white |
| `mossy_waystone` | Mossy Waystone | mossy_stone_bricks, or shapeless: waystone + 3 moss_block, or waystone + 3 vine | white |
| `sandy_waystone` | Sandy Waystone | chiseled_sandstone | white |
| `deepslate_waystone` | Deepslate Waystone | deepslate | white |
| `blackstone_waystone` | Black Waystone | blackstone | `0x993333` |
| `end_stone_waystone` | Ender Waystone | end_stone_bricks | `0x7200FF` |
| `red_nether_bricks_waystone` | Nether Waystone | red_nether_bricks | white |
| `purpur_waystone` | Purpur Waystone | purpur_block | white |
| `prismarine_waystone` | Prismarine Waystone | prismarine | white |
| `mud_bricks_waystone` | Muddy Waystone | mud_bricks | white |

- **Recipe (shaped):**
  ```
   S
  SWS
  OOO
  ```
  S = material, W = `warp_stone`, O = obsidian.
- **Collision:** a wide base that narrows towards the top; roughly a full block at the bottom and 10/16 wide at the top.
- **Mobs do not path through it.**
- **Runes:** a glowing rune overlay, tinted with the style's rune colour, drawn **only for players who have activated that waystone**. It is full-bright unless the client option `disableTextGlow` is on.
- **Ambient particles:** for players who have activated it, portal and enchant particles appear around the middle of the stone most ticks (about 75% chance per tick, within ±0.75 blocks).

### 1.3 Sharestones: 15 dyed variants
- **Ids:** `{orange, magenta, light_blue, yellow, lime, pink, gray, light_gray, cyan, purple, blue, brown, green, red, black}_sharestone`. There is no white sharestone.
- Each colour is its own waystone type and visibility.
- **Recipe:**
  ```
  SSS
  DWD
  OOO
  ```
  S = stone_bricks, D = dye of that colour, W = warp_stone, O = obsidian.
- **Collision:** like a waystone, but widening again at the very top.
- **Right-click:** lists **every sharestone of the same colour on the server**. No activation needed. Shift + right-click opens the Edit screen.
- **Tooltip:** says it teleports to any other sharestone of its colour (gray).
- **Look:** runes in the dye colour, always shown, and a small warp stone spinning and bobbing above the block (half size, one full turn every 36 seconds, bob amplitude 0.025).
- Every player is told about new sharestones, since everyone can see them.

### 1.4 Portstones: 16 colours including white
- **Ids:** `white_portstone` (named "Portstone") and `<color>_portstone` for the other 15 colours.
- **Recipe:**
  ```
  DSD
  SWS
  BBB
  ```
  D = dye (white dye for white), S = stone_bricks, W = warp_stone, B = polished_andesite.
- **Not a destination:** a portstone has no waystone record. Its tooltip says it cannot be teleported to.
- **Right-click (with or without shift):** opens a selection list.
  - White: the player's activated waystones plus global and team waystones.
  - Coloured: all sharestones of that colour.
- **Shape:** a 2-tall pedestal with a slanted, lectern-like top.
- **Look:** runes in the dye colour.

### 1.5 Warp Plate (1 tall)
- **Recipe:**
  ```
  SWS
  WFW
  SWS
  ```
  S = stone_bricks, W = warp_dust, F = dormant_shard.
- **Shape:** a full-width slab 1/16 tall with a raised 10×10 centre.
- **Status** (block state): empty, idle, attuning, warping, warping_invalid, redstone_disabled, locked.
- **Colour and name:**
  - Each plate has a short identifier: the first 8 letters of its UUID once digits and dashes are removed.
  - It is shown in the Standard Galactic Alphabet (vanilla `minecraft:alt` font).
  - The plate's colour is one of 15 chat colours picked from a hash of the identifier; gray, dark gray and black are replaced by light purple, dark purple and gold.
- **Naming:** gets a random name on placement. No menu opens on placement.
- **Visibility:** always "shard only".
- **Container:** 5 slots. Slot 0 holds the shard (one dormant shard at most). Slots 1–4 hold modifiers.
- **Interactions:**
  - Right-click with a warp shard while the shard slot is empty: inserts one shard (not consumed in creative).
  - Shift + right-click with an empty hand: ejects the shard.
  - Right-click: opens the Edit screen, which leads to the Warp Modifiers screen.
- **Attunement:**
  - A plate holding a **dormant shard** is attuning.
  - After **30 ticks** the shard becomes an `attuned_shard` bound to this plate and pops out of the plate (with a sound and a puff of particles).
  - The player puts that shard into **another** plate; that plate then sends things to the first one.
  - A deepslate shard locks the plate: it cannot send anything but can still be a destination.
- **Teleporting:**
  - An entity steps on the plate: it must not be riding, must not be on the deny list and must not be on cooldown for this plate.
  - If the plate is idle and has a target, it starts warping (or shows the "invalid" state if the target is gone or the player cannot pay).
  - Each tick on the plate counts up. Past `warpPlateUseTime` (default 15 ticks) the entity is sent to the target.
  - With several shards, the target is random. A **Quartz** modifier cycles through them in turn; a **Spider Eye** prefers crumbling shards.
  - A `crumbling_attuned_shard` is used up after one trip (not in creative).
- **Cooldown:** after arriving on a plate, an entity must step off for `warpPlateCooldownTime` (20 ticks) before that plate can send it back.
- **Action-bar errors** (dark red): the plate has no linked target yet; the shard in the plate no longer has a valid target.
- **Redstone:** with a redstone torch modifier, a powered plate is switched off (redstone_disabled).
- **Use-time modifiers:** each amethyst shard −1.6%, each slime ball +1.6%; the result is kept between 1 tick and twice the configured time.
- **Particles per status:**

  | Status | Particles |
  |---|---|
  | warping | 50 crimson spore + portal particles per tick, over 2 blocks of height |
  | warping_invalid | 10 smoke |
  | redstone_disabled | 1 redstone dust |
  | attuning | 10 warped spore + 10 soul fire flame |

- **Arrival:** on the plate itself, with no offset.

### 1.6 Warp Portal (no item; made by the Portal Scroll)
- **Shape:** 2 tall, no collision or outline. Light level 10. Unbreakable. No drops.
- **Animated texture.**
- **Lifetime:** 6000 ticks (5 minutes). Then it fizzles out (extinguish sound, 32 reverse-portal particles) and disappears.
- **Behaviour:**
  - Has a temporary waystone record of its own, listed as "Return to Portal".
  - Entities walking in are sent to the portal's target, with a 20-tick per-entity cooldown.
  - The portal is remembered as the entity's return portal.
  - If the target is gone, the player is told the portal closed and the portal is removed.
- **Where it opens:** 3–4 blocks ahead of the player horizontally, searching 5 blocks down from 2 above for a solid block with 3 free blocks over it.
- **Opening effects:** portal travel sound (volume 0.25, pitch 1.4), 96 portal and 48 reverse-portal particles.

### 1.7 Fleeting Memorial (no item; made by the Epitaph)
- **Block:** 1 tall, a thin gravestone that shows its owner's name. Hardness 2, resistance 6, light 6. No drops.
- **Particles:** 4 ash particles per tick at the top.
- **No edit actions** and no menu on placement.
- **Record:** type fleeting memorial, named "Fleeting Memorial", owned by the player who died.
- **Spawning:** when a player dies with an `epitaph` anywhere in the inventory:
  - The memorial goes one block above the death position, or the nearest free spot within 1–2 blocks sideways and 0–3 up.
  - One epitaph is used up.
  - The player sees the totem animation with the epitaph and hears the totem sound.
- **Use:** only its owner sees it as a destination. After a **successful** trip there it is removed.

---

## 2. Items

| Id | Name | Stack | Recipe | Behaviour |
|---|---|---|---|---|
| `warp_stone` | Warp Stone | 1, durability 128 (Unbreaking/Mending apply) | Shaped `DED / EGE / DED`: D = amethyst_shard, E = ender_pearl, G = emerald | Glint. Hold use for `warpStoneUseTime` (32 ticks), bow animation. Opens the selection list. Loses 1 durability after a successful warp if `enableDurability`. Taking damage cancels the charge. |
| `warp_scroll` | Warp Scroll | 64 | Yields 3. Shaped `GDG / GEG / PPP`: G = gold nugget, D = ink_sac, E = ender_pearl, P = paper | Charge for `scrollUseTime` (32 ticks), bow animation, quiet portal sound (volume 0.1, pitch 2). Opens the selection list. Used up after a successful trip. |
| `return_scroll` | Return Scroll | 64 | Yields 3. Shaped `GEG / PPP`: G = gold nugget, E = ink_sac, P = paper | Goes to the player's return portal if there is one, otherwise to the **nearest activated waystone in the same dimension**. Used up on success. Glint. |
| `blank_scroll` | Blank Scroll | 64 | Yields 3. Shaped `GFG / PPP`: G = gold nugget, F = feather, P = paper | Use on a waystone, sharestone or warp plate: one is used up and a `bound_scroll` for that block is given. The tooltip explains this. |
| `bound_scroll` | Bound Scroll | 64 | — | Glint. Tooltip shows the bound name in aqua. Charge 32 ticks, then go straight to that waystone. Used up on success. Errors go to the action bar in dark red. |
| `portal_scroll` | Portal Scroll | 64 | Yields 3. Shaped `SIS / SES / PPP`: S = amethyst_shard, I = ink_sac, E = ender_pearl, P = paper | Glint. After charging, fails if there is no room for a portal; otherwise opens the selection list, and picking a target opens a Warp Portal and uses up the scroll. |
| `warp_dust` | Warp Dust | 64 | Shapeless: ender_pearl + amethyst_shard gives 4 | Ingredient. |
| `dormant_shard` | Dormant Shard | 64 (1 in a plate) | Shapeless: 2 warp_dust + flint | Put it in a warp plate to attune it. |
| `attuned_shard` | Attuned Shard | 1 | (made by a plate) | Glint while its target exists. Tooltip: the target name (galactic name for plates), then a hint to put it in a plate, or a note that it has lost its target. |
| `crumbling_attuned_shard` | Crumbling Attuned Shard | 4 | **No recipe** (creative only) | Single use; the tooltip says so (white italic). |
| `deepslate_shard` | Deepslate Shard | 64 | Shapeless: deepslate + flint | Locks a warp plate. |
| `twinbound_feather` | Twinbound Feather | 1 | Shapeless: feather + amethyst_shard + gold_nugget + ink_sac | See §3.12. |
| `epitaph` | Epitaph | 1 | Shaped `GGG / ADA / GGG`: G = gold nugget, A = amethyst_shard, D = deepslate | Leaves a Fleeting Memorial on death. |

- **Item data:** a waystone item can carry its record's UUID; an attuned shard carries its target; a bound scroll carries the waystone id and name; a twinbound feather carries its own id and the ids it is linked to.
- **Creative tab order:** waystone, white portstone, red sharestone, warp plate, blank scroll, return scroll, warp scroll, portal scroll, warp stone, warp dust, dormant shard, twinbound feather, epitaph, deepslate shard, then the other waystone, sharestone and portstone variants, attuned shard, crumbling attuned shard.
- **Warp shards:** attuned, crumbling, dormant, deepslate. Only the crumbling one is single use.
- **Warp modifiers:** blaze_powder, poisonous_potato, ink_sac, milk_bucket, honey_block, diamond, feather, magma_cream, wither_rose, quartz, spider_eye, redstone_torch (redstone-sensitive), plus amethyst_shard and slime_ball for plate timing.
- **Valid destinations:** waystones, warp plates, portstones, sharestones, warp portals and fleeting memorials. A trip is refused if the destination block is no longer there.

---

## 3. Mechanics

### 3.1 Data
- **Waystone record:** uid, type, name, dimension, position, a "hidden until placed again" flag, origin, owner uid and name, visibility.
- **Per-player data:** activated waystones, manual sort order, sort mode, cooldowns (id → expiry time), aliases, the groups of each waystone, hidden waystones, and the player's groups (id, name, icon, colour, inbuilt, hidden, sort index).
- **Generated names** are kept unique per world: a repeated name gets a Roman numeral suffix (" II", " III", ...).
- **What a player sees:** the alias if set, otherwise the name.

### 3.2 Activation and discovery
- **Right-clicking a waystone you have not activated:**
  - Activates it for you and tells you so in chat (message yellow, name white).
  - Level-up sound at volume 0.2.
  - Two rings of 32 enchant particles fall from 3 and 4 blocks above.
  - Adjacent observers pulse.
- **Right-clicking an activated waystone:** opens the selection menu.
- **Only waystones need activation.** Sharestones and plates do not.
- **First activation of a generated waystone** (wild or village, still unnamed):
  - It gets a generated name (default template `{Syllables}`: random syllables, e.g. "Corenvale").
  - If it has no owner, the player who activates it becomes the owner and its visibility becomes the default.
- **Player-placed waystones are not named automatically.** Saving with an empty name gives "Untitled Waystone".

### 3.3 Visibility
- **Values:**
  - Activation: visible to players who activated it.
  - Global: listed for everyone, no activation needed.
  - Team: visible to the owner's scoreboard team.
  - Shard only: warp plates.
  - One per sharestone colour.
- **Choices in the Edit screen:**
  - Waystones offer the configured default, activation, team, and global (only for players who may manage global waystones).
  - Sharestones and plates: only their fixed visibility, so the button does nothing.
- **Global and team lists** are rebuilt when the server starts. Generated waystones that were never named are left out.

### 3.4 Ownership and permissions
- **Creative players can always edit.**
- **Editing is refused when:**
  - The waystone has an owner, `restrictedWaystones` contains PLAYER (the default), and you are not the owner.
  - Its origin is not player and that origin is in `restrictedWaystones`.
  - It is global and you may not manage global waystones.
- **Global waystone managers:** creative players, or everyone if `allowEveryoneToManageGlobalWaystones` is on, `defaultVisibility` is GLOBAL, or the deprecated `allowedVisibilities` contains GLOBAL.
- **When saving, the server:**
  - Checks the permission again.
  - Requires the player to be within 4 blocks.
  - Falls back to the first allowed visibility if the one sent is not allowed.
  - Turns an empty name into "Untitled Waystone".
  - Appends `*` if the name equals the inventory-button target name and a waystone with that name already exists.
- **If editing is refused**, the Edit screen still opens, read-only, with a red notice.

### 3.5 Teleporting
1. **Gather:** the entity, the destination, the source waystone, the warp item and hand, and whether the trip comes from the inventory button, a portstone or an admin menu.
   - Pets come along if `transportPets` allows it: tamed, owned by the player, within 10 blocks, not sitting, not leashed, not on the deny list.
   - Leashed mobs within 10 blocks come along.
   - Costs and cooldowns are worked out from `warpRequirements`.
2. **Check before starting:**
   - The entity is alive and still online.
   - The source waystone is in the same dimension and within reach (4 blocks for players, 8 for other entities).
   - The warp item is still held.
   - Leash rules: when leashed mobs may not come, when this mob is on the deny list, or when `SAME_DIMENSION` forbids taking them to another dimension, the trip is refused with a matching message.
   - The player can pay, or is in creative. Otherwise the trip is refused for lack of experience.
3. **Load the destination:** its chunk and the 4 neighbouring chunks, without blocking the server.
4. **Check again:** the same checks, plus the destination block must still exist; if it is gone the player is told the waystone has been moved or is missing.
5. **Pay, then move:**
   - **Arrival spot:** the block in front of the waystone (its facing), if it and the block above are not solid. Otherwise try east, west, south, north, then the waystone's own position. Centred in the block, facing away from the stone.
   - Mounts and passengers travel together and are re-mounted; leashes are re-attached.
   - Vertical speed is reset and the entity counts as on the ground, unless it is gliding with an elytra.
6. **Effects:**
   - Portal travel sound at the start (volume 0.01) and the destination (volume 0.05).
   - 128 portal particles within ±1.5 blocks at both ends.
   - The **destination's** modifiers are applied (§3.10).
7. **Errors** appear in dark red on the action bar.
- **Menus close** if the player moves more than 4 blocks away from the source waystone.

### 3.6 Costs and cooldowns
**Default rules, applied in order:**

| When | Effect |
|---|---|
| Same dimension | +1 XP point per 100 blocks |
| Different dimension | +27 XP points |
| From a warp plate, a scroll, to a global waystone, or to a fleeting memorial | cost × 0 (free) |
| Always | cost kept between 0 and 27 points |
| From the inventory button | 300-second cooldown `inventory_button` |

- Costs are **XP points**, not levels. 1000 blocks = 10 points.

**Rule syntax** (the `warpRequirements` config, which Bedstones accepts unchanged): one rule per entry, `[condition, condition(arg)] function(args)`.
- Ids without a namespace use `bedstones:`.
- Every condition also has a negated form: `not_` goes after the first `is_`, or in front if there is no `is_` (e.g. `is_not_interdimensional`).

**Functions:**
- Level cost: `add_level_cost(f)`, `multiply_level_cost(f)`, `scaled_add_level_cost(var, f)`, `scaled_multiply_level_cost(var, f)`, `min_level_cost(i)`, `max_level_cost(i)`.
- XP-point cost: `add_xp_cost(i)`, `multiply_xp_cost(f)`, `scaled_add_xp_cost(var, f)`, `min_xp_cost(i)`, `max_xp_cost(i)`.
- Cooldown: `add_cooldown(id, s)`, `multiply_cooldown(id, s)`, `scaled_add_cooldown(var, id, s)`, `scaled_multiply_cooldown(var, id, s)`, `min_cooldown(id, s)`, `max_cooldown(id, s)`.
- Soft cooldown (never blocks the trip, only sets the timer): the same six with `soft_cooldown`.
- Item cost: `add_item_cost(item, n)`, `multiply_item_cost`, `scaled_add_item_cost(var, item, n)`, `scaled_multiply_item_cost`, `min_item_cost`, `max_item_cost`.
- Other: `dismount()`, and `refuse(msg)` (always blocks the trip and shows a cancel icon; a message starting with `$` is a translation key).
- Switches: `enableCosts=false` turns off cost functions, `enableCooldowns=false` cooldown functions.

**Conditions:**
- Source: `source_name_equals(s)`, `source_name_contains(s)`, `source_is_warp_plate`, `source_is_portstone`, `source_is_waystone`, `source_is_sharestone`, `source_is_<color>_sharestone`, `source_is_inventory_button`, `source_is_scroll`, `source_is_bound_scroll`, `source_is_return_scroll`, `source_is_warp_scroll`, `source_is_portal_scroll`, `source_is_warp_stone`, `source_is_dimension(id)`.
- Target: `target_name_equals`, `target_name_contains`, `target_is_warp_plate`, `target_is_warp_portal`, `target_is_fleeting_memorial`, `target_is_player`, `target_is_global`, `target_is_sharestone`, `target_is_<color>_sharestone`, `target_is_waystone`, `target_is_dimension(id)`.
- Trip: `is_interdimensional`, `involves_dimension(id)`, `is_within_distance(f)`.
- Riding and company: `is_on_any_vehicle`, `is_on_vehicle(id or #tag)`, `is_with_passengers`, `is_with_pets`, `is_with_leashed`.
- Player: `has_cooldown(id)`, `has_cooldown_above(id, s)`, `has_item(item, n)`, `has_empty_inventory`, `is_wearing_any_armor`, `has_tag(s)`.

**Variables:** `distance`, `leashed`, `pets`, `passengers`.

**Cooldowns:**
- Kept per player as real time in milliseconds and sent to the client.
- In creative, using the inventory button clears them.
- The default cooldown id is `bedstones:inventory_button`.

**Travel between dimensions** is always allowed. A server that wants to forbid it adds a rule that refuses interdimensional trips.

### 3.7 What each menu lists
| Source | Destinations listed |
|---|---|
| Waystone | Activated + global/team waystones + bound scrolls/shards in the source's modifier slots + own memorials + Twinbound Feather partners + return portal |
| Sharestone | All sharestones of that colour (+ slot items, memorials, return portal) |
| Portstone (white) | Activated + global/team |
| Portstone (colour) | That colour's sharestones |
| Warp Scroll / Warp Stone | Activated + global/team + memorials + feather partners + return portal |
| Portal Scroll | Activated + global/team + memorials |
| Inventory button (ANY) | Activated + global/team + memorials + feather partners |

The source waystone is listed but cannot be picked.

### 3.8 Inventory button
- **Config `inventoryButton`:**
  - `""` or `NONE`: no button. **Default.**
  - `NEAREST`: nearest activated waystone in the same dimension (a return portal first).
  - `ANY`: opens the selection menu.
  - A waystone name: that activated waystone.
- **Position:**
  - Survival inventory: `inventoryButtonX = 58`, `inventoryButtonY = 60` from the inventory's top-left.
  - Creative: `creativeInventoryButtonX = 88`, `creativeInventoryButtonY = 33`, only on the survival-inventory tab.
- **Look:** 16×16. Shows the bound-scroll icon, and the warp-scroll icon on hover. When the trip cannot be paid (for example on cooldown), it is drawn faded and clicking only plays a click sound.
- **Tooltip:** line 1 names the action (return to a waystone, or to the nearest one, yellow); line 2 shows the bound name (gray, name dark aqua), or a red note that nothing is activated; when it cannot be paid, the remaining cooldown (gold).
- **Click:**
  - NEAREST or a name: a Yes/No confirmation asking whether to warp back, with the bound name below. Yes sends the request.
  - ANY: opens the selection screen.

### 3.9 Sharestones
Every sharestone reaches every other sharestone of its colour, server-wide, without activation. Only creative players can delete them from the list.

### 3.10 Modifiers (5 slots on waystones and warp plates)
- Slot 0 takes a dormant shard (one). Slots 1–4 take anything.
- Effects apply to entities **arriving** at that block.
- Potency starts at 1; each diamond adds 1, up to 4. Potency is the effect level.

| Modifier | Effect on arrivals |
|---|---|
| Blaze powder | Set on fire, 1 s per item |
| Poisonous potato | Poison, 1 s per item |
| Ink sac | Blindness, 1 s per item |
| Feather | Slow Falling, 1 s per item, max 8 s |
| Magma cream | Fire Resistance, 1 s per item, max 8 s |
| Wither rose | Wither, 1 s per item |
| Milk bucket / honey block | Clear effects |
| Quartz | Plates pick targets in turn |
| Spider eye | Plates prefer single-use shards |
| Redstone torch | Plate is off while powered |
| Amethyst shard / slime ball | Faster / slower plate |

- **Switch:** `enableModifiers`.
- **"N modifiers active"** counts every item above **except amethyst shards and slime balls**.
- **Access:** from the Edit screen. The server only checks distance (8 blocks), not permission; the client greys the button out when editing is not allowed.

### 3.11 World generation
- **Wild waystone generation:**
  - One per square of `chunksBetweenWildWaystones` (default 25) chunks, moved by a seeded random offset of up to half that.
  - On the ocean-floor surface. In dimensions with a ceiling, the first free spot above solid ground going down.
  - Default dimensions: overworld, the_nether, the_end.
  - Random facing. Origin wilderness.
- **Style by biome** (`wildWaystoneStyle = BIOME`):
  - desert → sandy
  - swamp, mangrove swamp, mushroom fields, jungles → mossy
  - Nether biomes → blackstone
  - End biomes → end stone
  - deep dark → deepslate
  - most other overworld biomes and oceans → default
  - Purpur, prismarine, nether-brick and mud-brick styles are never chosen.
- **Villages:**
  - A waystone can appear among the houses of plains, snowy, savanna and taiga villages (weight 1); desert villages have their own piece. A waterlogged variant exists.
  - `FREQUENT` puts one in every village.
  - Origin village.
- **Naming modes:** `PRESET_FIRST`, `RANDOM_ONLY`, `PRESET_ONLY`, `MIXED`. Template placeholders: `{Syllables}` (random syllables) and `{Biome}`.

### 3.12 Twinbound Feather
- **Linking:**
  - Two players hold use with feathers while looking at each other within 2 blocks.
  - After 20 ticks, hearts appear between them and the enchanting sound plays.
  - Each feather records the other.
- **Use:** while both carry their linked feathers and are online, each one is a destination for the other (listed by player name, in the Players group) in waystone, scroll, warp stone and inventory-button menus. The trip goes to the other player's current position. The cost tooltip names the feather in light purple.

### 3.13 Misc
- **On login** the client receives: sort mode, sort order, groups, activated waystones, all warp plates, all sharestones, cooldowns.
- **Taking damage** while charging a scroll or warp stone cancels it.
- **Map integrations** (JourneyMap, Dynmap, BlueMap, Jade/WTHIT) are Java Edition mods and out of scope; silk-touched waystones would be hidden from maps.

---

## 4. Screens

All screens draw widgets over the dimmed world, without a background panel, except the Warp Modifiers screen. Sizes below are in GUI pixels and are layout facts, not artwork.

### 4.1 Selection screen
- **Used for:** waystones, warp scrolls, warp stones, portal scrolls, portstones and the inventory button. Sharestones and the admin menu use small variations.
- **Size:** width 270; height at least 200, at most the screen height − 40.
- **Header (64 px):**
  - Title centred: at the top if there is a source waystone, otherwise 20 px lower.
  - Current location at y=20: a yellow prefix and the white name. Hovering underlines the name and shows a small edit icon; clicking opens the source's Edit screen.
  - Search box at y=40, 220 wide (198 with the sort button), 20 high. Case-insensitive match on the shown name.
  - Sort button 20×20 next to it, cycling **Manual → By Name → By Distance**, with a tooltip naming the mode. Saved on the server.
    - Name: case-insensitive.
    - Distance: same dimension first, then nearest.
    - Manual: the player's own order.
    - Return portals and memorials always come first.
- **Left column, 8 px left of the panel:**
  - Manage Waystones button 20×20 (not in the admin menu).
  - 5 px below, **group filter buttons** 20×20 with 2 px gaps, one per visible group that has at least one listed waystone. Up to 5, plus one per extra 22 px of height.
  - Each shows the group icon with its name as tooltip. Clicking toggles the filter; the active one is outlined in white.
- **List:** scrolling, rows 22 px, row width 220, buttons 216×20.
  - **Label:** shown name in the colour of its first group; "<unnamed>" if empty; warp portals in light purple.
  - **Left side, the cost:**
    - XP: the enchanting-table level icon (1, 2 or 3) for the cost in levels, greyed when unaffordable, with "+" above 3 levels.
    - Cooldown: "mm:ss" in light red.
    - Item cost: the item icon with its count, faded if missing.
    - Refused: a cancel icon.
    - Hovering shows the cost lines (XP green, cooldown gold, refusal red).
  - **Right side:** distance "123m", or "1.2km" from 10,000 m or for long names. Same dimension only. Nether and End entries get a dimension marker.
  - **Disabled rows:** the source waystone, and anything the player cannot pay for (unless in creative).
  - **Empty list:** a red note that nothing is activated, or that no sharestone of this colour exists.
- **Click:** the server checks the choice is in the list, closes the menu and starts the trip.

### 4.2 Manage Waystones screen
- Same frame, titled "Manage Waystones", no sort button.
- **Left column:** Back button at y=40, Manage Groups at y=64.
- **Rows:**
  - A 16 px drag handle; its tooltip explains drag, double-click (to top) and Shift + double-click (to bottom).
  - The name, not clickable.
  - A 13×13 edit icon for Personal Settings.
  - Then either a delete X (13×13) or an eye toggle (Hide / Show Waystone). The eye appears for global, team or sharestone entries this player cannot delete.
- **Delete X:** only works while **Shift** is held; the tooltip says so. For global waystones and sharestones a red line warns that it affects everyone.
- **Who can delete:**
  - Waystone entries.
  - Global ones only in creative or when global management is allowed.
  - Team ones only if activated.
  - Sharestones only in creative.
- **What delete does:**
  - Survival: forgets it for this player.
  - Creative, sharestone: removes it for good.
  - Creative, global: makes it activation-only again, and removes it if the block is gone.
- **Dragging:** scrolls when within 12 px of the list edge; each step swaps two entries on the server.
- Return portals and memorials are not in this list.

### 4.3 Personal Settings screen
- Width 176. Title = waystone name at y=44.
- **Widgets:**
  - Alias field 150×19 (176 if editing is unavailable), up to 128 characters, placeholder "No alias". Right-click clears it.
  - "Edit Waystone" button 21×21 at x+155, only within 4 blocks and not for feather partners.
  - **Group button** 150 wide: left-click next group, right-click previous. Shows icon and coloured name, or "No Group" / "No groups defined". Only the player's own (not inbuilt) groups.
  - Manage Groups button 20×20.
  - Favorite checkbox.
  - Save button 100×20.
- **Enter or Esc** saves and goes back.

### 4.4 Manage Groups and Edit Group
- **Manage Groups:** search box, a "+" button to create a group, and Back.
  - Rows: drag handle | icon and coloured name | edit icon | delete X (own groups, Shift needed) or hide/unhide toggle (inbuilt groups).
  - Empty list: a note that there are no groups.
- **Inbuilt groups:**
  - Favorites: gold, sort index −30.
  - Players: green, −20.
  - Global Waystones (yellow, −10).
  - Teams: aqua, −5.
  - One group per dimension (Overworld, The Nether, The End), white.
- **Edit Group** (176×120, titled "Edit Group"):
  - Name field in the group colour, placeholder "Unnamed Group".
  - Icon button 21×21 cycling the icons: community_hubs, dimension, dungeons, favorites, global, player_homes, players, resource_sites, teams, villages and the dimension icons. Right-click goes back. Tooltip shows the icon name.
  - Colour button 21×21 cycling 15 colours (white, gold, light_purple, aqua, yellow, green, red, gray, dark_gray, dark_aqua, dark_purple, blue, dark_blue, dark_green, dark_red). Tooltip shows the colour.
  - Save.
  - New group ids are `bedstones:user/<uuid>`.

### 4.5 Edit screen
- **Opened by:** shift + right-click, placing a waystone, or clicking the location in the selection header.
- **Title:** "Edit" and the block kind (Waystone, Sharestone or Warp Plate), centred at y=44.
- **Top to bottom:**
  1. An optional red notice at y=56.
  2. Name field 150×19, up to 128 characters, placeholder "Untitled Waystone". Read-only when editing is refused; right-click clears it.
  3. Personal Settings button 21×21 at x+155.
  4. Visibility button 20×20 with an icon per visibility and a label to its right; clicking cycles the allowed choices. When editing is refused, its tooltip adds a red notice.
  5. Manage Modifiers button 20×20, with the number of active modifiers (green) or a note that there are none (gray).
  6. Save (or Close when editing is refused), 100×20, centred.
- **Esc, Enter or the button** send the name and visibility to the server.

### 4.6 Warp Modifiers screen
- A 176×196 panel titled "Warp Modifiers", with the plate's galactic name at the top right.
- **Slots in a cross:** shard slot in the centre (80,45); modifiers above (80,17), right (108,45), below (80,73) and left (52,45).
- Player inventory at y=104, hotbar at y=162, inventory label at y=93.
- **Shift-click** moves one item into the shard slot if empty, otherwise into the modifier slots.

---

## 5. Configuration

Option names are stable; the table lists every section, key and default.

| Section | Key | Default |
|---|---|---|
| general | restrictedWaystones (set of origins) | [PLAYER] |
| general | defaultVisibility (ACTIVATION/GLOBAL/TEAM) | ACTIVATION |
| general | allowEveryoneToManageGlobalWaystones | false |
| general | allowedVisibilities (deprecated) | [] |
| general | warpStoneUseTime (ticks) | 32 |
| general | warpPlateUseTime | 15 |
| general | warpPlateCooldownTime | 20 |
| general | scrollUseTime | 32 |
| teleports | enableCosts | true |
| teleports | enableCooldowns | true |
| teleports | enableDurability | true |
| teleports | warpRequirements | rules in §3.6 |
| teleports | transportPets (ENABLED/SAME_DIMENSION/DISABLED) | DISABLED |
| teleports | transportLeashed | ENABLED |
| teleports | entityDenyList | [minecraft:wither] |
| teleports | enableModifiers | true |
| inventoryButton | inventoryButton | "" (off) |
| inventoryButton | inventoryButtonX / Y | 58 / 60 |
| inventoryButton | creativeInventoryButtonX / Y | 88 / 33 |
| worldGen | wildWaystoneStyle (DEFAULT, MOSSY, SANDY, BLACKSTONE, RED_NETHER_BRICKS, DEEPSLATE, END_STONE, PURPUR, PRISMARINE, MUD_BRICKS, BIOME) | BIOME |
| worldGen | chunksBetweenWildWaystones (0 = off) | 25 |
| worldGen | wildWaystonesDimensionAllowList | overworld, the_nether, the_end |
| worldGen | wildWaystonesDimensionDenyList | [] |
| worldGen | nameGenerationMode | PRESET_FIRST |
| worldGen | nameGenerationTemplate | `{Syllables}` |
| worldGen | nameGenerationPresets | [] |
| worldGen | spawnInVillages (DISABLED/REGULAR/FREQUENT) | REGULAR |
| client | disableTextGlow | false |
| compatibility | journeyMap / preferJourneyMapIntegrationMod / dynmap / fixVanillaTeleportBug | true / true / true / false |
| blueMap | enabled / includeWaystones / includeSharestones / includeUndiscoveredWaystones | true / true / true / false |


---

## 6. Commands
- **Root:** `/bedstones`. Every subcommand needs permission level 2.
- **Subcommands:**
  - `activate <targets> <pos>`: activates the waystone at `pos` for the targets and reports it (one player or a count).
  - `forget <targets> <pos>` and `forget <targets> all`: the reverse, with the same kind of report.
  - `place <pos> <style> <name…>`: places a named waystone; styles are suggested; an unknown style is an error.
  - `count <player>`: how many waystones the player has activated and how many of those they own.
  - `list <player> [all]`: a header, one line per waystone (owned or only activated) with its name in green and clickable yellow coordinates that suggest a teleport command, the dimension or the distance, and a footer with totals. Without `all`, only owned waystones.
  - `gui <player>`: opens that player's selection menu for the admin, without manage, sort or delete controls.
  - `twinbound <first> <second>`: gives or links a pair of feathers; errors when they are already linked, the players are the same, or a player has no free slot.
  - `cooldown <targets> reset all | reset <id> | add <id> <seconds> | set <id> <seconds≥0>`.
- An error is reported when no waystone is at the given position.

## 7. Text
Translation keys are `gui.bedstones.*`, `chat.bedstones.*`, `tooltip.bedstones.*`, `commands.bedstones.*`, ... for the core strings and `bs.<unit>.*` for unit-specific ones. The text lives in `assets/lang/<locale>/*.json` and the resource pack's `texts.d`.

## 8. Implementation

**Implementation units:**
1. Data store and sync
2. Blocks (waystone, sharestone, portstone, plate, portal, memorial)
3. Items
4. Teleporting
5. Costs, rules and cooldowns
6. Permissions and visibility
7. Selection UI
8. Manage, personal and group UIs
9. Edit and modifier UIs
10. Inventory button
11. World generation and naming
12. Commands
13. Feather and epitaph
14. Configuration
15. Localisation
