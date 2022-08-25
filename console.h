#pragma once

#include "common.h"
#include "recognizer.h"

struct Console
{
    int run();

protected:
    std::optional<std::string> readInput();

    std::string format(const ResultList& input) const;

private:
    // need Interface for different impl?
    Recognizer m_engine;
};


