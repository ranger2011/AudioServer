# CMakeList.txt: AudioServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required(VERSION 3.8)
project(AudioServer)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/home/ranger/softwares/cmake")
# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
# set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system thread filesystem date_time regex REQUIRED)
find_package(FFmpeg COMPONENTS AVFORMAT AVCODEC AVUTIL swresample REQUIRED)
if (Boost_FOUND AND FFMPEG_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    #  FFMPEG_INCLUDE_DIRS  - Include directory necessary for using the required components headers.
    #  FFMPEG_LIBRARIES     - Link these to use the required ffmpeg components.
    #  FFMPEG_DEFINITIONS   - Compiler switches required for using the required ffmpeg components.
    message("FFMPEG_INCLUDE_DIRS = ${FFMPEG_INCLUDE_DIRS} ")
    message("FFMPEG_LIBRARIES = ${FFMPEG_LIBRARIES} ")
    message("FFMPEG_DEFINITIONS = ${FFMPEG_DEFINITIONS} ")

    include_directories(${FFMPEG_INCLUDE_DIRS})

    add_executable(AudioServer
            AudioServer.cpp
            protocol.cpp protocol.h
            pack_data.cpp pack_data.h
            interface_constants.h
            interface.cpp interface.h
            Server.cpp Server.h
            crc.cpp crc.h
            ffmpeg_utils.h ffmpeg_utils.cpp
            System.cpp System.h
            )
    target_link_libraries(AudioServer ${Boost_LIBRARIES} ${FFMPEG_LIBRARIES} z)

endif ()
