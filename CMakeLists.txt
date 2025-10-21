cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(my-game)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCE_DIR "src/")
set(TESTS_DIR "tests/")

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
  endif()
endif()

add_subdirectory(external/raylib-cpp)

# Tests
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests "${TESTS_DIR}/test-main.cpp")
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# Project files
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")

# Our Project
add_executable(${PROJECT_NAME} "${SOURCES}")
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib raylib_cpp)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
