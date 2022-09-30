# Embedded Intent Recognizer

## Task

Build a small intent recognition command line tool. The input and output looks like this:

> What is the weather like today? => Prints (Intent: Get Weather)

> What is the weather like in Paris today? => Prints (Intent: Get Weather City)

> Tell me an interesting fact. => Prints (Intent: Get Fact)

Note that simply implementing the 3 use cases above without the coding being extendable and scalable will not be
sufficient to pass the test. The code only needs to handle the 3 sentences above but it should be clear how it could
be extended to handle more use cases.

## Requirements

Your code
* has to be buildable with cmake using C++ and the command: `mkdir build && cd build && cmake .. && make`
* should use modern C++ (>= C++17)
* should be pushed to a github repository that we can clone, e.g. https://github.com/
* should have good commit messages
* should have unit tests (Catch2, Google Test or any other framework)

## Hints

You should make your solution stand out by implementing and documenting one of the following areas:
* Handling different semantic variations of the input, i.e. completely generic handling of the input
* Having extendable and well designed software architecture
* Having proven highly scalable performance

