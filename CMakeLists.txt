cmake_minimum_required(VERSION 3.15)
project(shape_student CXX)

if(WIN32 AND NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR
        "Program bin/shape.exe jest 64-bitowy, biblioteke trzeba budowac "
        "dla x64. Usun katalog build i skonfiguruj ponownie, np.: "
        "cmake -S . -B build -A x64")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT WIN32 AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# pbPlots (wykresy plotera), pobierane przy pierwszej konfiguracji.
include(FetchContent)
FetchContent_Declare(
    pbplots_fetch
    GIT_REPOSITORY https://github.com/InductiveComputerScience/pbPlots.git
    GIT_TAG        eb4cdc091c23c3143fc37791e0211022f0104a43 # master, 2026-09-08
)
FetchContent_GetProperties(pbplots_fetch)
if(NOT pbplots_fetch_POPULATED)
    FetchContent_MakeAvailable(pbplots_fetch)
endif()

# pbPlots has no CMakeLists.txt, the library target is defined here.
add_library(pbplots STATIC
    ${pbplots_fetch_SOURCE_DIR}/Cpp/pbPlots.cpp
    ${pbplots_fetch_SOURCE_DIR}/Cpp/supportLib.cpp
)
target_include_directories(pbplots PUBLIC
    ${pbplots_fetch_SOURCE_DIR}/Cpp
)

add_library(student SHARED
    include/shape_api.h
    src/plotter.cpp
    src/plotter.h
    src/student.cpp
)

target_include_directories(student PRIVATE include)
target_link_libraries(student PRIVATE pbplots)

if(WIN32)
    # The program loads bin/student.dll.
    add_custom_command(TARGET student POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:student> ${CMAKE_CURRENT_SOURCE_DIR}/bin/student.dll
    )

    # F5 in Visual Studio starts the program with the library.
    set_target_properties(student PROPERTIES
        VS_DEBUGGER_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/shape.exe
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    )
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT student)
else()
    # Only the functions of shape_api.h are exported; no unique symbols, so
    # the library can be reloaded with R while the program is running. pbPlots
    # is linked into the shared library and gets the same options.
    set_target_properties(student pbplots PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        POSITION_INDEPENDENT_CODE ON)
    target_compile_options(student PRIVATE -fno-gnu-unique)
    target_compile_options(pbplots PRIVATE -fno-gnu-unique)

    # The program loads bin/libstudent.so.
    add_custom_command(TARGET student POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:student> ${CMAKE_CURRENT_SOURCE_DIR}/bin/libstudent.so
    )
endif()
