cmake_minimum_required(VERSION 3.10)

project(libvisongpu_test)
message(STATUS "Project: ${PROJECT_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 	${CMAKE_BINARY_DIR}/)

add_executable(${PROJECT_NAME}
    main.cpp
)

find_package(OpenCV REQUIRED)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${OpenCV_INCLUDE_DIRS}>
)

link_directories(
    ${CMAKE_BINARY_DIR}
)
message(${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME}
    libvisongpu
    ${OpenCV_LIBS}
)

file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/L_test.bmp
    ${CMAKE_CURRENT_SOURCE_DIR}/R_test.bmp
    DESTINATION ${CMAKE_BINARY_DIR}
)