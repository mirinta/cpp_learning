#pragma once
#include <functional>
#include <concepts>

struct Config
{
};

namespace runtime_polymorphism {
using ConfigLoader = std::function<Config(const std::string&)>;

void interface(ConfigLoader&& loader);

inline void usage()
{
    interface([](const std::string&) -> Config { return {}; });
}
} // namespace runtime_polymorphism

namespace compiletime_polymorphism {
template <typename T>
concept ConfigLoader = std::invocable<T, const std::string&>&&
    std::same_as<Config, std::invoke_result_t<T, const std::string&>>;

void interface(ConfigLoader auto&& loader);

inline void usage()
{
    interface([](const std::string&) -> Config { return {}; });
}
} // namespace compiletime_polymorphism
