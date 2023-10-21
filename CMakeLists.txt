cmake_minimum_required(VERSION 3.23)
project(CppArrai18n_example)
set(CMAKE_CXX_STANDARD 17)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_executable(CppArrai18n_example example.cpp)
target_include_directories(CppArrai18n_example
        PRIVATE include)
