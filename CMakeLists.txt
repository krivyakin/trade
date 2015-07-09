cmake_minimum_required (VERSION 2.6)

project(trade)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g -O3")
if(DISABLE_OUT)
	set(CMAKE_CXX_FLAGS "-DDISABLE_OUT=1 ${CMAKE_CXX_FLAGS}")
endif()

file(GLOB TRADE_SRCS
	main.cpp
	order.cpp
	feed_handler.cpp
	)

add_executable(trade ${TRADE_SRCS})
