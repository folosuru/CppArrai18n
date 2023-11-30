cmake_minimum_required(VERSION 3.23)
project(CppArrai18n CXX)
set(CMAKE_CXX_STANDARD 17)
add_library(CppArrai18n STATIC
        ${PROJECT_SOURCE_DIR}/include/CppArrai18n/Arrai18n_def.hpp
        ${PROJECT_SOURCE_DIR}/include/CppArrai18n/Arrai18n.hpp
)
set_target_properties(CppArrai18n PROPERTIES LINKER_LANGUAGE CXX)