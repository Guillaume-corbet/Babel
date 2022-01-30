cmake_minimum_required(VERSION 3.5)

project(Babel_Client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Network Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network Widgets LinguistTools REQUIRED)

set(TS_FILES Babel_Client_fr_FR.ts)

if(ANDROID)
  add_library(Babel_Client SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    dialogconnection.cpp
    dialogconnection.h
    dialogconnection.ui
    searchfriend.cpp
    searchfriend.h
    searchfriend.ui
    user.cpp
    user.h
    user.ui
    image.qrc
    request.cpp
    request.h
    request.ui
    call.cpp
    call.h
    call.ui
    ${TS_FILES}
  )
else()
  add_executable(Babel_Client
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    dialogconnection.cpp
    dialogconnection.h
    dialogconnection.ui
    searchfriend.cpp
    searchfriend.h
    searchfriend.ui
    user.cpp
    user.h
    user.ui
    image.qrc
    request.cpp
    request.h
    request.ui
    call.cpp
    call.h
    call.ui
    ${TS_FILES}
  )
endif()

target_link_libraries(Babel_Client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
