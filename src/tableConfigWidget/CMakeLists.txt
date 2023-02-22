aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} DASHBOARD_WIDGET)
add_library(tableConfigWidget STATIC
            ${DASHBOARD_WIDGET}
)
target_link_libraries(tableConfigWidget PRIVATE Qt6::Widgets)
target_include_directories(tableConfigWidget INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})