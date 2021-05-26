cmake_minimum_required(VERSION 3.19)
project(JzDnS)

set(CMAKE_CXX_STANDARD 14)

add_executable(JzDnS src/main.cpp
        src/UserBase.cpp src/UserBase.h
        src/User.cpp src/User.h test/UserBaseTests.cpp src/RiddleService.cpp src/RiddleService.h)

TARGET_LINK_LIBRARIES(JzDnS -lboost_serialization cryptopp)
