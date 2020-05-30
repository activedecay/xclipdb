cmake_minimum_required(VERSION 3.15)
project(x C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(appindicator REQUIRED IMPORTED_TARGET appindicator3-0.1)

add_executable(x main.c)
