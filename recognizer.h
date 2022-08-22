#pragma once

#include "common.h"

enum class Category { Get, Set, City, Weather, Fact };

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

struct Result : public std::vector<Category> 
{
    std::string to_string() const
    {
        std::string result;
        for (auto& category : *this)
        {
            std::format_to(std::back_inserter(result), "{} ", category);
        }
        return result;
    }
};

struct Repository
{
    // aus Dateien/Streams lesen?
    // single and multi-term keyword lists 

    Result findIntent(const StringList& input)
    {
        Result result;
        for (auto& word : input)
        {
            if (m_meanings.contains(word))
            {
                result.push_back(m_meanings.at(word));
            }
        }

        return result;
    }

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

struct Recognizer
{
    void init()
    {
        // file import stop words?
    }

    Result calculate(std::string_view input)
    {
        // normalize
        auto word_list = normalize(input);

        // get candidates
        auto content_words = exctract(word_list);

        // find candidates in big tree
        return m_repo.findIntent(content_words);
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

    StringList exctract(const StringList& words)
    {
        using namespace std::views;

        auto data = words
            | filter([this](const std::string& word) { return !m_stopwords.contains(word); })
            // Stemming / Lemmatization 
        ;

        return {data.begin(), data.end()};
    }

private:
	Repository m_repo;
	std::unordered_set<std::string> m_stopwords{ 
		"is", "not", "that", "there", "are", "can", "the",
		"you", "with", "of", "those", "after", "all", "one",
		"me", "an", "a", "in"
	};
};
