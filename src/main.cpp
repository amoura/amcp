#include <cassert>
#include <cctype>
#include <cstdint>

#include <iterator>
#include <optional>
#include <variant>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <format>
#include <print>

#define TESTING 1

#include "base.hpp"
#include "str.hpp"
#include "lex.hpp"

#include "str.cpp"
#include "lex.cpp"

int main()
{
    test_str();
    test_str_store();
    test_result_type();
    test_lexer_1_basic();

    return 0;
}
