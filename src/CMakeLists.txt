set(LIB_FILES "DataProcessor.cpp" "DataProcessor.h")
target_sources(xpldp PRIVATE "DataPlugin.cpp"
        PUBLIC FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}
        FILES ${LIB_FILES})