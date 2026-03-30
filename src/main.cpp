#include <cassert>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <algorithm>
#include <print>

#define TESTING 1

#include "str.hpp"
#include "str.cpp"

int main()
{
    test_str();
    test_str_store();
    return 0;
}
