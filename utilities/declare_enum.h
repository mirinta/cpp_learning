#pragma once

#include <array>
#include <string_view>
#include <type_traits>

namespace detail {
template <typename Enum>
inline constexpr auto to_underlying(Enum e) {
  return static_cast<std::underlying_type_t<Enum>>(e);
}
}  // namespace detail

#define DECLARE_ENUM(classname, ...)                                                       \
  enum class classname { __VA_ARGS__ };                                                    \
  inline static constexpr auto EnumValueToString(classname e) {                            \
    constexpr auto kStrings = [](std::string_view s) {                                     \
      enum class __##classname{__VA_ARGS__, ItemCount};                                    \
      std::array<std::string_view, detail::to_underlying(__##classname ::ItemCount)> strs; \
      for (size_t i = 0, j = 0, k = 0; i < s.size(); ++i) {                                \
        if (s[i] == ' ') {                                                                 \
          j = i + 1;                                                                       \
          continue;                                                                        \
        }                                                                                  \
        if (s[i] == ',' || i == s.size() - 1) {                                            \
          strs[k++] = s.substr(j, i - j + (i == s.size() - 1));                            \
          j = i + 1;                                                                       \
        }                                                                                  \
      }                                                                                    \
      return strs;                                                                         \
    }(#__VA_ARGS__);                                                                       \
    return kStrings[detail::to_underlying(e)];                                             \
  }

DECLARE_ENUM(COLOR, RED, YELLOW)
static_assert(EnumValueToString(COLOR::YELLOW) == "YELLOW", "");

struct Test
{
    DECLARE_ENUM(ERROR, NONE, ERROR1, ERROR2)
};
static_assert(Test::EnumValueToString(Test::ERROR::NONE) == "NONE", "");
