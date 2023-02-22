aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} DATA_VIEW_DIALOG)
add_library(dataViewDialog STATIC
            ${DATA_VIEW_DIALOG}
)
target_link_libraries(dataViewDialog PRIVATE Qt6::Widgets Qt6::Sql)
target_include_directories(dataViewDialog INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})