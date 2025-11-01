cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(my-game)

set(CMAKE_C_FLAGS "-O2")
set(CMAKE_CXX_FLAGS "-O2")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Static linking configuration
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

# Force static libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Set RPATH to look for libraries in the same directory as executable
if(UNIX)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
endif()

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

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

add_executable(tests "${TESTS_DIR}/test-main.cpp")
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# Project files
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")

# Our Project
add_executable(${PROJECT_NAME} ${SOURCES})
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib raylib_cpp fmt::fmt)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")

    # Set RPATH for macOS
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "@executable_path"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()

# Create helper script for bundling libraries
if(UNIX AND NOT APPLE)
    # Create the copy script at configure time
    file(WRITE "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "#!/bin/bash\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "EXECUTABLE=\"@EXECUTABLE@\"\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "TARGET_DIR=\"@TARGET_DIR@\"\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "echo \"Copying shared library dependencies...\"\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "ldd \"$EXECUTABLE\" | grep '=>' | awk '{print $3}' | grep -v '(0x' | while read lib; do\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "    if [ -f \"$lib\" ]; then\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "        basename_lib=$(basename \"$lib\")\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "        case \"$lib\" in\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "            */libc.so*|*/libm.so*|*/libdl.so*|*/libpthread.so*|*/ld-linux*.so*|*/libgcc_s.so*|*/libstdc++.so*|*/libresolv.so*|*/librt.so*)\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "                # Skip system libraries\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "                ;;\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "            *)\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "                if [ ! -f \"$TARGET_DIR/$basename_lib\" ]; then\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "                    echo \"Copying $lib\"\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "                    cp -L \"$lib\" \"$TARGET_DIR/\"\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "                fi\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "                ;;\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "        esac\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "    fi\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "done\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs.sh.in" "echo \"Library bundling complete!\"\n")

    # Generate the actual script with substituted values
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -DEXECUTABLE="$<TARGET_FILE:${PROJECT_NAME}>"
            -DTARGET_DIR="$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            -DINPUT_FILE="${CMAKE_BINARY_DIR}/copy_libs.sh.in"
            -DOUTPUT_FILE="${CMAKE_BINARY_DIR}/copy_libs.sh"
            -P "${CMAKE_BINARY_DIR}/configure_script.cmake"
        COMMAND chmod +x "${CMAKE_BINARY_DIR}/copy_libs.sh"
        COMMAND "${CMAKE_BINARY_DIR}/copy_libs.sh"
        COMMENT "Bundling shared libraries with executable"
    )

    # Create the CMake script that does the substitution
    file(WRITE "${CMAKE_BINARY_DIR}/configure_script.cmake"
        "file(READ \"\${INPUT_FILE}\" CONTENT)\n"
        "string(REPLACE \"@EXECUTABLE@\" \"\${EXECUTABLE}\" CONTENT \"\${CONTENT}\")\n"
        "string(REPLACE \"@TARGET_DIR@\" \"\${TARGET_DIR}\" CONTENT \"\${CONTENT}\")\n"
        "file(WRITE \"\${OUTPUT_FILE}\" \"\${CONTENT}\")\n"
    )
endif()

if(WIN32)
    # Windows: Copy DLLs next to executable
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()

add_custom_command(TARGET spin-or-die PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:spin-or-die>/assets/)

if(APPLE)
    # macOS: Copy dylibs next to executable
    file(WRITE "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "#!/bin/bash\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "EXECUTABLE=\"$1\"\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "TARGET_DIR=\"$2\"\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "echo \"Copying shared library dependencies...\"\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "otool -L \"$EXECUTABLE\" | grep -v '/System/' | grep -v '/usr/lib/' | awk '{print $1}' | tail -n +2 | while read lib; do\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "    if [ -f \"$lib\" ]; then\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "        echo \"Copying $lib\"\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "        cp -n \"$lib\" \"$TARGET_DIR/\" 2>/dev/null || true\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "    fi\n")
    file(APPEND "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "done\n")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND chmod +x "${CMAKE_BINARY_DIR}/copy_libs_macos.sh"
        COMMAND "${CMAKE_BINARY_DIR}/copy_libs_macos.sh" "$<TARGET_FILE:${PROJECT_NAME}>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Bundling shared libraries with executable"
    )
endif()
