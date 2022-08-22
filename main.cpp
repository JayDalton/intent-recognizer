#include "common.h"
#include "recognizer.h"

struct Console
{
    int run()
    {
        print("Embedded Intent Recognizer\n");

        m_engine.init();

        while (auto input = readInput())
        {
            //(*input) = "What is the weather like today?";

            print("Input: {}\n", input.value());
            auto intent = m_engine.calculate(input.value());
            print("Intend: {}\n", intent.to_string());
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

private:
    Recognizer m_engine;
};

// mkdir build && cd build && cmake .. && cmake --build . --config Release

auto main() -> int
{
    Console app;
    return app.run();
}

