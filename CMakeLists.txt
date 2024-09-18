cmake_minimum_required(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
set(CMAKE_CXX_STANDARD ${CMAKE_LANGUAGE_VERSION})
set(BOOST_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED FALSE)
set(Boost_DEBUG ON)
project("BeigeBox Suite")

add_subdirectory(BeigeBoxPlugin)
#add_subdirectory(BeigeBoxServer)
