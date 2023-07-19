cmake_minimum_required(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
set(PROJECT_NAME XStateReader)
set(CMAKE_LIBRARY_TYPE SHARED)
set(CMAKE_CXX_STANDARD ${CMAKE_LANGUAGE_VERSION})

project(${PROJECT_NAME})

include(src/CMakeLists.txt)


add_definitions(-DXPLM301=1 -DXPLM300=1 -DXPLM210=1 -DXPLM200=1)

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(WIN32)
    add_definitions(-DIBM=1)
    target_link_libraries( xpldp -static -static-libgcc -static-libstdc++ "${PROJECT_SOURCE_DIR}/SDK/Libraries/Win/XPLM_64.lib")
elseif (APPLE)
    add_definitions(-DAPL=1)
else ()
    add_definitions(-DLIN=1)
endif (WIN32)

set_target_properties(xpldp PROPERTIES PREFIX "")
set_target_properties(xpldp PROPERTIES OUTPUT_NAME "BeigeBox")
set_target_properties(xpldp PROPERTIES SUFFIX ".xpl")

if(WIN32)
    install(TARGETS xpldp RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/BeigeBox/win_x64/ )
elseif (APPLE)
    install(TARGETS xpldp RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/BeigeBox/mac_x64/ )
else()
    install(TARGETS xpldp RUNTIME DESTINATION ./BeigeBox/lin_x64/ )
endif (WIN32)
