set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(JzDnS)


#Server-----------------------------------------------------------------

add_executable(server
        src/server/main.cpp
        src/cryptography.hpp src/cryptography.cpp
        src/BlockingQueue.hpp
        src/server/authorization.hpp src/server/authorization.cpp
        src/server/Riddle.h src/server/Riddle.cpp
        src/server/RiddleBase.h src/server/RiddleBase.cpp
        src/server/User.h src/server/User.cpp
        src/server/UserBase.h src/server/UserBase.cpp
        src/message.h src/message.cpp
        src/server/RiddleService.h src/server/RiddleService.cpp
        src/server/AdminService.h src/server/AdminService.cpp
        src/server/Looper.cpp src/server/Looper.hpp
        src/QueueMap.cpp src/QueueMap.hpp
        src/server/AdminModule.h src/server/AdminModule.cpp
        src/server/RiddleModule.h src/server/RiddleModule.cpp
        src/server/ServerModule.cpp src/server/ServerModule.hpp
        src/Logger.cpp src/Logger.h)

set(LINK_FLAGS "-L /home/aniela/software/cryptopp -l cryptopp -lboost_serialization -lboost_system -pthread ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LINK_FLAGS}")
target_link_libraries(server -lboost_serialization cryptopp -pthread)
#target_link_libraries(jzdns Boost::archive)

#Admin-----------------------------------------------------------------

add_executable(admin_client
        src/admin_client/main.cpp
        src/admin_client/Admin.cpp src/admin_client/Admin.h
        src/admin_client/BlockingQueueAdmin.hpp
        src/admin_client/Problem.cpp src/admin_client/Problem.h
        src/admin_client/serverStructure.cpp src/admin_client/serverStructure.h
        src/cryptography.cpp src/cryptography.hpp
        src/message.cpp src/message.h)

target_link_libraries(admin_client -lboost_serialization cryptopp -pthread)

#Client-----------------------------------------------------------------

add_executable(client
        src/client/client.cpp src/client/client.hpp
        src/client/serverStructure.cpp src/client/serverStructure.h
        src/cryptography.cpp src/cryptography.hpp
        src/message.cpp src/message.h)

target_link_libraries(client -lboost_serialization cryptopp -pthread)

#Unit tests-----------------------------------------------------------------

include(BoostTestHelpers.cmake)

set(DEPS src/cryptography.cpp)
add_boost_test(test/cryptography_test.cpp "${DEPS}")

set(DEPS src/server/authorization.cpp src/server/Looper.cpp src/message.cpp src/server/UserBase.cpp src/server/User.cpp
        src/server/RiddleBase.cpp src/server/Riddle.cpp src/QueueMap.cpp src/server/RiddleService.cpp src/server/AdminService.cpp
        src/cryptography.cpp src/server/RiddleModule.cpp src/server/AdminModule.cpp src/Logger.cpp src/server/ServerModule.cpp)

add_boost_test(test/looper_test.cpp "${DEPS}")

set(DEPS src/server/authorization.cpp src/server/Looper.cpp src/message.cpp src/server/UserBase.cpp src/server/User.cpp
        src/server/RiddleBase.cpp src/server/Riddle.cpp src/QueueMap.cpp src/server/RiddleService.cpp src/server/AdminService.cpp
        src/cryptography.cpp src/server/RiddleModule.cpp src/server/AdminModule.cpp src/Logger.cpp src/server/ServerModule.cpp)

add_boost_test(test/UserBaseTests.cpp "${DEPS}")

set(DEPS src/server/authorization.cpp src/server/Looper.cpp src/message.cpp src/server/UserBase.cpp src/server/User.cpp
        src/server/RiddleBase.cpp src/server/Riddle.cpp src/QueueMap.cpp src/server/RiddleService.cpp src/server/AdminService.cpp
        src/cryptography.cpp src/server/RiddleModule.cpp src/server/AdminModule.cpp src/Logger.cpp src/server/ServerModule.cpp)

add_boost_test(test/riddleDbTests.cpp "${DEPS}")

set(DEPS src/message.cpp src/cryptography.cpp)

add_boost_test(test/serialization_test.cpp "${DEPS}")


enable_testing(true)
