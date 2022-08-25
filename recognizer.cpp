#include "recognizer.h"

void Repository::init()
{
    // read optional custom data
    importStopWordList();
    importWordRootList();
    importWordMeaning();
}

bool Repository::isStopword(const std::string& word) const
{
    // filter unnecessary words with minimal meaning
    return m_stopwords.contains(word);
}

std::string Repository::findRoot(const std::string& word) const
{
    // find root of word to handle different semantic variations of the input
    // no idea about Stemming / Lemmatization, so do it manually
    if (m_rootword.contains(word))
    {
        return m_rootword.at(word);
    }
    return word;
}

std::optional<Category> Repository::getIntent(const std::string& word) const
{
    // try find meaning of word
    // one word can have multiple meanings
    // should be weigted list by context?

    if (m_meanings.contains(word))
    {
        return m_meanings.at(word);
    }
    return std::nullopt;
}

void Repository::importStopWordList()
{
    std::ifstream file("../wordstop.file");
    if (file.is_open())
    {
        std::copy(
            std::istream_iterator<std::string>{file},
            std::istream_iterator<std::string>(),
            std::inserter(m_stopwords, m_stopwords.begin())
        );
    }
}

void Repository::importWordRootList()
{
    // not implemented
}

void Repository::importWordMeaning()
{
    // not implemented
}

//////////////////////////////////////////////////////////////////////////////

void Recognizer::initCustom()
{
    // load optional custom data
    std::call_once(m_init, [this](){ m_repo.init(); });
}

ResultList Recognizer::calculate(std::string_view input)
{
    // no idea of nlp-ai based black-box
    // so do it mannually...

    // do normalize
    auto word_list = normalize(input);

    // try get intent
    return exctract(word_list);
}

StringList Recognizer::normalize(std::string_view input)
{
    // filter unimportant noise
    // normalize input data by
    // a) filter non-alpha
    // b) format to lower
    // c) split by spaces
    // d) parallelize algo

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

ResultList Recognizer::exctract(const StringList& words)
{
    // find possible intent to words by
    // a) filter unimportant noise by stopwords
    // b) transform words to their root / stem
    // c) try find meaning of word
    // d) parallelize algo

    using namespace std::views;

    auto intent = words
        | filter([this](const std::string& word) { return !m_repo.isStopword(word); })
        | transform([this](const std::string& word) { return m_repo.findRoot(word); })
        | transform([this](const std::string& word) { return m_repo.getIntent(word); })
        | filter([](auto&& opt){ return opt.has_value(); })
        | transform([](auto&& opt){ return opt.value(); })
    ;

    return {intent.begin(), intent.end()};
}
