set(LIB_FILES "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.cpp" "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.hpp")
file(GLOB HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/*.hpp)


add_library(xplplugin SHARED ${CMAKE_CURRENT_LIST_DIR}/Plugin.cpp)




target_sources(xplplugin PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Plugin.cpp
        ${CMAKE_CURRENT_LIST_DIR}/DataProcessor.cpp
)







