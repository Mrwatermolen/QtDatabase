aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

qt_add_executable(untitled
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(untitled PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Sql customNavigationWidget customDataTableManager tableConfigWidget dataTableFilterWidget dataViewDialog)

set_target_properties(untitled PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(untitled)
endif()