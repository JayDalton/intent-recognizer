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
         // (*input) = "Whats the weather like in Bärlin?";
         print("Input: {}\n", input.value());

         auto list = (*input)
            | std::views::transform([](unsigned int c){ return c; })
         ;

         for (auto c : (*input))
         {
            print("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}\n", c);
         }

         // "int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}"
         print(
            "  int: {0:d};\n"
            "  hex: {0:#x};\n"
            "  oct: {0:#o};\n"
            "  bin: {0:#b}\n"
            // , input.value()
            , 'ä'
         );

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
