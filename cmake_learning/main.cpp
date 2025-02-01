#include <string>

#include "static_lib/add.h"
#include "dynamic_lib/print.h"

int main()
{
    print(std::to_string(add(11, 22)));
    return 0;
}
