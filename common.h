#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <iterator>
#include <format>
#include <fstream>
#include <map>
#include <mutex>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_set>
#include <unordered_map>
#include <variant>
#include <vector>

using String = std::string;
using StringList = std::vector<String>;
using StringOpt = std::optional<String>;
using StringView = std::string_view;

template <typename... Args>
void print(StringView format, Args&&... args)
{
   std::cout << std::vformat(format, std::make_format_args(args...));
}
