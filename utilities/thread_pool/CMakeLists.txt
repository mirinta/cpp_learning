generate_subproject_name(PROJECT_NAME)

project(${PROJECT_NAME})

file(GLOB CURRENT_SOURCES *.cpp)

file(GLOB CURRENT_HEADERS *.h)

add_executable(${PROJECT_NAME} ${CURRENT_SOURCES} ${CURRENT_HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
