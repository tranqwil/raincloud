cmake_minimum_required(VERSION 3.30)
project(RainCloud)

set(CMAKE_CXX_STANDARD 20)

# OpenAL paths
set(OPENAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/OpenAL/include)
set(OPENAL_LIB_DIR ${CMAKE_SOURCE_DIR}/external/OpenAL/lib)
set(OPENAL_DLL ${CMAKE_SOURCE_DIR}/external/OpenAL/lib/OpenAL32.dll)  # adjust if needed

# Include & link OpenAL
include_directories(${OPENAL_INCLUDE_DIR})
link_directories(${OPENAL_LIB_DIR})

# Build target
add_executable(RainCloud
        src/main.cpp

)
target_link_libraries(RainCloud OpenAL32)

# Automatically copy openal32.dll after build
add_custom_command(TARGET RainCloud POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OPENAL_DLL}"
        "$<TARGET_FILE_DIR:RainCloud>"
)
