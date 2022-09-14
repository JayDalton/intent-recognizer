#include "console.h"

// mkdir build && cd build && cmake .. && cmake --build . --config Release

// 'The 500 most used words in the English language 
// have an average of 23 different meanings.'

namespace irs {

    auto f = []() constexpr {
        return sizeof(void*);
    };

    std::array<int, f()> arr = {};

    template <typename... Ts>
    struct overload : Ts... {
        using Ts::operator()...;
    };

    // ---

    struct A{};
    struct B{};
    struct C{};
    using MyType = std::variant<A, B, C>;

    std::map<std::string, MyType> map{{
        {"abc", A{}},
        {"xyz", B{}},
    }};

    // ---

    auto fun = +[](int i){ return i * i; };
    static_assert(std::is_same_v<decltype(fun), int(*)(int)>);

    auto foo(auto... args) {
        print("size of: {}", sizeof...(args));
    }

    template <typename... Args>
    auto delay_invoke_foo(Args... args) {
        return [args...]() -> decltype(auto) {
            return foo(args...);
        };
    }

    struct Widget {
        decltype([]{}) l;
        int x{0};
    };

    using WidgetSet = std::set<Widget, decltype([](Widget& w1, Widget& w2){ return w1.x < w2.x; })>;
    WidgetSet widgets;
}

auto main() -> int
{
    using namespace irs;

    auto map = overload{
        [](int i){ print("int {}\n", i); },
        [](float i){ print("float {}\n", i); },
        [](double i){ print("double {}\n", i); }
    };

    map(42);
    map(3.21f);
    map(6.433);

    for (int i : std::views::iota(125,150))
    {
        print("{} ", i);
    }    

    Console app;
    return app.run();
}

