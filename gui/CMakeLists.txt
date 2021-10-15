cmake_minimum_required(VERSION 3.10)

project(libvisongui
    VERSION 1.1.0
    DESCRIPTION "A GUI library designed for vision project")
    message(STATUS "Project: ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "RELEASE")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# set moc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)
# since the built files will be stored in include directory,
# current path should be included
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# include Qt
find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)
message(STATUS "Qt6 directory: ${Qt6_DIR}")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ui SRC_UI)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/display SRC_DISPLAY)
add_library(${PROJECT_NAME} STATIC
    ${SRC_UI}
    ${SRC_DISPLAY}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Widgets Qt6::Core Qt6::Gui
)

# For testing libvisiongui
add_subdirectory(test)
