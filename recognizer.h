#pragma once

#include "common.h"

enum class Category { Get, Set, City, Weather, Fact };
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


struct Repository
{
    // aus Dateien/Streams lesen?
    // single and multi-term keyword lists 
    void init()
    {
        importStopWordList();
        importWordRootList();
        importWordMeaning();
    }

    bool isStopword(const std::string& word) const
    {
        // filter unnecessary words with minimal meaning
        return m_stopwords.contains(word);
    }

    std::string findRoot(const std::string& word) const
    {
        // find root of word to handle different semantic variations of the input
        // no idea about Stemming / Lemmatization, so do it manually
        if (m_rootword.contains(word))
        {
            return m_rootword.at(word);
        }
        return word;
    }

    std::optional<Category> getIntent(const std::string& word) const
    {
        // try find meaning of word
        if (m_meanings.contains(word))
        {
            return m_meanings.at(word);
        }
        return std::nullopt;
    }

protected:
    void importStopWordList()
    {
        std::ifstream file("../wordstop.file");
        if (file.is_open())
        {
            print("stop.file\n");
            std::copy(
                std::istream_iterator<std::string>{file},
                std::istream_iterator<std::string>(),
                std::inserter(m_stopwords, m_stopwords.begin())
                // std::ostream_iterator<std::string>(std::cout, "\n")
            );
            print("\n");
        }
    }

    void importWordRootList()
    {
        std::ifstream file("../wordroot.file");
        if (file.is_open())
        {
            print("root.file\n");
            // auto formatter = [](std::string line) -> std::pair<std::string, std::string> 
            // { 
            //     std::istringstream iss{line};
            //     std::vector<std::string> results(
            //         std::istream_iterator<std::string>{iss},
            //         std::istream_iterator<std::string>()
            //     );

            //     if (results.size() != 2)
            //     {
            //         throw std::exception("");
            //     }

            //     return { results.at(0), results.at(1) };
            // };
            
            std::transform(
                std::istream_iterator<std::string>{file},
                std::istream_iterator<std::string>(),
                std::inserter(m_rootword, m_rootword.begin()),
                [](std::string line) -> std::pair<std::string, std::string> { 
                    std::istringstream iss{line};
                    std::vector<std::string> results(
                        std::istream_iterator<std::string>{iss},
                        std::istream_iterator<std::string>()
                    );

                    if (results.size() != 2)
                    {
                        throw std::exception("");
                    }

                    return { results.at(0), results.at(1) };
                }
            );
        }
    }

    void importWordMeaning()
    {
        std::ifstream file("../wordmean.file");
        if (file.is_open())
        {
            print("mean.file\n");
            std::copy(
                std::istream_iterator<std::string>{file},
                std::istream_iterator<std::string>(),
                // std::inserter(m_stopwords, m_stopwords.begin())
                std::ostream_iterator<std::string>(std::cout, "\n")
            );
            print("\n");
            std::transform(
                std::istream_iterator<std::string>{file},
                std::istream_iterator<std::string>(),
                std::inserter(m_meanings, m_meanings.begin()),
                [](std::string line) -> std::pair<std::string, Category> { 
                    std::istringstream iss{line};
                    std::vector<std::string> results(
                        std::istream_iterator<std::string>{iss},
                        std::istream_iterator<std::string>()
                    );

                    if (results.size() != 2)
                    {
                        throw std::exception("");
                    }

                    // auto to_int = [](const std::string& str) -> std::optional<int>
                    // {
                    //     if (int value{0}; std::from_chars(str.data(), str.data() + str.size(), value).ec == std::errc{})
                    //         return value;
                    //     else
                    //         return std::nullopt;
                    // };

                    // very very dirty hack :/
                    const int value{std::atoi(results.at(1).data())};
                    return { results.at(0), static_cast<Category>(value) };
                }
            );
        }
    }

private:
	std::unordered_set<std::string> m_stopwords{ 
		"is", "not", "that", "there", "are", "can", "the",
		"you", "with", "of", "those", "after", "all", "one",
		"me", "an", "a", "in"
	};

    // very big dataset
    std::unordered_map<std::string, std::string> m_rootword
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

    // very big dataset
    std::unordered_map<std::string, Category> m_meanings
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

// using proper interfaces makes impl exchangable
// prefer composition to inheritance, e.g. std::variant
struct IRecognizer
{
    virtual ResultList calculate(std::string_view input) = 0;
};

struct Recognizer : public IRecognizer
{
    void initCustom()
    {
        std::call_once(m_init, [this](){ m_repo.init(); });
    }

    ResultList calculate(std::string_view input)
    {
        // do normalize
        auto word_list = normalize(input);

        // try get intent
        return exctract(word_list);
    }

    StringList normalize(std::string_view input)
    {
        using namespace std::views;

        auto words = input 
            | filter([](unsigned char c) { return std::isalpha(c) || std::isspace(c); })
            | transform([](unsigned char c) { return std::tolower(c); })
            // | filter/remove consecutive spaces, C++23?
            | split(' ')
            | transform([](auto&& c) -> std::string { return { c.begin(), c.end() }; })
            | filter([](auto&& c) { return !c.empty(); })
         ;

        return { words.begin(), words.end() };
    }

    ResultList exctract(const StringList& words)
    {
        using namespace std::views;

        auto data = words
            | filter([this](const std::string& word) { return !m_repo.isStopword(word); })
            | transform([this](const std::string& word) { return m_repo.findRoot(word); })
            | transform([this](const std::string& word) { return m_repo.getIntent(word); })
            | filter([](auto&& opt){ return opt.has_value(); })
            | transform([](auto&& opt){ return opt.value(); })
        ;

        return {data.begin(), data.end()};
    }

private:
	Repository m_repo;
    std::once_flag m_init;
};
