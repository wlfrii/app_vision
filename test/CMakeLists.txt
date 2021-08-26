cmake_minimum_required(VERSION 3.10)
project(app_vision_test)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src TEST_SRC)
add_executable(${PROJECT_NAME}
    ${TEST_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/camera/camera_parameters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/camera/map_calculator.cpp
)

set(CATCH_INCLUDE_DIR /usr/local/Cellar/catch2/2.13.7/include/)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CATCH_INCLUDE_DIR}>
        $<BUILD_INTERFACE:${OpenCV_INCLUDE_DIRS}>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src>
)

target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
)
