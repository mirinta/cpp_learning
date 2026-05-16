#include <array>
#include <optional>
#include <string_view>

namespace detail {
constexpr bool is_space(char c) noexcept
{
    return c == ' ';
}

constexpr bool is_digit(char c) noexcept
{
    return c >= '0' && c <= '9';
}

constexpr std::size_t count_top_level(std::string_view s) noexcept
{
    std::size_t count = 0;
    std::size_t start = 0;
    for (std::size_t end = 0; end < s.size(); ++end) {
        if (s[end] == ',') {
            count++;
            start = end + 1;
        }
    }
    if (start < s.size()) {
        count++;
    }
    return count;
}

template <std::size_t N>
constexpr auto split_top_level(std::string_view s) noexcept
{
    std::array<std::string_view, N> result{};
    std::size_t start = 0;
    std::size_t index = 0;
    for (std::size_t end = 0; end < s.size(); ++end) {
        if (s[end] == ',') {
            result[index++] = s.substr(start, end - start);
            start = end + 1;
        }
    }
    if (start < s.size()) {
        result[index] = s.substr(start);
    }
    return result;
}

constexpr std::string_view trim(std::string_view s) noexcept
{
    std::size_t start = 0;
    while (start < s.size() && is_space(s[start])) {
        start++;
    }
    std::size_t end = s.size();
    while (end > start && is_space(s[end - 1])) {
        end--;
    }
    return s.substr(start, end - start);
}

struct name_expr_t {
    std::string_view name;
    std::string_view expr;
};

constexpr name_expr_t parse_name_expr(std::string_view s) noexcept
{
    std::size_t eq = s.find('=');
    if (eq == std::string_view::npos)
        return {trim(s), {}};

    return {trim(s.substr(0, eq)), trim(s.substr(eq + 1))};
}

constexpr int eval_expr(std::string_view s) noexcept
{
    int result{0};
    for (const auto& c : s) {
        if (is_digit(c)) {
            result = result * 10 + (c - '0');
        }
    }
    return result;
}
} // namespace detail

template <typename E>
class EnumHelper;

#define DECLARE_ENUM(TYPE_NAME, ...)                                               \
    enum class TYPE_NAME : int { __VA_ARGS__ };                                    \
                                                                                   \
    template <>                                                                    \
    class EnumHelper<TYPE_NAME> {                                                  \
        using E = TYPE_NAME;                                                       \
                                                                                   \
    public:                                                                        \
        static constexpr std::string_view name() noexcept                          \
        {                                                                          \
            return #TYPE_NAME;                                                     \
        }                                                                          \
                                                                                   \
        static constexpr std::size_t count() noexcept                              \
        {                                                                          \
            return entries.size();                                                 \
        }                                                                          \
                                                                                   \
        static constexpr std::string_view name(E e) noexcept                       \
        {                                                                          \
            if (const auto i = index(e); i != -1)                                  \
                return entries[i].name;                                            \
            return {};                                                             \
        }                                                                          \
                                                                                   \
        static constexpr std::optional<int> value(E e) noexcept                    \
        {                                                                          \
            if (const auto i = index(e); i != -1)                                  \
                return entries[i].value;                                           \
            return {};                                                             \
        }                                                                          \
                                                                                   \
        static constexpr std::size_t index(E e) noexcept                           \
        {                                                                          \
            for (std::size_t i = 0; i < entries.size(); ++i) {                     \
                if (entries[i].value == static_cast<int>(e))                       \
                    return i;                                                      \
            }                                                                      \
            return -1;                                                             \
        }                                                                          \
                                                                                   \
        static constexpr bool contains(E e) noexcept                               \
        {                                                                          \
            return index(e) != -1;                                                 \
        }                                                                          \
                                                                                   \
        static constexpr std::optional<E> from_string(std::string_view s) noexcept \
        {                                                                          \
            for (std::size_t i = 0; i < entries.size(); ++i) {                     \
                if (entries[i].name == s)                                          \
                    return static_cast<E>(entries[i].value);                       \
            }                                                                      \
            return {};                                                             \
        }                                                                          \
                                                                                   \
        static constexpr auto names() noexcept                                     \
        {                                                                          \
            constexpr auto N = count();                                            \
            std::array<std::string_view, N> names{};                               \
            for (std::size_t i = 0; i < N; ++i) {                                  \
                names[i] = entries[i].name;                                        \
            }                                                                      \
            return names;                                                          \
        }                                                                          \
                                                                                   \
        static constexpr auto values() noexcept                                    \
        {                                                                          \
            constexpr auto N = count();                                            \
            std::array<int, N> values{};                                           \
            for (std::size_t i = 0; i < N; ++i) {                                  \
                values[i] = entries[i].value;                                      \
            }                                                                      \
            return values;                                                         \
        }                                                                          \
                                                                                   \
    private:                                                                       \
        struct Entry {                                                             \
            std::string_view name;                                                 \
            int value;                                                             \
        };                                                                         \
                                                                                   \
        static constexpr auto entries = [] {                                       \
            constexpr auto N = detail::count_top_level(#__VA_ARGS__);              \
            constexpr auto parts = detail::split_top_level<N>(#__VA_ARGS__);       \
            int next_value{0};                                                     \
            std::array<Entry, N> result{};                                         \
            for (std::size_t i = 0; i < N; ++i) {                                  \
                auto name_expr = detail::parse_name_expr(parts[i]);                \
                result[i].name = name_expr.name;                                   \
                int value = next_value;                                            \
                if (!name_expr.expr.empty()) {                                     \
                    value = detail::eval_expr(name_expr.expr);                     \
                }                                                                  \
                result[i].value = value;                                           \
                next_value = value + 1;                                            \
            }                                                                      \
            return result;                                                         \
        }();                                                                       \
    };

DECLARE_ENUM(Color, RED, YELLOW = 999, BLUE);

static_assert(EnumHelper<Color>::name() == "Color");
static_assert(EnumHelper<Color>::count() == 3);
static_assert(EnumHelper<Color>::name(Color::BLUE) == "BLUE");

static_assert(EnumHelper<Color>::contains(Color::BLUE));
static_assert(EnumHelper<Color>::index(Color::BLUE) == 2);
static_assert(EnumHelper<Color>::value(Color::BLUE) == 1000);

static_assert(!EnumHelper<Color>::contains(static_cast<Color>(888)));
static_assert(EnumHelper<Color>::index(static_cast<Color>(888)) == -1);

// std::array::operator==(...) is constexpr since c++20
static_assert(EnumHelper<Color>::names() == std::array<std::string_view, 3>{"RED", "YELLOW", "BLUE"});
static_assert(EnumHelper<Color>::values() == std::array<int, 3>{0, 999, 1000});