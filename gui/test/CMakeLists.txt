cmake_minimum_required(VERSION 3.10)

project(libvisongui_test)
message(STATUS "Project: ${PROJECT_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 	${CMAKE_BINARY_DIR}/)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

link_directories(
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME}
    libvisongui
)

