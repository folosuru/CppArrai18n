cmake_minimum_required(VERSION 3.23)
project(CppArrai18n CXX)
set(CMAKE_CXX_STANDARD 17)
add_library(CppArrai18n STATIC
        ${PROJECT_SOURCE_DIR}/src/Arrai18n.cpp
)
target_include_directories(CppArrai18n PRIVATE ${PROJECT_SOURCE_DIR}/include)
