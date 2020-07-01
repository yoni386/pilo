##cmake_minimum_required(VERSION 3.7)
##project(pilo)
##
#if(POLICY CMP0042)
#    cmake_policy(SET CMP0042 NEW) # MACOSX_RPATH
#    set(CMAKE_MACOSX_RPATH TRUE)
#endif()
##
##message(STATUS "Looking for CURL")
##include(FindCURL)
#find_package(CURL REQUIRED)
#if(CURL_FOUND)
#    message(STATUS "Found CURL version: ${CURL_VERSION_STRING}")
#    message(STATUS "Using CURL include dir(s): ${CURL_INCLUDE_DIRS}")
#    message(STATUS "Using CURL lib(s): ${CURL_LIBRARIES}")
#else()
#    message(FATAL_ERROR "Could not find CURL")
#endif()
##
### All following targets should search these directories for headers
#include_directories(
#        ${CMAKE_CURRENT_SOURCE_DIR}/include/
#####        ${CURL_INCLUDE_DIRS}
#)
##
##set(CMAKE_CXX_STANDARD 17)
##
##
##set(SOURCE_FILES main.cpp json.hpp ILO.cpp ILO.h DELL.cpp DELL.h HP.cpp HP.h)
##add_executable(pilo ${SOURCE_FILES})
###
##add_subdirectory(include/cpr)
###include_directories(${CPR_INCLUDE_DIRS})
###target_link_libraries(pilo ${CPR_LIBRARIES})
##
###add_subdirectory(cpr)
##
##include_directories(${CPR_INCLUDE_DIRS})
##target_link_libraries(mem1 curl)
##target_link_libraries(pilo ${CPR_LIBRARIES})
##
##
#
#
#cmake_minimum_required(VERSION 2.8.7)
#project(pilo)
#
##if(NOT ${CMAKE_VERSION} LESS 3.2)
##    set(CMAKE_CXX_STANDARD 11)
##    set(CMAKE_CXX_STANDARD_REQUIRED ON)
##else()
##    message(STATUS "Checking compiler flags for C++11 support.")
##    # Set C++11 support flags for various compilers
##    include(CheckCXXCompilerFlag)
##    check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
##    check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
##    if(COMPILER_SUPPORTS_CXX11)
##        message(STATUS "C++11 is supported.")
##        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
##            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
##        else()
##            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
##        endif()
##    elseif(COMPILER_SUPPORTS_CXX0X)
##        message(STATUS "C++0x is supported.")
##        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
##            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++")
##        else()
##            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
##        endif()
##    else()
##        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
##    endif()
##endif()
#
#set(CMAKE_CXX_STANDARD 17)
#
#
##add_subdirectory(include/args)
#
#add_executable(pilo main.cpp json.hpp ILO.cpp ILO.h DELL.cpp DELL.h HP.cpp HP.h)
#target_link_libraries(pilo ${CPR_LIBRARIES})
#include_directories(${CPR_INCLUDE_DIRS})
#
##set(JSON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/json/src PARENT_SCOPE)
#
##if(WIN32)
##    option(CMAKE_USE_OPENSSL "Use OpenSSL code. Experimental" OFF)
##endif()
#
#set(BUILD_CPR_TESTS OFF CACHE BOOL "Don't build cpr tests")
#find_package(CURL)
#if(CURL_FOUND)
#    set(USE_SYSTEM_CURL ON CACHE BOOL "Use the system curl for faster builds")
#endif()
#
#
#add_subdirectory(include/cpr)
#
##add_executable(example example.cpp)
##target_link_libraries(example ${CPR_LIBRARIES})
##include_directories(${CPR_INCLUDE_DIRS} ${JSON_INCLUDE_DIRS})
#include_directories(${CPR_INCLUDE_DIRS})



cmake_minimum_required(VERSION 3.7)
project(pilo)

#if(NOT ${CMAKE_VERSION} LESS 3.2)
#    set(CMAKE_CXX_STANDARD 11)
#    set(CMAKE_CXX_STANDARD_REQUIRED ON)
#else()
#    message(STATUS "Checking compiler flags for C++11 support.")
#    # Set C++11 support flags for various compilers
#    include(CheckCXXCompilerFlag)
#    check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
#    check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
#    if(COMPILER_SUPPORTS_CXX11)
#        message(STATUS "C++11 is supported.")
#        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
#        else()
#            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#        endif()
#    elseif(COMPILER_SUPPORTS_CXX0X)
#        message(STATUS "C++0x is supported.")
#        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++")
#        else()
#            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#        endif()
#    else()
#        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
#    endif()
#endif()

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(include/libcurlCPR)

#add_executable(pilo example.cpp json.hpp)
add_executable(pilo main.cpp json.hpp ILO.cpp ILO.h DELL.cpp DELL.h HP.cpp HP.h)

target_link_libraries(pilo ${CPR_LIBRARIES})
include_directories(
        ${CPR_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
)
#set(EXECUTABLE_OUTPUT_PATH "bin")