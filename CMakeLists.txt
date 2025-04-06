cmake_minimum_required(VERSION 3.30)
project(RainCloud)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenAL paths
set(OPENAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/OpenAL/include)
set(OPENAL_LIB_DIR ${CMAKE_SOURCE_DIR}/external/OpenAL/lib)
set(OPENAL_DLL ${CMAKE_SOURCE_DIR}/external/OpenAL/lib/OpenAL32.dll)  # adjust if needed

# Add the libsndfile subdirectory
add_subdirectory(external/libsndfile)


# Include & link OpenAL
include_directories(${OPENAL_INCLUDE_DIR})
link_directories(${OPENAL_LIB_DIR})

# Build target
add_executable(RainCloud
        src/main.cpp
        src/SoundBuffer.cpp
        src/SoundBuffer.h
        src/SoundDevice.cpp
        src/SoundDevice.h
        src/SoundSource.cpp
        src/SoundSource.h


)
target_link_libraries(RainCloud PUBLIC OpenAL32 PRIVATE sndfile)

# Automatically copy openal32.dll after build
add_custom_command(TARGET RainCloud POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OPENAL_DLL}"
        "$<TARGET_FILE_DIR:RainCloud>"
)
