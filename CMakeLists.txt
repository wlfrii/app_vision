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
# @WITH_CUDA
# If use CUDA, then define macro "WITH_CUDA", and the code running 
# on GPU will be compiled.
option(WITH_CUDA "select to us GPU" OFF)
message(STATUS "WITH_CUDA: " ${WITH_CUDA})
if(WITH_CUDA)
    add_definitions(-DWITH_CUDA)
endif(WITH_CUDA)

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
    if(WITH_CUDA)
        set(OpenCV_DIR "D:/MyProjects/include/opencv_cmake/install/x64/vc15/lib")
    else()
        set(OpenCV_DIR "D:/MyProjects/include/opencv/build/x64/vc15/lib")
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

# @GLFW
# The OpenGL library is used for visualization
option(WITH_GL "select to use OpenGL" OFF)
message(STATUS "WITH_GL: ${WITH_GL}")
if(WITH_GL)
    if(CMAKE_SYSTEM_NAME MATCHES "Windows")
        include_directories(
            ${PROJECT_SOURCE_DIR}/lib
        )
    else()
        find_package(glfw3 REQUIRED)
    endif()
endif(WITH_GL)

# @Qt
# If use Qt GUI, then define macro "WITH_QT_GUI", and the code for 
# construting an Qt GUI will be compiled.
# 
option(WITH_QT_GUI "select to use Qt GUI" ON)
message(STATUS "WITH_QT_GUI: ${WITH_QT_GUI}")
if(WITH_QT_GUI)
    message(STATUS "  Inculde Qt")
    add_definitions(-DWITH_QT_GUI)
    # set moc
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    #set(CMAKE_AUTOUIC ON)
    # since the built files will be stored in include directory,
    # current path should be included
    if(CMAKE_VERSION VERSION_LESS "3.7.0")
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
    endif()
    message("CMAKE_INCLUDE_CURRENT_DIR: ${CMAKE_INCLUDE_CURRENT_DIR}")
    # include Qt
    find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)
    message(STATUS "Qt6 directory: ${Qt6_DIR}")
endif(WITH_QT_GUI)

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
aux_source_directory(${PROJECT_SOURCE_DIR}/src/def SRC_DEF)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/ui SRC_UI)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/dm SRC_DM)
add_executable(${PROJECT_NAME}
    ${SRC_SRC}
    ${SRC_CAMERA}
    ${SRC_IMAGE}
    ${SRC_DEF}
    ${SRC_UI}
    ${SRC_DM}
)
message("CMAKE_INCLUDE_CURRENT_DIR 2:${CMAKE_INCLUDE_CURRENT_DIR}")

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
if(WITH_GL)
    target_link_libraries(${PROJECT_NAME}
        glfw3
    )
endif(WITH_GL)
if(WITH_QT_GUI)
    #message("Link Qt libraries")
    target_link_libraries(${PROJECT_NAME}
        Qt6::Widgets Qt6::Core Qt6::Gui
    )
endif(WITH_QT_GUI)




# Set the output path where your program will be created
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

# # Include the Header files
# include_directories(
#     ${CMAKE_SOURCE_DIR}/lib_vision_gpu/src
# )
# include_directories(${CUDA_INCLUDE_DIRS})
# # Include the libraries
# link_directories(
#     ${CMAKE_BINARY_DIR}/lib/lib_vision_gpu
# )

# aux_source_directory(./src SRC_SRC)
# aux_source_directory(./src/def DEF_SRC)
# aux_source_directory(./src/unix UNIX_SRC)

# # message("the src is ${UNIX_SRC}" )

# add_executable(${PROJECT_NAME}
#     ${SRC_SRC}
#     ${DEF_SRC}
#     ${UNIX_SRC}
# )

# # link external library
# target_link_libraries(${PROJECT_NAME}
#     turbojpeg
#     lib_vision_gpu
# )
