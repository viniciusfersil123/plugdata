# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# Try to auto-detect ESP-IDF if IDF_PATH is not set in the environment.
if(NOT DEFINED ENV{IDF_PATH})
	set(_IDF_GUESS1 "$ENV{HOME}/esp/esp-idf")
	set(_IDF_GUESS2 "${CMAKE_SOURCE_DIR}/esp-idf")
	set(_IDF_GUESS3 "${CMAKE_SOURCE_DIR}/../esp-idf")
	foreach(_IDF_DIR IN LISTS _IDF_GUESS1 _IDF_GUESS2 _IDF_GUESS3)
		if(EXISTS "${_IDF_DIR}/tools/cmake/project.cmake")
			set(ENV{IDF_PATH} "${_IDF_DIR}")
			message(STATUS "IDF_PATH not set; using detected path: ${_IDF_DIR}")
			break()
		endif()
	endforeach()
endif()

if(NOT EXISTS "$ENV{IDF_PATH}/tools/cmake/project.cmake")
	message(FATAL_ERROR "ESP-IDF not found. Set IDF_PATH or run: . $HOME/esp/esp-idf/export.sh")
endif()

# Try to locate Xtensa toolchain if compilers are unset or not absolute
set(_NEED_TOOLCHAIN FALSE)
if(NOT CMAKE_C_COMPILER)
	set(_NEED_TOOLCHAIN TRUE)
elseif(NOT IS_ABSOLUTE "${CMAKE_C_COMPILER}")
	set(_NEED_TOOLCHAIN TRUE)
endif()
if(NOT CMAKE_CXX_COMPILER)
	set(_NEED_TOOLCHAIN TRUE)
elseif(NOT IS_ABSOLUTE "${CMAKE_CXX_COMPILER}")
	set(_NEED_TOOLCHAIN TRUE)
endif()

if(_NEED_TOOLCHAIN)
	set(_XTENSA_BASE "$ENV{HOME}/.espressif/tools/xtensa-esp-elf")
	file(GLOB _XTENSA_BIN_DIRS "${_XTENSA_BASE}/*/xtensa-esp32-elf/bin")
	list(LENGTH _XTENSA_BIN_DIRS _XTENSA_BIN_COUNT)
	if(_XTENSA_BIN_COUNT GREATER 0)
		list(GET _XTENSA_BIN_DIRS 0 _XTENSA_BIN)
		set(CMAKE_C_COMPILER "${_XTENSA_BIN}/xtensa-esp32-elf-gcc" CACHE FILEPATH "" FORCE)
		set(CMAKE_CXX_COMPILER "${_XTENSA_BIN}/xtensa-esp32-elf-g++" CACHE FILEPATH "" FORCE)
			# Also export to environment for any sub-processes
			set(ENV{CC} "${_XTENSA_BIN}/xtensa-esp32-elf-gcc")
			set(ENV{CXX} "${_XTENSA_BIN}/xtensa-esp32-elf-g++")
			set(ENV{PATH} "${_XTENSA_BIN}:$ENV{PATH}")
		message(STATUS "Using detected Xtensa toolchain at: ${_XTENSA_BIN}")
	endif()
endif()

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ime-embarcados-lib)

# Speed up builds: include only main + its dependencies
# See ESP-IDF docs: MINIMAL_BUILD property trims components for faster compile/link
idf_build_set_property(MINIMAL_BUILD ON)
