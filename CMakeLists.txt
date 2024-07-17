CMAKE_MINIMUM_REQUIRED(VERSION 3.25)

project(GyaanSrot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Network Widgets Sql WebEngineWidgets)

add_subdirectory(src)

add_executable(gyaansrot WIN32 src/main.cpp src/mainapplication.h src/mainapplication.cpp resources/resources.qrc)

target_link_libraries(gyaansrot PRIVATE Qt6::Core Qt6::Network Qt6::Widgets Network Parser Database Util Gui)

