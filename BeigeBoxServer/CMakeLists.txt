set(XPLANE_INCLUDE_DIR "SDK/xplane")

if(WIN32)
    set(CWIC_LIB SDK/cwic/lib/cwic_lib.dll)
else ()
    set(CWIC_LIB SDK/cwic/lib/libcwic_lib.so)
endif (WIN32)

find_package(Boost 1.82.0 REQUIRED)


if(Boost_FOUND)
    message("Boost Directory: ${BOOST_ROOT}")
    include_directories(${Boost_INCLUDE_DIR})
endif ()

if(NOT Boost_FOUND)
    message("Could not find boost at: ${BOOST_INCLUDE_DIR}")
endif ()

project(BBServer)
link_directories(../SDK/Libraries)

include_directories(${XPLANE_INCLUDE_DIR})


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
    message(..)

endif (WIN32)

set_target_properties(${PROJECT_NAME} PROPERTIES
        PREFIX ""
        OUTPUT_NAME "../server")


if(WIN32)
    install(TARGETS ${PROJECT_NAME} DESTINATION ../BeigeBox/win_x64)
else()
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/BeigeBox/lin_x64/" )
endif (WIN32)