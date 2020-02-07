cmake_minimum_required(VERSION 3.5)

project(testlibav)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_library(AVCODEC_LIBRARY avcodec)
find_library(AVFORMAT_LIBRARY avformat)
find_library(AVFILTER_LIBRARY avfilter)
find_library(AVDEVICE_LIBRARY avdevice)
find_library(AVUTIL_LIBRARY avutil)

find_library(AVRESAMPLE_LIBRARY avresample)

find_library(SWRESAMPLE_LIBRARY swresample)
find_library(SWSCALE_LIBRARY swscale)
find_library(KVAZAAR_LIBRARY kvazaar)


find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)

add_executable(testlibav main.cpp convert.cpp)

target_include_directories(testlibav PRIVATE
    ${AVCODEC_INCLUDE_DIR}
    ${AVFORMAT_INCLUDE_DIR}
    ${AVUTIL_INCLUDE_DIR}
    ${AVDEVICE_INCLUDE_DIR}
    )

# order of libraries added matters
target_link_libraries(testlibav PRIVATE
    ${AVFORMAT_LIBRARY}
    ${AVCODEC_LIBRARY}
    ${AVFILTER_LIBRARY}
    ${AVDEVICE_LIBRARY}
    ${AVUTIL_LIBRARY}
    ${AVRESAMPLE_LIBRARY}
    ${SWRESAMPLE_LIBRARY}
    ${SWSCALE_LIBRARY}

    ${KVAZAAR_LIBRARY}
)




