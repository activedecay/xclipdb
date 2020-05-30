cmake_minimum_required(VERSION 3.15)
project(indicator C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(appindicator REQUIRED IMPORTED_TARGET appindicator3-0.1)
pkg_check_modules(gtk3 REQUIRED IMPORTED_TARGET gtk+-3.0)

add_executable(indicator main.c)
target_link_libraries(indicator PRIVATE
        PkgConfig::appindicator)
