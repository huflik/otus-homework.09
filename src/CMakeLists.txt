set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include)

# Исходники лежат в CMAKE_CURRENT_SOURCE_DIR (src/)
add_library(async_lib SHARED
    consoleLogger.cpp
    fileLogger.cpp
    parser.cpp
    processor.cpp
    async.cpp
)

# Устанавливаем имя библиотеки и выходную директорию
set_target_properties(async_lib PROPERTIES 
    OUTPUT_NAME "async"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Подключаем директории с заголовками для компиляции
target_include_directories(async_lib
    PUBLIC 
        ${HEADER_DIR}  # Публичные заголовки доступны пользователям
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}  # Приватные заголовки (если есть)
)

# Исполняемый файл для демонстрации
add_executable(async_cli main.cpp)

# Линкуем с библиотекой
target_link_libraries(async_cli PRIVATE
    async_lib
)

# Подключаем заголовочные файлы для исполняемого файла
target_include_directories(async_cli
    PRIVATE 
        ${HEADER_DIR}  # Публичные заголовки из include/
)

# Устанавливаем выходную директорию для исполняемого файла
set_target_properties(async_cli PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Установка библиотеки
install(TARGETS async_lib
    LIBRARY DESTINATION lib
)

# Установка публичных заголовков
install(DIRECTORY ${HEADER_DIR}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Установка исполняемого файла
install(TARGETS async_cli
    RUNTIME DESTINATION bin
)

# Для удобства разработки - копируем библиотеку в корень build
add_custom_command(TARGET async_lib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/libasync.so
        ${CMAKE_BINARY_DIR}/
    COMMENT "Copying libasync.so to build directory"
    DEPENDS async_lib
)