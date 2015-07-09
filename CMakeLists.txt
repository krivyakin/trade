cmake_minimum_required (VERSION 2.6)

project(trade)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g -O3")

file(GLOB TRADE_SRCS
	main.cpp
	order.cpp
	feed_handler.cpp
	)

add_executable(trade ${TRADE_SRCS})
