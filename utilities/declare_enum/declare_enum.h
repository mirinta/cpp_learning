#pragma once

#include <array>
#include <string_view>
#include <type_traits>

namespace details {
template <typename Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum e)
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}

constexpr size_t count_commas(std::string_view s)
{
    size_t count = 0;
    for (const auto& c : s) {
        count += c == ',';
    }
    return count;
}

template <size_t N>
constexpr std::array<std::string_view, N> split_enum_strings(std::string_view s)
{
    std::array<std::string_view, N> strings{};
    for (size_t i = 0, j = 0, k = 0; i < s.size(); ++i) {
        if (s[i] == ' ') {
            j = i + 1;
            continue;
        }
        if (s[i] == ',' || i == s.size() - 1) {
            strings[k++] = s.substr(j, i - j + (i == s.size() - 1));
            j = i + 1;
        }
    }
    return strings;
}
} // namespace details

#define DECLARE_ENUM(classname, ...)                                                     \
    enum class classname { __VA_ARGS__ };                                                \
                                                                                         \
    template <typename Enum, std::enable_if_t<std::is_same_v<Enum, classname>, int> = 0> \
    static constexpr size_t EnumCount([[maybe_unused]] classname e = {})                 \
    {                                                                                    \
        return details::count_commas(#__VA_ARGS__) + 1;                                  \
    }                                                                                    \
                                                                                         \
    template <typename Enum, std::enable_if_t<std::is_same_v<Enum, classname>, int> = 0> \
    static constexpr std::string_view EnumName([[maybe_unused]] classname e = {})        \
    {                                                                                    \
        return #classname;                                                               \
    };                                                                                   \
                                                                                         \
    static constexpr std::string_view EnumKeyName(classname e)                           \
    {                                                                                    \
        constexpr auto count = details::count_commas(#__VA_ARGS__) + 1;                  \
        constexpr auto strings = details::split_enum_strings<count>(#__VA_ARGS__);       \
        return strings[details::to_underlying(e)];                                       \
    }
