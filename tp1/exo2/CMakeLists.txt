cmake_minimum_required(VERSION 3.30)
project(The_silent_killer C)

set(CMAKE_C_STANDARD 99)

add_executable(The_silent_killer exercice1.c)

# add pthread, math
target_link_libraries(The_silent_killer m pthread)

# If build tag is Release, add -O3
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(The_silent_killer PRIVATE -O3)
endif()


# Check if environment variables are set and use them to set the definitions
if(DEFINED ENV{USE_STRUCT})
    add_definitions(-DUSE_STRUCT=$ENV{USE_STRUCT})
else()
    add_definitions(-DUSE_STRUCT=1) # Default value
endif()

if(DEFINED ENV{PADDING})
    add_definitions(-DPADDING=$ENV{PADDING})
else()
    add_definitions(-DPADDING=1) # Default value
endif()