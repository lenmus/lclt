#---------------------------------------------------------------------------------------
#  This file is part of lclt (Lomse command-line tool) program
#  Copyright (c) 2015-2020 LenMus project
#
#  This program is free software; you can redistribute it and/or modify it under the
#  terms of the GNU General Public License as published by the Free Software Foundation,
#  either version 3 of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful, but WITHOUT ANY
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
#  PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along with this
#  program. If not, see <http:#www.gnu.org/licenses/>.
#
#  For any comment, suggestion or feature request, please contact the manager of
#  the project at s.cecilio@gmail.com
#
#---------------------------------------------------------------------------------------

#-------------------------------------------------------------------------------------
# This is a CMake configuration file for building makefiles and installfiles for
# the lclt (Lomse command-line tool) program
#
# To use it you need CMake which can be downloaded from http://www.cmake.org/
#
# Usage
#   cmake [<options>] <source-tree>
#
# example:
#   cmake ../lclt
#
# The default target (in Linux) is "Unix Makefiles". If you would like to generate
# a diferent makefile use option -G<desired-target>. For instance:
#   cmake -G "CodeBlocks - Unix Makefiles" ...
#
# The default installation prefix is "/usr/local". You can change the install 
# location by specifying option CMAKE_INSTALL_PREFIX:
#   cmake -DCMAKE_INSTALL_PREFIX=/new/install/prefix ...
#
#-------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)

# project name
project(lclt)
set( LCLT_APP_NAME     "\"lclt (Lomse command-line tool)\"" )
set( LCLT_VENDOR_NAME  "\"LenMus\"" )
set( LCLT_VENDOR_SITE  "\"https://github.com/lenmus/lclt\"" )

# main directories 
set( ROOT_DIR  ${CMAKE_CURRENT_SOURCE_DIR} )
set( SRC_DIR  ${ROOT_DIR}/src )
set( OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/bin )
set( EXECUTABLE_OUTPUT_PATH ${OUTDIR})

# directories to search for CMake modules
set( CMAKE_MODULE_PATH    "${ROOT_DIR}/cmake-modules" )

# force to use c++11
if (CMAKE_VERSION VERSION_LESS "3.1")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else ()
    set(CMAKE_CXX_STANDARD 11)				#require c+11 or greater
    set(CMAKE_CXX_STANDARD_REQUIRED ON) 	#prevent fallback to any previous standard
endif ()


# macro STRING_UNQUOTE(var str)
#   Remove quote marks or double quote marks around a string.
#   If the string is not quoted, then content of str is copied to var
#   * Parameters:
#     + var: A variable that stores the result.
#     + str: A string.
#------------------------------------------------------
MACRO(STRING_UNQUOTE var str)
	SET(_ret "${str}")
	STRING(LENGTH "${str}" _strLen)

	# if _strLen > 1
	#   if lCh and rCh are both "'"
	#      Remove _lCh and _rCh
	#   elseif lCh and rCh are both "\""
	#      Remove _lCh and _rCh
	# Otherwise don't touch
	if(_strLen GREATER 1)
	    STRING(SUBSTRING "${str}" 0 1 _lCh)
	    MATH(EXPR _strLen_1 ${_strLen}-1)
	    MATH(EXPR _strLen_2 ${_strLen_1}-1)
	    STRING(SUBSTRING "${str}" ${_strLen_1} 1 _rCh)
	    #MESSAGE("_lCh=${_lCh} _rCh=${_rCh} _ret=|${_ret}|")
	    if("${_lCh}" STREQUAL "'" AND "${_rCh}" STREQUAL "'")
		    STRING(SUBSTRING "${_ret}" 1 ${_strLen_2} _ret)
        ELSEIF ("${_lCh}" STREQUAL "\"" AND "${_rCh}" STREQUAL "\"")
		    STRING(SUBSTRING "${_ret}" 1 ${_strLen_2} _ret)
	    endif()
	endif()
	SET(${var} "${_ret}")
ENDMACRO()


# set up configuration variables for LCLT_config.h
#------------------------------------------------------

# version. Extract values from lclt_version.h header file
#file(STRINGS ${ROOT_DIR}/include/lclt_version.h LCLT_VERSION_LIST)
#list (GET LCLT_VERSION_LIST 5 MAJOR_LINE)
#list (GET LCLT_VERSION_LIST 6 MINOR_LINE)
#list (GET LCLT_VERSION_LIST 7 PATCH_LINE)
#list (GET LCLT_VERSION_LIST 8 TYPE_LINE)
#string(REGEX REPLACE "\#define LCLT_VERSION_MAJOR   " "" LCLT_VERSION_MAJOR "${MAJOR_LINE}")
#string(REGEX REPLACE "\#define LCLT_VERSION_MINOR   " "" LCLT_VERSION_MINOR "${MINOR_LINE}")
#string(REGEX REPLACE "\#define LCLT_VERSION_PATCH   " "" LCLT_VERSION_PATCH "${PATCH_LINE}")
#string(REGEX REPLACE "\#define LCLT_VERSION_TYPE    " "" LCLT_VERSION_TYPE "${TYPE_LINE}")

#message ("major = '${LCLT_VERSION_MAJOR}'") 
#message ("minor = '${LCLT_VERSION_MINOR}'") 
#message ("patch = '${LCLT_VERSION_PATCH}'") 
#message ("type = '${LCLT_VERSION_TYPE}'") 


#build version string for installer name
set (LCLT_VERSION_STRING "0.1")
#set(LCLT_VERSION_STRING "${LCLT_VERSION_MAJOR}.${LCLT_VERSION_MINOR}" )
#if (NOT("${LCLT_VERSION_PATCH}" STREQUAL "0"))
#    set(LCLT_VERSION_STRING "${LCLT_VERSION_STRING}.${LCLT_VERSION_PATCH}" )
#endif()
#if (NOT("${LCLT_VERSION_TYPE}" STREQUAL "' '") AND NOT("${LCLT_VERSION_TYPE}" STREQUAL "\" \""))
#    STRING_UNQUOTE(LTYPE ${LCLT_VERSION_TYPE})
#    set(LCLT_VERSION_STRING "${LCLT_VERSION_STRING}.${LTYPE}" )
#endif()
message ("version string = '${LCLT_VERSION_STRING}'") 


# installation folders
# ------------------------------------------------------------------------------
# You can change the install location by running cmake like this:
#
#   cmake -DCMAKE_INSTALL_PREFIX=/new/install/prefix
#
# By default, the prefix is "/usr/local"
# ------------------------------------------------------------------------------
message("CMAKE_INSTALL_PREFIX = " ${CMAKE_INSTALL_PREFIX} )
set(ROOT_INSTALL ${CMAKE_INSTALL_PREFIX} )

set(ROOT_INSTALL_HOME ${ROOT_INSTALL}/share/${CMAKE_PROJECT_NAME} )
message("ROOT_INSTALL_HOME = " ${ROOT_INSTALL_HOME} )



# set up other variables
#--------------------------------------------------------


# check that compiler supports namespace sdt
include(TestForSTDNamespace)
if(CMAKE_NO_STD_NAMESPACE)
  message(FATAL_ERROR "The compiler doesn't support namespace std.")
endif()

 
# add headers directories from source tree
include_directories(
    ${ROOT_DIR}
)


# Check for needed libraries and set all related includes, flags, etc. 
#-------------------------------------------------------------------------

# Check for wxWidgets
find_package(wxWidgets 3.0 COMPONENTS base core adv aui xml xrc net qa html propgrid stc REQUIRED )
if(wxWidgets_FOUND)
    INCLUDE(${wxWidgets_USE_FILE})
    message(STATUS "wxWidgets include dirs: " ${wxWidgets_INCLUDE_DIRS})
    message(STATUS "wxWidgets libraries: " ${wxWidgets_LIBRARIES})
else(wxWidgets_FOUND)
    message(SEND_ERROR "wxWidgets not found")
endif(wxWidgets_FOUND)


# Check for Lomse
find_package(Lomse 0.16.2 REQUIRED)                    
if( Lomse_FOUND )
    include_directories( ${Lomse_INCLUDE_DIRS} )
    link_libraries( ${Lomse_LIBRARIES} )
    message(STATUS "Lomse found. Version: ${Lomse_VERSION}" )
    message(STATUS "Lomse libraries: ${Lomse_LIBRARIES}" )
    message(STATUS "Lomse include dirs: ${Lomse_INCLUDE_DIRS}" )
    message(STATUS "Lomse definitions: ${Lomse_DEFINITIONS}" )
else()
    message(SEND_ERROR "Lomse package not found.")
endif()



# "Print all warnings", macros for GCC & __UNIX__
if(UNIX)
    add_definitions( -Wall -DGCC -D__UNIX__ )
endif(UNIX)

#include binary dir so that lclt_config.h can be accesed during build
include_directories("${CMAKE_CURRENT_BINARY_DIR}")



#////////////////////////////////////////////////////////////////////////
# Target: lclt program
#////////////////////////////////////////////////////////////////////////

set (LCLT  lclt)

# set name of lclt executable
set( CMAKE_EXECUTABLE_SUFFIX "" )

# source files to compile
set(ALL_SOURCES 
    ${SRC_DIR}/lclt_main.cpp
    ${SRC_DIR}/lclt_command_parser.cpp
    ${SRC_DIR}/lclt_command_processor.cpp
    ${SRC_DIR}/lclt_injectors.cpp
)

#executable
add_executable( ${LCLT} ${ALL_SOURCES} )

# libraries to link
target_link_libraries ( ${LCLT} ${wxWidgets_LIBRARIES} ${Lomse_LIBRARIES} )



#///////////////////////////////////////////////////////////////////////////////
# Program installation
# You can change the install location by running cmake like this:
#
#   cmake -DCMAKE_INSTALL_PREFIX=/new/install/prefix
#
# By default, the prefix is "/usr/local"
#
# Remember:
#  ROOT_DIR: the root of the package source tree: i.e. $HOME/Downloads/lclt-5.3
#  ROOT_INSTALL:  install location: i.e. /usr/local
#  ROOT_INSTALL_HOME: shared files i.e. /usr/local/share/lclt/
#///////////////////////////////////////////////////////////////////////////////

    # lclt program
install( TARGETS ${LCLT} DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" )

    # 1. Software and essentials (ROOT_INSTALL_HOME):
    # ------------------------------------------------------------------------------

    # files in root folder
install(FILES  CHANGELOG.md LICENSE README.md 
        DESTINATION ${ROOT_INSTALL_HOME} )



