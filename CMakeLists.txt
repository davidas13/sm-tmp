cmake_minimum_required(VERSION 3.22)

project(sm-tmp LANGUAGES C)


if (APPLE)
    set(HOMEBREW_PATH "/opt/homebrew")

    list(APPEND CMAKE_PREFIX_PATH "${HOMEBREW_PATH}")

    if (NOT "$ENV{PKG_CONFIG_PATH}" STREQUAL "")
        list(APPEND ENV{PKG_CONFIG_PATH} "${HOMEBREW_PATH}/lib/pkgconfig:${HOMEBREW_PATH}/share/pkgconfig")
    else()
        set(ENV{PKG_CONFIG_PATH} "${HOMEBREW_PATH}/lib/pkgconfig:${HOMEBREW_PATH}/share/pkgconfig")
    endif()
endif()

find_package(PkgConfig REQUIRED)

pkg_check_modules(MPV REQUIRED IMPORTED_TARGET mpv)

if (NOT MPV_FOUND)
    message(FATAL_ERROR "Could not find mpv")
endif()

add_executable(sm-tmp src/main.c)

target_link_libraries(sm-tmp PRIVATE PkgConfig::MPV)

target_compile_features(sm-tmp PRIVATE c_std_11)

