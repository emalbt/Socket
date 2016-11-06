cmake_minimum_required (VERSION 2.6)
project(glove_wifi)

include_directories(include)

find_package(Boost 1.6 REQUIRED  )
include_directories(${Boost_INCLUDE_DIR})

add_executable(glove_wifi 
    src/main.cpp 
    src/qbmove_communications.cpp 
    src/imuboard_communications.cpp
    src/wireless_communicator.cpp
) 
target_link_libraries(glove_wifi ${Boost_LIBRARIES} -lboost_system -lpthread)



