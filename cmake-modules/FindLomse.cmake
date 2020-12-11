#  - Try to find Lomse
# Once done this will define
#
#  Lomse_FOUND - system has Lomse
#  Lomse_VERSION - detected version of Lomse
#  Lomse_INCLUDE_DIRS - the Lomse include directories
#  Lomse_LIBRARIES - Link these to use Lomse
#  Lomse_DEFINITIONS - Compiler switches required for using Lomse
#
# See documentation on how to write CMake scripts at
# http://www.cmake.org/Wiki/CMake:How_To_Find_Libraries

find_package(PkgConfig)
pkg_check_modules(Lomse QUIET liblomse)

set(Lomse_DEFINITIONS ${PC_LOMSE_CFLAGS_OTHER})
set(Lomse_INCLUDE_DIRS ${PC_LOMSE_INCLUDE_DIRS} )
set(Lomse_LIBRARIES ${PC_LOMSE_LIBRARIES} )

# handle the QUIETLY and REQUIRED arguments and set Lomse_FOUND to TRUE
# if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
        Lomse 
        FOUND_VAR Lomse_FOUND
        REQUIRED_VARS Lomse_LIBRARIES Lomse_INCLUDE_DIRS
        VERSION_VAR Lomse_VERSION
)

mark_as_advanced(Lomse_INCLUDE_DIRS Lomse_LIBRARIES)

