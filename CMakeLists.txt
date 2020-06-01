cmake_minimum_required(VERSION 3.15)
project(xclipdb C)

add_subdirectory(src/global-hotkey)

find_package(PkgConfig REQUIRED)
pkg_check_modules(appindicator REQUIRED IMPORTED_TARGET appindicator3-0.1)

add_executable(xclipdb main.c)
target_link_libraries(xclipdb PRIVATE
        Xaw7
        Xmu
        Xt
        X11
        PkgConfig::appindicator
        )
