cmake_minimum_required (VERSION 3.20)
project(ArduinoHost)
set(CMAKE_VERBOSE_MAKEFILE OFF)

unset(USE_BOOST_TIMING CACHE)
option(USE_BOOST_TIMING "Use boost::chrono and boost::this_thread for millis and delay instead of std:: library" OFF)

set(CORELIB_NAME "${PROJECT_NAME}-lib")

#message("### root  CORELIB_NAME = ${CORELIB_NAME}")
set(CORELIB_CXX_STANDARD cxx_std_11)

if(USE_BOOST_TIMING)
    message("### Using boost:: for timing")
    find_package(Boost 1.77 REQUIRED COMPONENTS thread chrono)
    add_definitions(-DBOOST_TIMING=1)
else()
    message("### Using std:: for timing")
    add_definitions(-DBOOST_TIMING=0)
endif()

add_subdirectory(api)
add_subdirectory(test)
