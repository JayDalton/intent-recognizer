cmake_minimum_required(VERSION 3.20)

project(recognizer LANGUAGES CXX VERSION 0.1) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES 
    common.h
    main.cpp
    recognizer.h
)

add_executable(recognizer ${SOURCE_FILES})


Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.9 # or a later release
)

FetchContent_MakeAvailable(Catch2)

# find_package(Catch2 REQUIRED)
add_executable(tests test.cpp)
target_link_libraries(tests Catch2::Catch2)
