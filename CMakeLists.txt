set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(JzDnS)
include_directories(${CRYPTOPP_INCLUDE_DIRS})
find_package(CryptoPP REQUIRED)
set(LIBS ${LIBS} ${CRYPTOPP_LIBRARIES})


add_executable(jzdns
src/main.cpp
src/cryptography.hpp src/cryptography.cpp
)

target_link_libraries(jzdns ${LIBS})
include(BoostTestHelpers.cmake)

set(DEPS src/cryptography.cpp)
add_boost_test(test/cryptography_test.cpp "${DEPS}")      


enable_testing()


