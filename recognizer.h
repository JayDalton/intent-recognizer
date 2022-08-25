#pragma once

#include "common.h"

// Intent classes as enum, could also be 
enum class Category { Get, Set, City, Weather, Fact };

// list / set of intents
using ResultList = std::vector<Category>;

template<>
struct std::formatter<Category> : std::formatter<std::string_view>
{
    template<typename Context>
    auto format(const Category state, Context& context)
    {
        switch (state)
        {
        case Category::Get:
            return formatter<std::string_view>::format("Get", context);
        case Category::Set:
            return formatter<std::string_view>::format("Set", context);
        case Category::City:
            return formatter<std::string_view>::format("City", context);
        case Category::Weather:
            return formatter<std::string_view>::format("Weather", context);
        case Category::Fact:
            return formatter<std::string_view>::format("Fact", context);
        }

        // unreachable
        return context.out();
    }
};

//////////////////////////////////////////////////////////////////////////////

struct Repository
{
    void init();

    bool isStopword(const std::string& word) const;

    std::string findRoot(const std::string& word) const;

    std::optional<Category> getIntent(const std::string& word) const;

protected:
    void importStopWordList();

    void importWordRootList();

    void importWordMeaning();

private:
    // medium sized dataset
	std::unordered_set<std::string> m_stopwords{ 
		"is", "not", "that", "there", "are", "can", "the",
		"you", "with", "of", "those", "after", "all", "one",
		"me", "an", "a", "in"
	};

    // very big dataset, needs quick read access
    std::map<std::string, std::string> m_rootword
    { {
        {"eating", "eat"},
        {"eaten", "eat"},
        {"eats", "eat"},
        {"eat", "eat"},

        {"facts", "fact"},
        {"fact", "fact"},

        {"whats", "what"},
        {"what", "what"},

        {"bärlin", "berlin"},
        {"berlin", "berlin"},

    }};

    // very big dataset, needs quick read access
    std::map<std::string, Category> m_meanings
    { {
        {"what", Category::Get},
        {"when", Category::Get},
        {"who", Category::Get},
        {"where", Category::Get},

        {"show", Category::Set},
        {"calc", Category::Set},

        {"berlin", Category::City},
        {"paris", Category::City},
        {"london", Category::City},
        {"madrid", Category::City},

        {"weather", Category::Weather},
        {"rain", Category::Weather},
        {"sun", Category::Weather},
        {"cloud", Category::Weather},
        {"temperatur", Category::Weather},

        {"fact", Category::Fact},
    }};
};

//////////////////////////////////////////////////////////////////////////////

// using proper interfaces makes impl exchangable
// prefer composition over inheritance, e.g. std::variant
struct IRecognizer
{
    virtual ResultList calculate(std::string_view input) = 0;
};

struct Recognizer : public IRecognizer
{
    void initCustom();

    ResultList calculate(std::string_view input);

    StringList normalize(std::string_view input);

    ResultList exctract(const StringList& words);

private:
	Repository m_repo;
    std::once_flag m_init;
};
