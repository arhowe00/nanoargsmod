cmake_minimum_required(VERSION 3.28)
project(nanoargsmod CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.0
  FIND_PACKAGE_ARGS NAMES Catch2)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(libnanoargsmod)
add_subdirectory(tests)
