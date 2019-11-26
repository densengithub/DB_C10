#  :copyright: (c) 2017 Alex Huszagh.
#  :license: FreeBSD, see LICENSE.txt for more details.

# Description
# ===========
#
#   Use:
#       Move to a custom directory, ideally out of source, and
#       type `cmake $LXW_SOURCE $FLAGS`, where `LXW_SOURCE` is the
#       path to the libxlsxwriter project, and `FLAGS` are custom
#       flags to pass to the compiler.
#
#   Example:
#       For example, in the build directory, to build libxlsxwriter
#       and the unittests in debug mode, type:
#           cd cmake
#           cmake .. -DBUILD_TESTS=ON
#           make
#           make check_xlsxwriter_unit
#           make install
#
#   Flags:
#       ZLIB_ROOT
#           The ZLIB root directory can be specified either through
#           an environment variable (`export ZLIB_ROOT=/usr/include`)
#           or using a flag with CMake (`-DZLIB_ROOT:STRING=/usr/include`).
#           This sets the preferred search path for the ZLIB installation.
#
#       BUILD_STATIC
#           Build static libraries (default on). To build shared
#           libraries, pass `-DBUILD_STATIC=OFF` during configuration.
#
#       BUILD_TESTS
#           Build unittests (default off). To build the unittests,
#           pass `-DBUILD_TESTS=ON` during configuration.
#
#       BUILD_EXAMPLES
#           Build example files (default off). To build the examples,
#           pass `-DBUILD_EXAMPLES=ON` during configuration.
#
#       USE_STANDARD_TMPFILE
#           Use the standard tmpfile() function (default off). To enable
#           the standard tmpfile, pass `-DUSE_STANDARD_TMPFILE=ON`
#           during configuration. This may produce bugs while cross-
#           compiling or using MinGW/MSYS.
#
#   CMake Options:
#       CMake sets debug and release builds with the `CMAKE_BUILD_TYPE`
#       option, which can be set as a flag during configuration.
#       To build in release mode, pass `-DCMAKE_BUILD_TYPE=Release`
#       during configuration.
#
#   Generators:
#       CMake also supports custom build generators, such as MakeFiles,
#       Ninja, Visual Studio, and XCode. For example, to generate
#       a Visual Studio solution, configure with:
#           cmake .. -G "Visual Studio 14 2015 Win64"
#
#       For more information on using generators, see:
#           https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html
#

# OPTIONS
# -------

# CONFIGURATIONS
# --------------

if(USE_STANDARD_TMPFILE)
    add_definitions(-DUSE_STANDARD_TMPFILE)
endif()


add_definitions(-DUSE_STANDARD_TMPFILE)
include_directories(include include/xlsxwriter)
add_library(xlsxwriter STATIC
    src/app.c
    src/chart.c
    src/content_types.c
    src/core.c
    src/custom.c
    src/drawing.c
    src/format.c
    src/hash_table.c
    src/packager.c
    src/relationships.c
    src/shared_strings.c
    src/styles.c
    src/theme.c
    src/utility.c
    src/workbook.c
    src/worksheet.c
    src/xmlwriter.c
    )