
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")

add_definitions(-DWEBRTC_POSIX)
add_definitions(-DWEBRTC_ANDROID)

find_library(log-lib log)


include_directories(src/main/jni/vad_src/)
aux_source_directory(src/main/jni/vad_src/ VAD_SRCS)
add_library(vad-lib SHARED ${VAD_SRCS})
target_link_libraries(vad-lib ${log-lib} )

include_directories(src/main/jni/serial_src/)
aux_source_directory(src/main/jni/serial_src/ SERIAL_SRCS)
add_library(serial-lib SHARED ${SERIAL_SRCS})
target_link_libraries(serial-lib ${log-lib})