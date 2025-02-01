cmake_minimum_required(VERSION 3.5)

project(cmake_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(libs/static_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE static_lib)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)

add_subdirectory(libs/dynamic_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE dynamic_lib)
