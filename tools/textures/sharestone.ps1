# Unit U2: the 15 dyed sharestones (docs/SPEC.md §1.3). Original pixel art + generated pack files:
#   assets/pack/rp/textures/blocks/sharestone/<color>_sharestone.png   48x32 UV sheet (stones_common.ps1)
#   assets/pack/rp/models/blocks/sharestone.geo.json                   lower / upper / item geometry
#   assets/pack/rp/{terrain_texture.d,blocks.d}/sharestone.json, texts.d/<locale>/sharestone.lang
#   assets/pack/bp/blocks/<color>_sharestone.json, recipes/<color>_sharestone.json
#
#   pwsh tools/textures/sharestone.ps1 [-Preview]   (-Preview also writes 8x previews to tools/textures/preview/)
param([switch]$Preview)

. "$PSScriptRoot/lib.ps1"
. "$PSScriptRoot/stones_common.ps1"

$root = (Resolve-Path "$PSScriptRoot/../..").Path
$rp = Join-Path $root 'assets/pack/rp'
$bp = Join-Path $root 'assets/pack/bp'

# ---- shape (Bedstones' own design) ---------------------------------------------------------------------
# A low full-width slab and a 12-wide plinth carry an 8-wide core (lower y 5 .. upper y 9); above it
# the stone flares out in two steps (12, then 14 wide) and ends in a raised 10-wide dais.
$lower = @(
    (New-Box 0 0 0 16 2 16),
    (New-Box 2 2 2 12 3 12),
    (New-Box 4 5 4 8 11 8)
)
$upper = @(
    (New-Box 4 0 4 8 9 8),
    (New-Box 2 9 2 12 3 12),
    (New-Box 1 12 1 14 2 14),
    (New-Box 3 14 3 10 2 10),
    # The floating warp stone (Java renders a spinning item; here a static gem above the top).
    (New-Box 6 19 6 4 4 4 @{ rot = @(0, 45, 0); pivot = @(8, 21, 8); allUv = @(32, 0, 4, 4) })
)
Save-StoneGeometry (Join-Path $rp 'models/blocks/sharestone.geo.json') 'sharestone' $lower $upper

# ---- texture ---------------------------------------------------------------------------------------------
# Rune column on the pillar (rows 9..22 span the joint of both halves).
$runes = @(
    '.rr.r.',
    '.r..r.',
    '.rurr.',
    '....r.',
    'r.....',
    'rrr.rr',
    '..r..r',
    '.rur.r',
    '......',
    '.r..r.',
    'rrrru.',
    '.r..r.',
    '......',
    'r.rr.r'
)
function New-SharestoneSheet {
    $g = New-Sheet
    # Side picture, upper half (rows 0..15) then lower half (rows 16..31).
    # rows 0..1 dais, 2..3 wide rim, 4..6 flare, 7..26 core, 27..29 plinth, 30..31 slab
    Fill-Band $g 0 15 0 1 'trim'
    Fill-Band $g 0 15 2 3 'polished'
    Fill-Band $g 0 15 4 6 'dark'
    Fill-Band $g 0 15 7 26 'brick'
    Fill-Band $g 0 15 27 29 'polished' 3
    Fill-Band $g 0 15 30 31 'dark' 5
    Draw-Runes $g 5 9 $runes
    # Top faces: the cap with a rune ring in the middle.
    Fill-Band $g 16 31 0 15 'brick' 5
    for ($i = 16; $i -le 31; $i++) { $g[0][$i] = 'a'; $g[15][$i] = 'a' }
    for ($j = 0; $j -le 15; $j++) { $g[$j][16] = 'a'; $g[$j][31] = 'a' }
    for ($j = 0; $j -le 15; $j++) {
        for ($i = 0; $i -le 15; $i++) {
            $dx = $i - 7.5; $dy = $j - 7.5; $r = [Math]::Sqrt($dx * $dx + $dy * $dy)
            if ($r -ge 3.6 -and $r -lt 4.6) { $g[$j][16 + $i] = 'r' }
            elseif ($r -lt 1.6) { $g[$j][16 + $i] = 'u' }
        }
    }
    # Bottom faces and extras.
    Fill-Band $g 16 31 16 31 'dark' 7
    Fill-Band $g 32 47 0 31 'polished' 9
    Draw-Gem $g 32 0
    return , $g
}

$terrain = [ordered]@{}
$blocks = [ordered]@{}
$en = @(); $zh = @()
foreach ($color in $StoneColors) {
    if ($color -eq 'white') { continue } # there is no white sharestone
    $path = "$($color)_sharestone"
    $png = Join-Path $rp "textures/blocks/sharestone/$path.png"
    $sheet = New-SharestoneSheet
    Save-Sheet $sheet $color $png
    if ($Preview) {
        $rows = foreach ($line in $sheet) { -join $line }
        Save-PixelArt -Rows $rows -Palette (Get-StonePalette $color) -Path (Join-Path $PSScriptRoot "preview/$path.png") -Scale 8
    }
    $terrain["bs_$path"] = [ordered]@{ textures = "textures/blocks/sharestone/$path" }
    $blocks["bedstones:$path"] = [ordered]@{ sound = 'stone' }
    $en += "tile.bedstones:$path.name=$($ColorNames[$color]) Sharestone"
    $zh += "tile.bedstones:$path.name=$($ColorNamesZh[$color])共享石碑"

    $lowerBox = Get-Box @(-8, 0, -8) @(16, 16, 16)
    $upperBox = Get-Box @(-7, 0, -7) @(14, 16, 14)
    Save-Json (New-StoneBlock $path 'sharestone' "bs_$path" $lowerBox $upperBox) (Join-Path $bp "blocks/$path.json")
    $key = [ordered]@{
        S = [ordered]@{ item = 'minecraft:stone_bricks' }
        D = [ordered]@{ item = "minecraft:$($color)_dye" }
        W = [ordered]@{ item = 'bedstones:warp_stone' }
        O = [ordered]@{ item = 'minecraft:obsidian' }
    }
    Save-Json (New-ShapedRecipe $path @('SSS', 'DWD', 'OOO') $key) (Join-Path $bp "recipes/$path.json")
}
Save-Json $terrain (Join-Path $rp 'terrain_texture.d/sharestone.json')
Save-Json $blocks (Join-Path $rp 'blocks.d/sharestone.json')
Save-Text (($en -join "`n") + "`n") (Join-Path $rp 'texts.d/en_US/sharestone.lang')
Save-Text (($zh -join "`n") + "`n") (Join-Path $rp 'texts.d/zh_CN/sharestone.lang')
Save-Json ([ordered]@{ pools = @() }) (Join-Path $bp 'loot_tables/blocks/bedstones_stones_none.json')
Write-Host "sharestone assets written"
