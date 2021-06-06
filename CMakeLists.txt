set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(JzDnS)
#include_directories(/home/aniela/software/cryptopp/)
#include_directories(boost/archive)
#include_directories(boost/serialization)

#find_package(Boost REQUIRED COMPONENTS archive)

add_executable(server
        server.cpp
        src/cryptography.hpp src/cryptography.cpp
        src/BlockingQueue.hpp
        src/authorization.hpp src/authorization.cpp
        src/Riddle.h src/Riddle.cpp
        src/RiddleBase.h src/RiddleBase.cpp
        src/User.h src/User.cpp
        src/UserBase.h src/UserBase.cpp
        src/message.h src/message.cpp
        src/RiddleService.h src/RiddleService.cpp
        src/AdminService.h src/AdminService.cpp
        src/Looper.cpp src/Looper.hpp
        src/QueueMap.cpp src/QueueMap.hpp
        src/AdminModule.h src/AdminModule.cpp
        src/RiddleModule.h src/RiddleModule.cpp)

set(LINK_FLAGS "-L /home/aniela/software/cryptopp -l cryptopp -lboost_serialization -lboost_system -pthread ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LINK_FLAGS}")
target_link_libraries(server -lboost_serialization cryptopp -pthread)
#target_link_libraries(jzdns Boost::archive)

include(BoostTestHelpers.cmake)

set(DEPS src/cryptography.cpp)
add_boost_test(test/cryptography_test.cpp "${DEPS}")

set(DEPS src/authorization.cpp src/Looper.cpp src/message.cpp src/UserBase.cpp src/User.cpp
        src/RiddleBase.cpp src/Riddle.cpp src/QueueMap.cpp src/RiddleService.cpp src/AdminService.cpp
        src/cryptography.cpp src/RiddleModule.cpp src/AdminModule.cpp)
add_boost_test(test/looper_test.cpp "${DEPS}")


enable_testing(true)
