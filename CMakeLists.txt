# 1. Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# 2. Project name and language
project(MyBrowser VERSION 0.1 LANGUAGES CXX)

# 3. Use Modern C++ (Ladybird uses C++20 or newer!)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. Define your executable (The final program)
# This says: "Take main.cpp and turn it into a program called 'browser'"
add_executable(browser main.cpp parsers/url.cpp)
