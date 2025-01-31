cmake_minimum_required(VERSION 3.5)

project(CppLearning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party/spdlog)

function(add_examples root_dir)
    file(GLOB children RELATIVE ${root_dir} ${root_dir}/*)

    foreach(child ${children})
        set(full_path ${root_dir}/${child})

        if(IS_DIRECTORY ${full_path})
            if(child MATCHES "^(third_party|build)$")
                continue()
            endif()

            if(EXISTS ${full_path}/CMakeLists.txt)
                message(STATUS "Found example: ${full_path}")
                add_subdirectory(${full_path})
            else()
                add_examples(${full_path})
            endif()
        endif()
    endforeach()
endfunction()

function(generate_subproject_name OUTPUT)
    get_filename_component(CURRENT_FULL_PATH ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
    get_filename_component(PARENT_DIR_PATH ${CURRENT_FULL_PATH} DIRECTORY)
    get_filename_component(PARENT_DIR_NAME ${PARENT_DIR_PATH} NAME)
    get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    set(PROJECT_NAME "${PARENT_DIR_NAME}_${CURRENT_DIR_NAME}")
    set(${OUTPUT} "${PROJECT_NAME}" PARENT_SCOPE)
endfunction()

add_examples(${CMAKE_CURRENT_SOURCE_DIR})
