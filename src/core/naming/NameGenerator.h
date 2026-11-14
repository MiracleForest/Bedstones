#pragma once

// Name generator (pure C++, docs/SPEC.md §3.1 / §3.11): random syllable names,
// templates with `{Syllables}` / `{Biome}`, preset modes and the Roman-numeral suffix that keeps
// every generated name unique against the world's used-names set.

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "nlohmann/json.hpp"

namespace bs::core::naming {

/// `nameGenerationMode`.
enum class Mode {
    PresetFirst, ///< an unused preset if any is left, otherwise a template name
    RandomOnly,  ///< always a template name (warp plates)
    PresetOnly,  ///< any preset (duplicates get a numeral); template name only without presets
    Mixed,       ///< 50/50 between an unused preset and a template name
};

/// "PRESET_FIRST" / "preset_first" -> Mode; nullopt for unknown values.
[[nodiscard]] std::optional<Mode> modeFromString(std::string_view s);
[[nodiscard]] std::string_view    toString(Mode m); ///< "PRESET_FIRST", ...

/// The three syllable tables of the syllable name generator: one entry of each, concatenated.
extern std::array<std::string_view, 25> const kSyllables1;
extern std::array<std::string_view, 20> const kSyllables2;
extern std::array<std::string_view, 13> const kSyllables3;

/// Uniform random integer in [0, bound) (bound > 0).
using Random = std::function<std::uint32_t(std::uint32_t bound)>;

/// A random syllable name, e.g. "Corenvale".
[[nodiscard]] std::string randomSyllableName(Random const& rng);
/// "minecraft:cherry_grove" -> "Cherry Grove" (text without an id shape is returned as is).
[[nodiscard]] std::string biomeDisplayName(std::string_view biome);
/// Replaces every `{Syllables}` with `syllableName` and every `{Biome}` with `biome`; trims spaces.
[[nodiscard]] std::string expandTemplate(std::string_view tpl, std::string_view syllableName, std::string_view biome);
/// 2 -> "II", 14 -> "XIV" (n >= 1; 0 and below give "").
[[nodiscard]] std::string toRoman(int n);

struct Settings {
    Mode                     mode = Mode::PresetFirst;
    std::string              nameTemplate = "{Syllables}";
    std::vector<std::string> presets;
};

/// The world's generator state: the set of names handed out so far (`UsedNames`).
class NameGenerator {
public:
    /// A new unique name; it is added to `usedNames()`. `biome` fills `{Biome}` (id or display name).
    std::string generate(Settings const& settings, std::string_view biome, Random const& rng);
    /// `name` if unused, otherwise "name II", "name III", ... (the first unused one).
    [[nodiscard]] std::string resolveDuplicate(std::string const& name) const;

    [[nodiscard]] std::set<std::string> const& usedNames() const { return mUsed; }
    void                                       markUsed(std::string const& name) { mUsed.insert(name); }
    bool                                       forget(std::string const& name) { return mUsed.erase(name) > 0; }
    void                                       clear() { mUsed.clear(); }

    [[nodiscard]] nlohmann::json toJson() const; ///< {"usedNames": [...]}
    void                         loadJson(nlohmann::json const& j);

private:
    [[nodiscard]] std::optional<std::string> presetName(Settings const& s, bool onlyUnused, Random const& rng) const;
    std::set<std::string>                    mUsed;
};

} // namespace bs::core::naming
