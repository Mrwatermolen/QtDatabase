aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} CUSTOM_DATA_TABLE_MANAGER_WIDGET)
add_library(customDataTableManager STATIC
            ${CUSTOM_DATA_TABLE_MANAGER_WIDGET}
)
target_link_libraries(customDataTableManager PRIVATE Qt6::Widgets Qt${QT_VERSION_MAJOR}::Sql)
target_include_directories(customDataTableManager INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})