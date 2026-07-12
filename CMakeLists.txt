cmake_minimum_required(VERSION 3.10)
project(pp5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_DIR "C:/Users/grhig/boulder_dodge/SFML-3.1.0/lib/cmake/SFML" CACHE PATH "")
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(pp5
    main.cpp
    dripstone.h
)

target_link_libraries(pp5 PRIVATE SFML::Graphics SFML::Window SFML::System)
if (WIN32)
    add_custom_command(
        TARGET pp5 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:pp5>
            $<TARGET_FILE_DIR:pp5>
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Data"
            $<TARGET_FILE_DIR:pp5>/Data
        COMMAND_EXPAND_LISTS
    )
endif()
