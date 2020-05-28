cmake_minimum_required(VERSION 3.15)
project(clipboard-manager C)
set(CMAKE_C_STANDARD 99)

find_package(SQLite3) # cmake --help-module FindSQLite3
find_package(X11) # cmake --help-module FindX11
find_package(PkgConfig REQUIRED)
pkg_check_modules(appindicator REQUIRED IMPORTED_TARGET appindicator3-0.1)
#pkg_check_modules(gtk3 REQUIRED IMPORTED_TARGET gtk+-3.0)

#message(X11_FOUND ${X11_FOUND}) # - True if X11 is available
#message(X11_INCLUDE_DIR ${X11_INCLUDE_DIR}) # - include directories to use X11
#message(X11_LIBRARIES ${X11_LIBRARIES}) # - link against these to use X11

include_directories(
        ./src)
add_executable(clipboard-manager src/main.c)
target_include_directories(clipboard-manager PUBLIC
        /usr/include/X11/
        /usr/include/X11/ICE/
        /usr/include/X11/SM/
        /usr/include/X11/Xaw/
        /usr/include/X11/Xmu/)
target_link_libraries(clipboard-manager PUBLIC
        -lXt -lX11 -lXaw7 -lXmu -lXt -lX11
#        X11::X11
#        PkgConfig::gtk3
        PkgConfig::appindicator
#        SQLite::SQLite3
        )
