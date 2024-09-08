cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Path to assets
set(ASSETS_PATH "\"${CMAKE_CURRENT_SOURCE_DIR}/assets/\"")

# Configure config.h from config.h.in
set(CONFIG_HEADER_IN "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in")
set(CONFIG_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/config.hpp")
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
configure_file(${CONFIG_HEADER_IN} ${CONFIG_HEADER})

# Declare cpp files
set(SOURCES
    src/main.cpp
    src/Particle.cpp
    src/Effect.cpp
    src/ThreadPool.cpp
)

set(HEADERS
    src/config.hpp
    src/defines.hpp
    src/ThreadPool.hpp
    src/Particle.hpp
    src/Effect.hpp
)

# Fetch SFML from GitHub
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main ${SOURCES} ${HEADERS})

# Link all SFML components
target_link_libraries(main PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

# Set C++ standard
target_compile_features(main PRIVATE cxx_std_17)

# Copy OpenAL DLL on Windows (for sound support)
if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()
