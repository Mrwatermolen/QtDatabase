aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} CUSTOM_NETWORK_DOWNLOADER)
add_library(customNetworkdownloader STATIC
            ${CUSTOM_NETWORK_DOWNLOADER}
)
target_link_libraries(customNetworkdownloader PRIVATE Qt6::Widgets Qt6::Network)
target_include_directories(customNetworkdownloader INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})