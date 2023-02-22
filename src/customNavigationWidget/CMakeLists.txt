# 库文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} CUSTOM_NAV_WIDGET)
add_library(customNavigationWidget STATIC
            ${CUSTOM_NAV_WIDGET}
)
target_link_libraries(customNavigationWidget PRIVATE Qt6::Widgets)
target_include_directories(customNavigationWidget INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})