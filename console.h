#pragma once

#include "common.h"
#include "recognizer.h"

struct Console
{
    int run()
    {
        print("Embedded Intent Recognizer\n");

        // load custom dictionaries
        // m_engine.initCustom(); 

        while (auto input = readInput())
        {
            // some variations of input
            (*input) = "Whats the weather like in Bärlin today?";

            print("Input: {}\n", input.value());
            auto intent = m_engine.calculate(input.value());
            print("Intend: {}\n", format(intent));
        }

        print("End!\n");
        return {};
    }

private:
    std::optional<std::string> readInput()
    {
        while (true)
        {
            try
            {
                print("Input: ");
                std::string input;
                std::getline(std::cin, input);
                if (!input.empty()) {
                    return input;
                }
                return std::nullopt;
            }
            catch (...) {}
        }
    }

    std::string format(const ResultList& input) const
    {
        std::string result;
        for (auto& category : input)
        {
            std::format_to(std::back_inserter(result), "{} ", category);
        }
        return result;
    }

private:
    Recognizer m_engine;
};


