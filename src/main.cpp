#include <cassert>
#include <cctype>

#include <optional>
#include <variant>
#include <string_view>
#include <unordered_set>
#include <algorithm>
#include <print>

#define TESTING 1

#include "base.hpp"
#include "str.hpp"

#include "str.cpp"

int main()
{
    test_str();
    test_str_store();
    test_result_type();

    return 0;
}
