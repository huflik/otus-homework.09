add_library(async_core
    consoleLogger.cpp
    fileLogger.cpp
    parser.cpp
    processor.cpp
    async.cpp
)

target_include_directories(async_core
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_executable(async main.cpp)

target_link_libraries(async PRIVATE
    async_core
)

set_target_properties(async PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

set_target_properties(async PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})