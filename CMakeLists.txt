cmake_minimum_required(VERSION 3.19)
project(JzDnS)

set(CMAKE_CXX_STANDARD 14)

add_executable(JzDnS src/main.cpp
        src/UserBase.cpp src/UserBase.h
        src/Riddle.h src/Riddle.cpp
        src/RiddleBase.h src/RiddleBase.cpp
        src/User.cpp src/User.h
        test/UserBaseTests.cpp)

TARGET_LINK_LIBRARIES(JzDnS -lboost_serialization cryptopp)
