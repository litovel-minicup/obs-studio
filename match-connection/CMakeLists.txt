project(match-connection)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)


add_definitions(-DLIBOBS_EXPORTS -DMATCH_CONN_EXPORT)

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

set(match_connection_SOURCES
    reconnectingsocket.cpp
)

set(match_connection_HEADERS
    matchconnectionglobal.h
    reconnectingsocket.h
)

add_library(match-connection SHARED
    ${match_connection_SOURCES}
    ${match_connection_HEADERS}
)

target_link_libraries(match-connection
    Qt5::Core
    Qt5::WebSockets
)

install_obs_core(match-connection)