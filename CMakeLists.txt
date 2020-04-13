cmake_minimum_required(VERSION 3.1)

project(multiple-dispatch VERSION 0.1 LANGUAGES CXX)

add_executable(md test/md.cpp)
target_compile_features(md PUBLIC cxx_std_17)
target_include_directories(md PRIVATE include)

enable_testing()
add_test(NAME md COMMAND md)
