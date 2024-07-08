set(LIB_FILES "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.cpp" "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.h")
file(GLOB HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/*.hpp)

set(PUBLIC_SOURCE
        "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
        )
get_filename_component(PREVIOUS_DIR . ABSOLUTE)



add_executable(bbserver)
target_sources(bbserver PUBLIC "${CMAKE_CURRENT_LIST_DIR}/main.cpp")










