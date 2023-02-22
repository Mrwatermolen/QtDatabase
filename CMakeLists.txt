cmake_minimum_required(VERSION 3.5)

project(DatabaseManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Network)

include_directories(${PROJECT_SOURCE_DIR}/include)

# add_subdirectory(src/common)
add_subdirectory(src/app)
add_subdirectory(src/customNavigationWidget)
add_subdirectory(src/customDataTableManager)
# add_subdirectory(src/customNetworkdownloader)
add_subdirectory(src/tableConfigWidget)
add_subdirectory(src/dataViewDialog)
add_subdirectory(src/dataTableFilterWidget)