cmake_minimum_required(VERSION 3.19)
project(JzDnS)

set(CMAKE_CXX_STANDARD 17)

add_executable(JzDnS src/main.cpp
        src/message.h src/message.cpp
        src/authorization.cpp src/authorization.hpp
        src/QueueMap.cpp src/QueueMap.hpp
        src/AdminService.h src/AdminService.cpp
        src/RiddleService.cpp src/RiddleService.h
        src/UserBase.cpp src/UserBase.h
        src/Riddle.h src/Riddle.cpp
        src/RiddleBase.h src/RiddleBase.cpp
        src/User.cpp src/User.h
        src/Looper.hpp src/Looper.cpp
        src/AdminModule.cpp src/AdminModule.h src/RiddleModule.cpp src/RiddleModule.h)

TARGET_LINK_LIBRARIES(JzDnS -lboost_serialization -pthread cryptopp)
