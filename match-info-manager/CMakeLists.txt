project(match-info-manager)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)


add_definitions(-DLIBOBS_EXPORTS -DMATCH_IM_EXPORT)

find_package(Qt5Core REQUIRED)
if(NOT Qt5Core_FOUND)
    if (ENABLE_UI)
        message(FATAL_ERROR "Failed to find Qt5 core")
    else()
        message(STATUS "Qt5 core not found")
        return()
    endif()
endif()

set(match_info_manager_SOURCES
        streammatchinfomanager.cpp
        )

set(match_info_manager_HEADERS
        streammatchinfomanager.h
        )

add_library(match-info-manager SHARED
        ${match_info_manager_SOURCES}
        ${match_info_manager_HEADERS}
        )

target_link_libraries(match-info-manager
        match-connection
        Qt5::Core
        )

install_obs_core(match-info-manager)