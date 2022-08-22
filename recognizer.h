#pragma once

#include "common.h"

struct Recognizer
{
    void init()
    {
        // file import?
		  // stop words?
    }

    StringList normalize(std::string_view input)
    {
        using namespace std::views;

        auto words = input 
            | filter([](unsigned char c) { return std::isalpha(c) || std::isspace(c); })
            | transform([](unsigned char c) { return std::tolower(c); })
            | split(' ')
            | transform([](auto&& c) -> std::string { return { c.begin(), c.end() }; })
            | filter([](auto&& c) { return !c.empty(); })
				// remove consecutive spaces
         ;

        return { words.begin(), words.end() };
    }

};
