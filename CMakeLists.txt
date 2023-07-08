cmake_minimum_required(VERSION 3.5)

project(nex-system-wallpapermanager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
#find_package(Qt6 REQUIRED COMPONENTS )
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets MultimediaWidgets WebEngineWidgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        Resources/resources.qrc
        desktopcm.cpp
        desktopcm.h
        desktopcm.ui
        settingswindow.h
        settingswindow.cpp
        settingswindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(nex-system-wallpapermanager
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET nex-system-wallpapermanager APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(nex-system-wallpapermanager SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(nex-system-wallpapermanager
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(nex-system-wallpapermanager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets )
target_link_libraries(nex-system-wallpapermanager PRIVATE Qt6::WebEngineWidgets)
target_link_libraries(nex-system-wallpapermanager PRIVATE Qt6::MultimediaWidgets)


set_target_properties(nex-system-wallpapermanager PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS nex-system-wallpapermanager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(nex-system-wallpapermanager)
endif()
