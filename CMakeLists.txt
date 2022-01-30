cmake_minimum_required(VERSION 3.11)

project(babel)

add_definitions("-fPIC")

set(QMAKE_CXXFLAGS_DEBUG)
set(QMAKE_CXXFLAGS_DEBUG += /MDd)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

IF (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
ELSE()
    message(FATAL_ERROR "No conanbuildinfo.cmake file found")
ENDIF()


set(SRCS_SERVER
    server/main.cpp
    server/Connexion.cpp
    server/Server.cpp
    server/Parser.cpp
    server/Db.cpp
    )

set(HEADERS_SERVER
    server/Connexion.hpp
    server/Server.hpp
    server/Parser.hpp
    server/Db.hpp
    )

set(TS_FILES client/Babel_Client_fr_FR.ts)

set(SRCS_CLIENT
    client/main.cpp
    client/mainwindow.cpp
    client/dialogconnection.cpp
    client/searchfriend.cpp
    client/user.cpp
    client/request.cpp 
    )

set(HEADERS_CLIENT
    client/mainwindow.h
    client/mainwindow.ui
    client/dialogconnection.h
    client/dialogconnection.ui
    client/searchfriend.h
    client/searchfriend.ui
    client/user.h
    client/user.ui
    client/image.qrc
    client/request.h
    client/request.ui
    )

add_executable(babel_server ${SRCS_SERVER} ${HEADERS_SERVER})

add_executable(babel_client ${SRCS_CLIENT} ${HEADERS_CLIENT} ${TS_FILES})

conan_basic_setup(TARGETS)

find_package(Qt5Core CONFIG)
find_package(Qt5 COMPONENTS Widgets CONFIG)
find_package(Qt5Network CONFIG)

target_link_libraries(babel_client PRIVATE CONAN_PKG::portaudio)
target_link_libraries(babel_client PRIVATE CONAN_PKG::opus)
target_link_libraries(babel_client PRIVATE CONAN_PKG::qt)
target_link_libraries(babel_server PRIVATE CONAN_PKG::boost)
target_link_libraries(babel_server PRIVATE CONAN_PKG::sqlite3)

target_link_libraries(babel_client PRIVATE Qt5::Core)
target_link_libraries(babel_client PRIVATE Qt5::Widgets)
target_link_libraries(babel_client PRIVATE Qt5::Network)

include_directories(${CMAKE_INCLUDE_PATH})

IF(MSVC)
ELSE(MSVC)
    target_link_libraries(babel_client PRIVATE ${CMAKE_BINARY_DIR}/alsa/lib/libasound.so)
    target_include_directories(babel_client PRIVATE ${CMAKE_BINARY_DIR}/alsa/include)
ENDIF(MSVC)