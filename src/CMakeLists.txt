add_library(async_lib SHARED
    consoleLogger.cpp
    fileLogger.cpp
    parser.cpp
    processor.cpp
    async.cpp
)

set_target_properties(async_lib PROPERTIES 
    OUTPUT_NAME "async"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(async_lib
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include 
)

add_executable(async_cli main.cpp)

target_link_libraries(async_cli PRIVATE
    async_lib
)

target_include_directories(async_cli
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include  
)

set_target_properties(async_cli PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

install(TARGETS async_lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

install(TARGETS async_cli
    RUNTIME DESTINATION bin
)

add_custom_command(TARGET async_lib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/libasync.so
        ${CMAKE_BINARY_DIR}/
    DEPENDS async_lib
)