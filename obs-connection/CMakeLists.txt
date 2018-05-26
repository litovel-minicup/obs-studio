project(obs-connection)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)


add_definitions(-DLIBOBS_EXPORTS)

find_package(Qt5WebSockets REQUIRED)
if(NOT Qt5WebSockets_FOUND)
    if (ENABLE_UI)
        message(FATAL_ERROR "Failed to find Qt5 websockets")
    else()
        message(STATUS "Qt5 sockets not found")
        return()
    endif()
endif()

find_package(Qt5Core REQUIRED)
if(NOT Qt5Core_FOUND)
    if (ENABLE_UI)
        message(FATAL_ERROR "Failed to find Qt5 core")
    else()
        message(STATUS "Qt5 core not found")
        return()
    endif()
endif()

set(obs_connection_SOURCES
    reconnectingsocket.cpp
)

set(obs_connection_HEADERS
    reconnectingsocket.h
)

add_library(obs-connection SHARED
    ${obs_connection_SOURCES}
    ${obs_connection_HEADERS}
)

target_link_libraries(obs-connection
    Qt5::Core
    Qt5::WebSockets
)

install_obs_core(obs-connection)