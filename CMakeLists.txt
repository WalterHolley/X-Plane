cmake_minimum_required(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
set(PROJECT_NAME XStateReader)
set(CMAKE_LIBRARY_TYPE SHARED)
set(CMAKE_CXX_STANDARD ${CMAKE_LANGUAGE_VERSION})
set(SRC_FILES "src/DataProcessor.cpp" "src/DataProcessor.h")


project(${PROJECT_NAME})
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()


add_library(xpldp ${CMAKE_LIBRARY_TYPE} ${CMAKE_DEFAULT_PROJECT_FILE})
target_include_directories(xpldp PRIVATE "${PROJECT_SOURCE_DIR}")
add_subdirectory("src")
install(TARGETS xpldp FILE_SET HEADERS)
