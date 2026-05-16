#include "spdlog/spdlog.h"

auto __init = []() {
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][process %P][thread %t][%l] [%v]");
    return true;
}();

int main()
{
    spdlog::info("=====BEGIN=====");
    spdlog::info("=====END=====");
}