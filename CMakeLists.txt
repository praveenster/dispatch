cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__LEPFILE__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

project (dispatch)

add_definitions(-D__ENABLE_LOGGER__)
add_subdirectory (src)
