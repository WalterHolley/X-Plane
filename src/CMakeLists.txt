set(LIB_FILES "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.cpp" "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.h")
file(GLOB HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/*.hpp)

set(PUBLIC_SOURCE
"${CMAKE_CURRENT_LIST_DIR}/Plugin.cpp"
)

set(PRIVATE_SOURCE
        "../SDK/cwic/include/c_headers.h"
        "${CMAKE_CURRENT_LIST_DIR}/DataUtil.h"
        "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.h"
        "${CMAKE_CURRENT_LIST_DIR}/UDPClient.h"
        "${CMAKE_CURRENT_LIST_DIR}/UDPClient.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/DataProcessor.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/DataUtil.cpp"
)


add_library(xplplugin SHARED ${PUBLIC_SOURCE} ${PRIVATE_SOURCE})



target_sources(xplplugin PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Plugin.cpp
        ${CMAKE_CURRENT_LIST_DIR}/DataProcessor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/DataUtil.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UDPClient.cpp
)







