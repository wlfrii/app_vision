cmake_minimum_required(VERSION 3.10)

project(libvisongpu
    VERSION 1.1.0
    DESCRIPTION "A library for GPU operation")
message(STATUS "Project: ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "RELEASE")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    include_directories(
            ${PROJECT_SOURCE_DIR}/lib
        )
else()
    find_package(glfw3 REQUIRED)
endif()

set(EXTERNAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external")
message(STATUS "libvisiongpu external path: ${EXTERNAL_PATH}")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/display SRC_DISPLAY)
add_library(${PROJECT_NAME} STATIC
    ${SRC_DISPLAY}
    ${EXTERNAL_PATH}/glad/src/glad.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${EXTERNAL_PATH}/glad/include>
        $<BUILD_INTERFACE:${EXTERNAL_PATH}/glm>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/display>
)

target_link_libraries(${PROJECT_NAME}
    glfw
    ${OpenCV_LIBS}
)

file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/imtexture.vs
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/imtexture.fs
    DESTINATION ${CMAKE_BINARY_DIR}
)
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/display/displayer.h
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/display/
)

# For testing this library
add_subdirectory(test)