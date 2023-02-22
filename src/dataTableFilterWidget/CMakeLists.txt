aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} DATA_TABLE_FILTER_WIDGET)
add_library(dataTableFilterWidget STATIC
            ${DATA_TABLE_FILTER_WIDGET}
)
target_link_libraries(dataTableFilterWidget PRIVATE Qt6::Widgets Qt6::Sql)
target_include_directories(dataTableFilterWidget INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})