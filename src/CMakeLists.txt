cmake_minimum_required(VERSION 3.17.3)
project(monsoon C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER_NAMES clang)

if ("$ENV{TURMOIL_LOCAL}" STREQUAL "1") 
    set(isLocal TRUE)
else()
    set(isLocal FALSE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(isDebug TRUE)
else()
    set(isDebug FALSE)
endif()

message("isLocal: ${isLocal}")
message("isDebug: ${isDebug}")

set(useSanitizers ${isDebug})
message("useSanitizers: ${useSanitizers}")


if (useSanitizers)
    message("using sanitizers")
    set(sanitizers "-fsanitize=address")
endif()

if (isLocal)
    set(deps ../deps/)
else()
    set(deps ../dependencies/)
endif()        

message("deps:${deps}")

if (APPLE)
        add_compile_definitions(TORNADO_OS_MACOS)
        set(OS_MACOS 1)
elseif (UNIX)
        add_compile_definitions(TORNADO_OS_LINUX)
        set(OS_LINUX 1)
endif()

if (isDebug)
        message("Debug build detected")
        add_compile_definitions(CONFIGURATION_DEBUG TYRAN_CONFIGURATION_DEBUG)
        set(CONFIGURATION_DEBUG 1)
endif()

add_compile_definitions(_POSIX_C_SOURCE=200112L)

include_directories(include)

# Core
include_directories(${deps}clog/src/include)
include_directories(${deps}tyran/src/include)
include_directories(${deps}tiny-libc/src/include)
include_directories(${deps}imprint/src/include)
include_directories(${deps}latch/src/include)
include_directories(${deps}basal-c/src/include)

# Main Window
include_directories(${deps}breathe/src/include)

# Input
include_directories(${deps}sense/src/include)
include_directories(${deps}sense-swamp/src/include)
include_directories(${deps}blare-swamp/src/include)
include_directories(${deps}scream/src/include)

# Audio
include_directories(${deps}thunder/src/include)
include_directories(${deps}scream/src/include)
include_directories(${deps}scream-swamp/src/include)
include_directories(${deps}blare/src/include)

# Resources
include_directories(${deps}burst/src/include)
include_directories(${deps}monsoon-resource/src/include)

# Render
include_directories(${deps}scenery/src/include)
include_directories(${deps}lightning/src/include)
include_directories(${deps}lightning-swamp/src/include)

# Swamp
include_directories(${deps}swamp-runtime/src/include)
include_directories(${deps}swamp-unpack/src/include)
include_directories(${deps}swamp-script-scope/src/include)


# Gameplay

include_directories(${deps}swamp-gameplay/src/include)

file(GLOB_RECURSE deps_src FOLLOW_SYMLINKS
        "${deps}*/src/lib/*.c"
        )

# Remove iOS and SDL (including Webassembly)
list(FILTER deps_src EXCLUDE REGEX ".*sdl_main.c" )
list(FILTER deps_src EXCLUDE REGEX ".*webassembly/main.c" )
list(FILTER deps_src EXCLUDE REGEX ".*src/examples/main.c" )
list(FILTER deps_src EXCLUDE REGEX ".*src/example/main.c" )
list(FILTER deps_src EXCLUDE REGEX ".*src/example/app.c" )
list(FILTER deps_src EXCLUDE REGEX ".*src/platform/ios/.*" )
list(FILTER deps_src EXCLUDE REGEX ".*src/platform/posix/.*" )
list(FILTER deps_src EXCLUDE REGEX ".*src/platform/webassembly/.*" )
list(FILTER deps_src EXCLUDE REGEX ".*src/platform/sdl/.*" )
list(FILTER deps_src EXCLUDE REGEX ".*src/platform/pulseaudio/.*" )


file(GLOB_RECURSE lib_src FOLLOW_SYMLINKS
        "lib/*.c"
        )

function(unixlike)
        message("unixlike")
        file(GLOB_RECURSE deps_platform_src FOLLOW_SYMLINKS
        "${deps}*/src/platform/posix/*.c"
        "${deps}*/src/platform/glfw/*.c"
        )
        message("${deps_platform_src}")

        list(FILTER deps_platform_src EXCLUDE REGEX ".*breathe/src/platform/posix/.*" )                

        set(deps_platform_src ${deps_platform_src} PARENT_SCOPE)
endfunction()

if (OS_LINUX)
        message("Linux Detected!")
        unixlike()
elseif (OS_MACOS)
        message("MacOS detected!")
        unixlike()
endif()


add_executable(monsoon
        ${deps_src}
        ${lib_src}
        ${deps_platform_src}
        ${deps}thunder/src/platform/sdl/sound_driver.c
        ${deps}thunder/src/platform/sdl_common/sdl_sound_driver.c
        examples/main.c
        )

set_target_properties(monsoon
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


if (useSanitizers)
    target_link_libraries(monsoon ${sanitizers})
endif()


target_compile_options(monsoon PRIVATE -Wall -Wno-unused-variable ${sanitizers})

if (NOT isDebug)
    message("optimize!")
    target_compile_options(monsoon PRIVATE -O3) # -flto)
endif()

find_package (Vulkan 1.2 REQUIRED)
if (Vulkan_FOUND)
    include_directories(${Vulkan_INCLUDE_DIRS})
    target_link_libraries (monsoon ${Vulkan_LIBRARIES})
endif (Vulkan_FOUND)
find_package(glfw3 3.3 REQUIRED)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(monsoon ${SDL2_LIBRARIES})


# Opus
find_package(Opus REQUIRED)
message("tjoho:${OPUS_INCLUDE_DIRS}")
include_directories(${OPUS_INCLUDE_DIRS})
include_directories("${OPUS_INCLUDE_DIRS}/opus")
target_link_libraries(monsoon ${OPUS_LIBRARIES})



target_link_libraries(monsoon m)
target_link_libraries(monsoon glfw)

