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
            return formatter<StringView>::format("Get", context);
        case Category::Set:
            return formatter<StringView>::format("Set", context);
        case Category::City:
            return formatter<StringView>::format("City", context);
        case Category::Weather:
            return formatter<StringView>::format("Weather", context);
        case Category::Fact:
            return formatter<StringView>::format("Fact", context);
        }

        // unreachable
        return context.out();
    }
};

//////////////////////////////////////////////////////////////////////////////

struct Repository
{
   auto init()
   {
      // read optional custom data
      importStopWordList();
      importWordRootList();
      importWordMeaning();
   }

   auto isStopword(const String& word) const
   {
      // filter unnecessary words with minimal meaning
      return m_stopwords.contains(word);
   }

   auto findRoot(const String& word) const -> String
   {
      // find root of word to handle different semantic variations of the input
      // no idea about Stemming / Lemmatization, so do it manually
      if (m_rootword.contains(word))
      {
         return m_rootword.at(word);
      }
      return word;
   }

   auto getIntent(const String& word) const -> std::optional<Category>
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

   void importStopWordList()
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

   void importWordRootList()
   {
      // not implemented
   }

   void importWordMeaning()
   {
      // not implemented
   }

private:
    // medium sized dataset
   std::unordered_set<String> m_stopwords{
      "is", "not", "that", "there", "are", "can", "the",
      "you", "with", "of", "those", "after", "all", "one",
      "me", "an", "a", "in"
   };

   // very big dataset, needs quick read access
   std::map<String, String> m_rootword{{
       {"eating", "eat"},
       {"eaten", "eat"},
       {"eats", "eat"},
       {"eat", "eat"},

       {"facts", "fact"},
       {"fact", "fact"},

       {"whats", "what"},
       {"what", "what"},

       {"berlin", "berlin"},
       {"bärlin", "berlin"},
       {"baerlin", "berlin"},
   }};

   // very big dataset, needs quick read access
   std::map<String, Category> m_meanings{{
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
