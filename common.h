#pragma once

#include <iostream>
#include <format>
#include <optional>
#include <ranges>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>
#include <vector>


template <typename... Args>
void print(std::string_view format, Args&&... args)
{
    std::cout << std::vformat(format, std::make_format_args(args...));
}

using StringList = std::vector<std::string>;

