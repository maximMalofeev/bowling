cmake_minimum_required(VERSION 3.0)

project(bowling LANGUAGES CXX)

option(cmd "cmd application" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(cmd)
    message("cmd")

    set(src "src/main_cmd.cpp"
        "src/input.h" "src/input.cpp"
        "src/game_backend.h" "src/game_backend.cpp")

    add_executable(bowling ${src})

    add_subdirectory(test_cmd)
else()
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    find_package(Qt5 COMPONENTS Core Quick Test REQUIRED)

    set(src "src/main.cpp"
        "src/input.h" "src/input.cpp"
        "src/game.h" "src/game.cpp")

    if(ANDROID)
        add_library(bowling SHARED ${src} "qml/qml.qrc")
    else()
        add_executable(bowling ${src} "qml/qml.qrc")
    endif()

    target_compile_definitions(bowling
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
    target_link_libraries(bowling
        PRIVATE Qt5::Core Qt5::Quick)

    # QtCreator supports the following variables for Android, which are identical to qmake Android variables.
    # Check http://doc.qt.io/qt-5/deployment-android.html for more information.
    # These variables must use CACHE, otherwise QtCreator won't see them.

    #if(ANDROID)
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android" CACHE INTERNAL "")
    #    if (ANDROID_ABI STREQUAL "armeabi-v7a")
    #        set(ANDROID_EXTRA_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so CACHE INTERNAL "")
    #    endif()
    #endif()

    add_subdirectory(test)
endif()

