cmake_minimum_required(VERSION 3.5)

project(dynamic_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB CURRENT_SOURCES *.cpp)

add_library(${PROJECT_NAME} SHARED ${CURRENT_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
