cmake_minimum_required(VERSION 3.15)

project(Quill
    VERSION 0.1.0
    LANGUAGES C
)

#-------------------------------------------------
# Output directories (flat structure)
# Works correctly with both Ninja and Visual Studio
#-------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# Needed for multi-config generators (Visual Studio)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}")
endforeach()

#-------------------------------------------------
# Language settings
#-------------------------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

#-------------------------------------------------
# Compiler warnings
#-------------------------------------------------
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#-------------------------------------------------
# Sources
#-------------------------------------------------
file(GLOB_RECURSE QUILL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c"
)
list(SORT QUILL_SOURCES)

add_executable(quill ${QUILL_SOURCES})

target_include_directories(quill PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Src"
)

##-------------------------------------------------
# Convenience targets
#-------------------------------------------------
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "==== Run Log ===="
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND $<TARGET_FILE:quill>
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "================="
    COMMENT "Running quill"
    USES_TERMINAL
    VERBATIM
)

add_custom_target(build-and-run
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "==== Run Log ===="
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND $<TARGET_FILE:quill>
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "================="
    DEPENDS quill
    COMMENT "Building and running quill"
    USES_TERMINAL
    VERBATIM
)
#-------------------------------------------------
# Testing (ready for later)
#-------------------------------------------------
include(CTest)
if(BUILD_TESTING)
    # add_subdirectory(Tests)
endif()
