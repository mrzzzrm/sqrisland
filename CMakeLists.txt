cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project("Sqr.Island" C CXX)

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(SDL_ttf REQUIRED)

set(PLATFORM "PLATFORM_LINUX" CACHE STRING "Platform to build for (WINDOWS, LINUX, WIZ, PANDORA, CAANOO)")
add_definitions(-D${PLATFORM})

add_subdirectory(src)
