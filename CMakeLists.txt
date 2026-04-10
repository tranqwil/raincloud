cmake_minimum_required(VERSION 3.30)

project(Raincloud)

set(CMAKE_CXX_STANDARD 20)


#OPEN AL
##set(LIBTYPE "STATIC" CACHE STRING "" FORCE)
##set(ALSOFT_UTILS                  OFF CACHE BOOL "" FORCE)
##set(ALSOFT_EXAMPLES               OFF CACHE BOOL "" FORCE)
##set(ALSOFT_TESTS                  OFF CACHE BOOL "" FORCE)
##set(ALSOFT_INSTALL                OFF CACHE BOOL "" FORCE)
##set(ALSOFT_INSTALL_CONFIG         OFF CACHE BOOL "" FORCE)
##set(ALSOFT_INSTALL_HRTF_DATA      OFF CACHE BOOL "" FORCE)
##set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/openal)


#LIBSNDFILE
set(BUILD_PROGRAMS  OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES  OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING   OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/sndfile)


#RAINCLOUD ENGINE
add_library(Raincloud SHARED
        src/SoundBuffer.cpp
        src/SoundDevice.cpp
        src/SoundSource.cpp
        src/Stream.cpp
)
target_compile_definitions(Raincloud PRIVATE RAINCLOUD_EXPORTS)

target_link_libraries(Raincloud PRIVATE
        OpenAL
        sndfile
)

target_include_directories(Raincloud
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/openal-soft/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/libsndfile/include
)

#RAINCLOUD EXAMPLES
add_executable(example_basic_playback examples/basic_playback.cpp)
add_executable(example_streaming      examples/streaming.cpp)

foreach(example IN ITEMS example_basic_playback example_streaming)
    target_link_libraries(${example}
            PRIVATE
            Raincloud
    )
endforeach()




