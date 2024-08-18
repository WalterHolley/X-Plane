cmake_minimum_required(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
set(CMAKE_LIBRARY_TYPE SHARED)
set(BOOST_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED FALSE)
set(Boost_DEBUG ON)
set(XPLANE_INCLUDE_DIR "../SDK/xplane")
get_filename_component(PROJECT_ROOT .. ABSOLUTE)
project(BBPlugin)

find_package(Boost 1.82.0 REQUIRED)


if(Boost_FOUND)
    message("Boost Directory: ${BOOST_ROOT}")
    include_directories(${Boost_INCLUDE_DIR})
endif ()

if(NOT Boost_FOUND)
    message("Could not find boost at: ${BOOST_INCLUDE_DIR}")
endif ()


link_directories(../SDK/Libraries)

include_directories(${XPLANE_INCLUDE_DIR})
include_directories(src/include)
include_directories(../SDK/cwic/include)
include_directories(../SDK/sqlite)

file(GLOB HEADERS
        src/include/*.hpp)

set(PUBLIC_SOURCE
        "src/Plugin.cpp"
        )

set(PRIVATE_SOURCE
        "${PROJECT_ROOT}/SDK/cwic/include/c_headers.h"
        "src/include/DataUtil.h"
        "src/include/DataProcessor.h"
        "src/include/UDPClient.h"
        "src/include/Logger.h"
        "src/UDPClient.cpp"
        "src/DataUtil.cpp"
        "src/DataProcessor.cpp"
        "src/Logger.cpp"
        src/include/BBTypes.h src/Recorder.cpp src/include/Recorder.h src/PluginMenu.cpp src/include/PluginMenu.h)

add_library(${PROJECT_NAME} SHARED ${PUBLIC_SOURCE} ${PRIVATE_SOURCE})



#add_definitions(-DXPLM400=1 -DXPLM303=1 -DXPLM301=1 -DXPLM300=1 -DXPLM210=1 -DXPLM200=1)

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()


if(WIN32)
    message("Building for windows")
    add_definitions(-DIBM=1 -DBOOST_ALL_DYN_LINK=1)
    option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
    target_link_libraries( ${PROJECT_NAME}
            -static -static-libgcc -static-libstdc++ ${PROJECT_ROOT}/SDK/Libraries/Win/XPLM_64.lib
            -static -static-libgcc -static-libstdc++ ${PROJECT_ROOT}/SDK/Libraries/Win/XPWidgets_64.lib
            -static -static-libgcc -static-libstdc++ ${PROJECT_ROOT}/SDK/cwic/lib/cwic_lib.dll
            "ws2_32")
elseif (APPLE)
    add_definitions(-DAPL=1)
else ()
    add_definitions(-DLIN=1)
    message("Creating build for Linux")
    message(..)
    target_link_libraries( ${PROJECT_NAME} PRIVATE "${PROJECT_ROOT}/SDK/Libraries/libcwic_lib.so"
            -shared -rdynamic -nodefaultlibs -undefined_warning "${PROJECT_SOURCE_DIR}/SDK/Libraries/libXPLM_64.so")
endif (WIN32)

set_target_properties(${PROJECT_NAME} PROPERTIES
        PREFIX ""
        OUTPUT_NAME "BeigeBox"
        SUFFIX ".xpl")


if(WIN32)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${PROJECT_ROOT}/BeigeBox/win_x64)
else()
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/BeigeBox/lin_x64/" )
endif (WIN32)
