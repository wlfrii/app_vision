cmake_minimum_required(VERSION 3.10)

# Project Name
project(app_vision
    VERSION 2.0.0
    LANGUAGES CXX
)
message(STATUS "Project: ${PROJECT_NAME}")

# Display current OS
message(STATUS "Current operation system: ${CMAKE_SYSTEM}")
message(STATUS "Host system processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

# Setting
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")


# --------------------------------------------------------------------
#                  Configure for different platform
# --------------------------------------------------------------------
# @Platform
# Checke the current operation system, and then make configuration.
# @Windows
# Which is the most complicated platform.
#     The directory of OpenCV should be specified, particularly when
#     i) there is more than one OpenCV version, and ii) the CUDA is
#     required.
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "Platform: Windows")
    # Set output directory
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64/build)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64/bin)
    # Set OpenCV path depend on CUDA
    set(OpenCV_FOLDER "D:/MyProjects/include")
    if(WITH_CUDA)
        set(OpenCV_DIR ${OpenCV_FOLDER}/opencv_cmake/install/x64/vc15/lib)
    else()
        set(OpenCV_DIR ${OpenCV_FOLDER}/opencv/build/x64/vc15/lib)
    endif(WITH_CUDA)

    add_definitions(-DUNICODE -D_UNICODE)

# @Linux
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "Platform: Linux")
# @Mac OS
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message(STATUS "Platform: Mac OS")
else()
    message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
endif()

# @OpenCV
# The OpenCV library then should be include after checking “WITH_CUDA”
find_package(OpenCV REQUIRED)
if(${OpenCV_FOUND})
    message(STATUS "OpenCV library status:")
    message(STATUS "  version: ${OpenCV_VERSION}")
    message(STATUS "  libraries: ${OpenCV_LIBS}")
    message(STATUS "  include path: ${OpenCV_INCLUDE_DIRS}")
else(${OpenCV_FOUND})
    message(WARNING "Cannot found OpenCV.")
endif(${OpenCV_FOUND})


# @lib_instrument
set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../buildtarget/)
find_package(lib_instrument REQUIRED)

# @Eigen
find_package(EIGEN3 REQUIRED)

# --------------------------------------------------------------------
#                   Create target and set properties
# --------------------------------------------------------------------
# Find the source files
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/camera SRC_CAMERA)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/image SRC_IMAGE)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/util SRC_UTIL)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/linux SRC_LINUX)
add_executable(${PROJECT_NAME}
    ${SRC_SRC}
    ${SRC_CAMERA}
    ${SRC_IMAGE}
    ${SRC_UTIL}
    ${SRC_LINUX}
)
message(${GLAD_PATH}/src)


## ------------- Include the Header files ------------- ##
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIRS}>
        $<BUILD_INTERFACE:${OpenCV_INCLUDE_DIRS}>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

# Link external library
target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
    ${lib_instrument_LIBRARIES}
)


# --------------------------------------------------------------------
#                   Optionally Create targets
# --------------------------------------------------------------------
# @WITH_CUDA
# If use CUDA, then define macro "WITH_CUDA", and the code running
# on GPU will be compiled.
option(WITH_CUDA "select to us GPU" OFF)
message(STATUS "WITH_CUDA: " ${WITH_CUDA})
if(WITH_CUDA)
    add_definitions(-DWITH_CUDA)
endif(WITH_CUDA)


# @OpenGL
# The OpenGL library is used for visualization
option(WITH_GL "select to use OpenGL" ON)
message(STATUS "WITH_GL: ${WITH_GL}")
if(WITH_GL)
    message(STATUS "  Inculde OpenGL")
    add_definitions(-DWITH_GL)
    add_subdirectory(gpu)
endif(WITH_GL)


# @Qt
# If use Qt GUI, then define macro "WITH_QT_GUI", and the code for 
# construting an Qt GUI will be compiled.
option(WITH_QT_GUI "select to use Qt GUI" ON)
message(STATUS "WITH_QT_GUI: ${WITH_QT_GUI}")
if(WITH_QT_GUI)
    message(STATUS "  Inculde Qt")
    add_definitions(-DWITH_QT_GUI)
    add_subdirectory(gui)
endif(WITH_QT_GUI)
