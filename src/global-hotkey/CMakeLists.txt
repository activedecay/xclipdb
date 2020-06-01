cmake_minimum_required(VERSION 3.15)
project(global-hotkey C)

find_package(X11)
find_package(PkgConfig REQUIRED)
pkg_check_modules(gtk REQUIRED IMPORTED_TARGET gtk+-3.0)

add_executable(global-hotkey main.c)
target_link_libraries(global-hotkey PRIVATE
        X11::X11
        PkgConfig::gtk)
