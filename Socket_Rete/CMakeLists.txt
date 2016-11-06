cmake_minimum_required(VERSION 2.6)
project(glove_wifi_pc)

include_directories(include)
find_package(Boost 1.6 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(glove_wifi_pc 	
	src/main.cpp
	src/Network.cpp
	src/Network_B.cpp
)

target_link_libraries(glove_wifi_pc ${Boost_LIBRARIES} -lboost_system -lpthread) 