#include "spdlog/spdlog.h"

auto _ = []() {
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][process %P][thread %t][%l] %v");
    return true;
}();

int main() { return 0; }