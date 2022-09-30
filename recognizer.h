#pragma once

#include "repository.h"

// using proper interfaces makes impl exchangable
// prefer composition over inheritance, e.g. std::variant
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

   auto calculate(StringView input) -> ResultList
   {
      // no idea of nlp-ai based black-box
      // so do it mannually...
      return exctract(normalize(input));
   }

   auto normalize(StringView input) -> StringList
   {
      using namespace std::views;

      auto words = input 
         | filter([](unsigned char c) { return std::isalpha(c) || std::isspace(c); })
         | transform([](unsigned char c) { return std::tolower(c); })
         // | filter/remove consecutive spaces, C++23?
         | split(' ')
         | transform([](auto&& c) -> String { return { c.begin(), c.end() }; })
         | filter([](auto&& c) { return !c.empty(); })
      ;

      return { words.begin(), words.end() };
   }

   auto exctract(const StringList& words) -> ResultList
   {
       using namespace std::views;

       auto intent = words
           | filter([this](const String& word) { return !m_repo.isStopword(word); })
           | transform([this](const String& word) { return m_repo.findRoot(word); })
           | transform([this](const String& word) { return m_repo.getIntent(word); })
           | filter([](auto&& opt){ return opt.has_value(); })
           | transform([](auto&& opt){ return opt.value(); })
       ;

       return { intent.begin(), intent.end() };
   }

private:
   Repository m_repo;
   std::once_flag m_init;
};
