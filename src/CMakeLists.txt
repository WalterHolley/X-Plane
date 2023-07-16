set(LIB_FILES "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.cpp" "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.h")
set(CMAKE_DEFAULT_PROJECT_FILE "DataPlugin.cpp")

add_library(xpldp SHARED ${CMAKE_CURRENT_LIST_DIR}/DataPlugin.cpp)

target_sources(xpldp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/DataPlugin.cpp
        PUBLIC FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/src/
        FILES ${LIB_FILES})






