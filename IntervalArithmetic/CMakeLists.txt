cmake_minimum_required(VERSION 2.8.4)
project(interval)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g -DNDEBUG=1 -Wall -Wextra -pedantic")

set(
	SOURCE_FILES example.cpp
	IntervalArithmetic.cpp
	Interval.cpp
	)
add_executable(interval ${SOURCE_FILES})

target_link_libraries(interval mpfr)

include_directories(
	./
	)