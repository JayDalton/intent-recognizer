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

      while (auto input = readInputNew())
      {
         for (auto c : (*input))
         {
            print(L"int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}\n", c);
         }

         // print("\n");

         // print("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}\n", 'e');
         // // print("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}\n", L"a");
         // print("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}\n", 'ä');
         // // print("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}\n", L'ä');         
      }

      while (auto input = readInput())
      {
         // some variations of input
         // (*input) = "Whats the weather like in Bärlin?";
         print("Input: {}\n", input.value());

         auto intent = m_engine.calculate(input.value());
         print("Intend: {}\n", format(intent));
      }

      print("End!\n");
      return {};
   }

private:
   StringOpt readInput()
   {
       while (true)
       {
           try
           {
               String input;
               print("Input: ");
               std::getline(std::cin, input);
               if (!input.empty()) {
                  return input;
               }
               return std::nullopt;
           }
           catch (...) {}
       }
   }

   std::optional<std::wstring> readInputNew()
   {
       while (true)
       {
           try
           {
               print("Input: ");
               std::wstring input;
               std::getline(std::wcin, input);
               if (!input.empty()) {
                  return input;
               }
               return std::nullopt;
           }
           catch (...) {}
       }
   }

   String format(const ResultList& input) const
   {
      String result;
      for (auto& category : input)
      {
         std::format_to(std::back_inserter(result), "{} ", category);
      }
      return result;
   }

private:
   Recognizer m_engine;
};
