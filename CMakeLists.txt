# Check if cmake has the required version
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

# Set name of our project to "unpacking-mice".
project(unpacking-mice)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
#message(headers ${headers})
#message(install ${CMAKE_INSTALL_PREFIX})
INSTALL_FILES(/include/unpacking/ FILES ${headers})

#INCLUDE(${CMAKE_BINARY_DIR}/FindROOT.cmake)

set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Dlong32=int -DdatePointer=long")

if (STEPIV_DATA)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_STEPIV_DATA=1")
endif (STEPIV_DATA)

INCLUDE_DIRECTORIES(. src)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib)

if    (ROOT_FOUND)
  INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIR})
  LINK_DIRECTORIES   (${ROOT_LIBRARY_DIR})
endif (ROOT_FOUND)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/app)

