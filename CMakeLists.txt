cmake_minimum_required(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
set(PROJECT_NAME XStateReader)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_TYPE SHARED)
set(CMAKE_CXX_STANDARD ${CMAKE_LANGUAGE_VERSION})
set(BOOST_USE_STATIC_LIBS OFF)
set(BOOST_ROOT "${BOOST_PATH}")
set(Boost_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/SDK/boost/include")
set(XPLANE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/SDK/xplane")

if(WIN32)
    set(CWIC_LIB SDK/cwic/lib/cwic_lib.dll)
else ()
    set(CWIC_LIB SDK/cwic/lib/libcwic_lib.so)
endif (WIN32)

find_package(Boost 1.82.0 REQUIRED)

if(Boost_FOUND)
    message("Boost Directory: ${BOOST_PATH}")
    include_directories(${Boost_INCLUDE_DIR})
endif ()

if(NOT Boost_FOUND)
    message("Could not fine boost at: ${BOOST_INCLUDE_DIR}")
endif ()

project(${PROJECT_NAME})
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/SDK/Libraries)
include_directories(${XPLANE_INCLUDE_DIR})
include_directories(src)
include(src/CMakeLists.txt)


add_definitions(-DXPLM301=1 -DXPLM300=1 -DXPLM210=1 -DXPLM200=1)

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(WIN32)

    add_definitions(-DIBM=1)
    include_directories(${Boost_INCLUDE_DIR})
    target_link_libraries( xplplugin
            -static -static-libgcc -static-libstdc++ "${PROJECT_SOURCE_DIR}/SDK/Libraries/Win/XPLM_64.lib"
            -static -static-libgcc -static-libstdc++ "${PROJECT_SOURCE_DIR}/SDK/boost/lib/libboost_json-vc143-mt-x64-1_82.lib")
elseif (APPLE)
    add_definitions(-DAPL=1)
else ()
    add_definitions(-DLIN=1)
    option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")
    message("Creating build for Linux")
    message(${PROJECT_SOURCE_DIR})
    target_link_libraries( xplplugin PRIVATE "${PROJECT_SOURCE_DIR}/SDK/Libraries/libcwic_lib.so"
            -shared -rdynamic -nodefaultlibs -undefined_warning "${PROJECT_SOURCE_DIR}/SDK/Libraries/libXPLM_64.so")
endif (WIN32)

set_target_properties(xplplugin PROPERTIES OUTPUT_NAME "BeigeBox")
set_target_properties(xplplugin PROPERTIES SUFFIX ".xpl")

if(WIN32)
    install(TARGETS xplplugin RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/BeigeBox/win_x64/ )
else()
    install(TARGETS xplplugin LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/BeigeBox/lin_x64/" )
endif (WIN32)
