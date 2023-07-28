set(LIB_FILES "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.cpp" "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.hpp")
set(CMAKE_DEFAULT_PROJECT_FILE "Plugin.cpp")
file(GLOB HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/*.hpp)


add_library(xplplugin SHARED ${CMAKE_CURRENT_LIST_DIR}/Plugin.cpp)

add_library(dp SHARED ${CMAKE_CURRENT_LIST_DIR}/DataProcessor.cpp)


target_sources(xplplugin PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Plugin.cpp
        PUBLIC FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/src/
        FILES ${LIB_FILES})

target_sources(dp PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/DataProcessor.cpp
        PUBLIC FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/src/
        FILES ${LIB_FILES})




