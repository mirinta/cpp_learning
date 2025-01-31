#include "declare_enum.h"

DECLARE_ENUM(COLOR, RED, BLUE, YELLOW)
static_assert(EnumKeyName(COLOR::YELLOW) == "YELLOW", "");
static_assert(EnumCount<COLOR>() == 3, "");
static_assert(EnumName<COLOR>() == "COLOR", "");

DECLARE_ENUM(Type, A, B, C, D)
static_assert(EnumKeyName(Type::C) == "C", "");
static_assert(EnumCount<Type>() == 4, "");
static_assert(EnumName<Type>() == "Type", "");

struct Test
{
    DECLARE_ENUM(ERROR, NONE, ERROR1, ERROR2)
    DECLARE_ENUM(STATUS, READY, RUNNING)
};
static_assert(Test::EnumKeyName(Test::ERROR::NONE) == "NONE", "");
static_assert(Test::EnumCount<Test::ERROR>() == 3, "");
static_assert(Test::EnumName<Test::ERROR>() == "ERROR", "");

static_assert(Test::EnumKeyName(Test::STATUS::RUNNING) == "RUNNING", "");
static_assert(Test::EnumCount<Test::STATUS>() == 2, "");
static_assert(Test::EnumName<Test::STATUS>() == "STATUS", "");

int main() {}