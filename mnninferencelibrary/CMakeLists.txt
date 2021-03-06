


cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -static-openmp --hash-style=sysv")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fopenmp")

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -fvisibility=hidden  -fomit-frame-pointer -fstrict-aliasing -ffast-math -ffunction-sections -fdata-sections -flax-vector-conversions")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -fomit-frame-pointer -fstrict-aliasing -ffast-math -ffunction-sections -fdata-sections -flax-vector-conversions")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -static-openmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -static-openmp")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")
set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -Wl,--gc-sections")


set(lib_DIR ${CMAKE_SOURCE_DIR}/libs)
include_directories(${CMAKE_SOURCE_DIR}/cpp_include)


add_library(libMNN SHARED IMPORTED)
set_target_properties(libMNN PROPERTIES IMPORTED_LOCATION ${lib_DIR}/${ANDROID_ABI}/libMNN.so)


add_library(libopencv_java SHARED IMPORTED)
set_target_properties(libopencv_java PROPERTIES IMPORTED_LOCATION ${lib_DIR}/${ANDROID_ABI}/libopencv_java4.so)

file(GLOB_RECURSE CPP_SRCS
        src/main/cpp/MNNAndroidRecongize.h
        src/main/cpp/MNNAndroidRecongize.cpp
#        src/main/cpp/MNNAndroidDetect.h
#        src/main/cpp/MNNAndroidDetect.cpp
        src/main/cpp/Utils.h
        src/main/cpp/Utils.cpp
        src/main/cpp/mnn_jni.cpp)

add_library(mnnInferenceLibrary SHARED ${CPP_SRCS})

find_library(log-lib log android)
find_library(jnigraphics-lib jnigraphics)

target_link_libraries(mnnInferenceLibrary libMNN libopencv_java z android ${log-lib} ${jnigraphics-lib})







