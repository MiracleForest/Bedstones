#include "core/naming/NameGenerator.h"

#include <algorithm>
#include <cctype>

namespace bs::core::naming {

std::array<std::string_view, 25> const kSyllables1{
    "Ae",  "Bel", "Cor",  "Dun", "El",  "Fal", "Gor", "Hal", "Is", "Kel", "Lor", "Mal", "Nor",
    "Ost", "Pel", "Quen", "Ros", "Sel", "Tor", "Ul",  "Val", "Wen", "Yr", "Zan", "Thal",
};
std::array<std::string_view, 20> const kSyllables2{
    "a",   "e",   "i",   "o",   "an",  "en",  "ir",  "or",  "ul",  "ath",
    "ber", "dra", "gol", "lin", "mor", "nes", "rin", "sha", "tav", "wyn",
};
std::array<std::string_view, 13> const kSyllables3{
    "n", "s", "th", "dor", "fell", "gard", "holm", "mere", "more", "stead", "vale", "wick", "ford",
};

namespace {

constexpr std::array<std::string_view, 4> kModes{"PRESET_FIRST", "RANDOM_ONLY", "PRESET_ONLY", "MIXED"};

template <std::size_t N>
std::string_view pick(std::array<std::string_view, N> const& table, Random const& rng) {
    return table[rng(static_cast<std::uint32_t>(N)) % N];
}

void replaceAll(std::string& s, std::string_view from, std::string_view to) {
    for (std::size_t at = s.find(from); at != std::string::npos; at = s.find(from, at + to.size())) {
        s.replace(at, from.size(), to);
    }
}

std::string trim(std::string s) {
    auto notSpace = [](unsigned char c) { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
    s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
    return s;
}

} // namespace

std::optional<Mode> modeFromString(std::string_view s) {
    for (std::size_t i = 0; i < kModes.size(); ++i) {
        auto m = kModes[i];
        if (m.size() == s.size()
            && std::equal(m.begin(), m.end(), s.begin(), [](char a, char b) {
                   return a == std::toupper(static_cast<unsigned char>(b));
               })) {
            return static_cast<Mode>(i);
        }
    }
    return std::nullopt;
}

std::string_view toString(Mode m) { return kModes[static_cast<std::size_t>(m)]; }

std::string randomSyllableName(Random const& rng) {
    std::string out(pick(kSyllables1, rng));
    out += pick(kSyllables2, rng);
    out += pick(kSyllables3, rng);
    return out;
}

std::string biomeDisplayName(std::string_view biome) {
    if (auto colon = biome.find(':'); colon != std::string_view::npos) biome.remove_prefix(colon + 1);
    else if (biome.find(' ') != std::string_view::npos) return std::string(biome); // already a display name
    std::string out;
    bool        upper = true;
    for (char c : biome) {
        if (c == '_') {
            out += ' ';
            upper = true;
        } else {
            out += upper ? static_cast<char>(std::toupper(static_cast<unsigned char>(c))) : c;
            upper = false;
        }
    }
    return out;
}

std::string expandTemplate(std::string_view tpl, std::string_view syllableName, std::string_view biome) {
    std::string out(tpl);
    replaceAll(out, "{Syllables}", syllableName);
    replaceAll(out, "{Biome}", biome);
    return trim(std::move(out));
}

std::string toRoman(int n) {
    static constexpr std::array<std::pair<int, std::string_view>, 13> kTable{{
        {1000, "M"},
        {900, "CM"},
        {500, "D"},
        {400, "CD"},
        {100, "C"},
        {90, "XC"},
        {50, "L"},
        {40, "XL"},
        {10, "X"},
        {9, "IX"},
        {5, "V"},
        {4, "IV"},
        {1, "I"},
    }};
    std::string out;
    for (auto const& [value, glyph] : kTable) {
        while (n >= value) {
            out += glyph;
            n   -= value;
        }
    }
    return out;
}

std::optional<std::string> NameGenerator::presetName(Settings const& s, bool onlyUnused, Random const& rng) const {
    std::vector<std::string const*> candidates;
    for (auto const& p : s.presets) {
        if (p.empty()) continue;
        if (onlyUnused && mUsed.contains(p)) continue;
        candidates.push_back(&p);
    }
    if (candidates.empty()) return std::nullopt;
    return *candidates[rng(static_cast<std::uint32_t>(candidates.size())) % candidates.size()];
}

std::string NameGenerator::resolveDuplicate(std::string const& name) const {
    std::string attempt = name;
    for (int i = 2; mUsed.contains(attempt); ++i) attempt = name + " " + toRoman(i);
    return attempt;
}

std::string NameGenerator::generate(Settings const& settings, std::string_view biome, Random const& rng) {
    std::optional<std::string> name;
    switch (settings.mode) {
    case Mode::PresetFirst:
        name = presetName(settings, true, rng);
        break;
    case Mode::PresetOnly:
        name = presetName(settings, false, rng);
        break;
    case Mode::Mixed:
        if (rng(2) == 0) name = presetName(settings, true, rng);
        break;
    case Mode::RandomOnly:
        break;
    }
    if (!name) {
        name = expandTemplate(settings.nameTemplate, randomSyllableName(rng), biomeDisplayName(biome));
        if (name->empty()) name = randomSyllableName(rng); // a template without placeholders that expands to nothing
    }
    auto unique = resolveDuplicate(*name);
    mUsed.insert(unique);
    return unique;
}

nlohmann::json NameGenerator::toJson() const { return {{"usedNames", mUsed}}; }

void NameGenerator::loadJson(nlohmann::json const& j) {
    mUsed.clear();
    if (!j.is_object()) return;
    auto it = j.find("usedNames");
    if (it == j.end() || !it->is_array()) return;
    for (auto const& v : *it) {
        if (v.is_string()) mUsed.insert(v.get<std::string>());
    }
}

} // namespace bs::core::naming
