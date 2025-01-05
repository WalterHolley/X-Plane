cmake_minimum_required(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
get_filename_component(PROJECT_ROOT .. ABSOLUTE)
find_package(Boost 1.82.0 REQUIRED)


if(Boost_FOUND)
    message("Boost Directory: ${BOOST_ROOT}")
    include_directories(${Boost_INCLUDE_DIR})
endif ()

if(NOT Boost_FOUND)
    message("Could not find boost at: ${BOOST_INCLUDE_DIR}")
endif ()

project(BBServer)

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_executable(${PROJECT_NAME} src/main.cpp)

if(WIN32)
    message("Building for windows")

elseif (APPLE)
    message("Build for apple")
else ()
    message("Creating build for Linux")

endif (WIN32)

set_target_properties(${PROJECT_NAME} PROPERTIES
        PREFIX ""
        OUTPUT_NAME "../server")


if(WIN32)
    install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_ROOT}/BeigeBox/win_x64)
else()
    install(TARGETS ${PROJECT_NAME} DESTINATION "${PROJECT_SOURCE_DIR}/BeigeBox/lin_x64/" )
endif (WIN32)