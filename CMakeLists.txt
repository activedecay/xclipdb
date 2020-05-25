cmake_minimum_required(VERSION 3.15)
project(src C)
set(CMAKE_C_STANDARD 99)

find_package(SQLite3)
find_package(PkgConfig REQUIRED)
pkg_check_modules(gtk3 REQUIRED IMPORTED_TARGET gtk+-3.0)

include_directories(src)
add_executable(src src/main.c)

target_link_libraries(src PUBLIC
        PkgConfig::gtk3
        SQLite::SQLite3)
