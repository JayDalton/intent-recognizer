#pragma once

#include <iostream>
#include <format>
#include <optional>
#include <string>
#include <string_view>

template <typename... Args>
void print(std::string_view format, Args&&... args)
{
    std::cout << std::vformat(format, std::make_format_args(args...));
}

