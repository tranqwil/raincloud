cmake_minimum_required(VERSION 3.30)
project(RainCloud)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenAL paths
set(OPENAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/OpenAL/include)
set(OPENAL_LIB_DIR ${CMAKE_SOURCE_DIR}/external/OpenAL/lib)
set(OPENAL_DLL ${CMAKE_SOURCE_DIR}/external/OpenAL/lib/OpenAL32.dll)  # adjust if needed

#raylib
# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()


# Add the libsndfile subdirectory
add_subdirectory(external/libsndfile)


# Include & link OpenAL
include_directories(${OPENAL_INCLUDE_DIR} external/raylib/include)
link_directories(${OPENAL_LIB_DIR} external/raylib/lib)

# Build target
add_executable(RainCloud
        src/main.cpp
        src/SoundBuffer.cpp
        src/SoundBuffer.h
        src/SoundDevice.cpp
        src/SoundDevice.h
        src/SoundSource.cpp
        src/SoundSource.h
        src/Stream.cpp
        src/Stream.h
        src/GameCamera.cpp
        src/GameCamera.h


)
#set(raylib_VERBOSE 1)
target_link_libraries(RainCloud PUBLIC OpenAL32 raylib PRIVATE sndfile)

# Automatically copy openal32.dll after build
add_custom_command(TARGET RainCloud POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OPENAL_DLL}"
        "$<TARGET_FILE_DIR:RainCloud>"
)

add_custom_command(TARGET RainCloud POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/samples
        $<TARGET_FILE_DIR:RainCloud>/../samples
)
