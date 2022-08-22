#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <format>
#include <fstream>
#include <map>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <vector>


template <typename... Args>
void print(std::string_view format, Args&&... args)
{
    std::cout << std::vformat(format, std::make_format_args(args...));
}

using StringList = std::vector<std::string>;
