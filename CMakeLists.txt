cmake_minimum_required(VERSION 3.15)
project(x C)

add_subdirectory(src/xtclipboard)

find_package(X11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(appindicator REQUIRED IMPORTED_TARGET appindicator3-0.1)
pkg_check_modules(x11xcb REQUIRED IMPORTED_TARGET x11-xcb)
pkg_check_modules(xcbkeys REQUIRED IMPORTED_TARGET xcb-keysyms)
#pkg_check_modules(xaw7 REQUIRED IMPORTED_TARGET xaw7)

add_executable(x  main.c)
target_include_directories(x PRIVATE
        .

        )
target_link_libraries(x PRIVATE
        X11::X11
        X11::Xmu
        PkgConfig::appindicator
        PkgConfig::x11xcb
#        PkgConfig::xaw7
        PkgConfig::xcbkeys
        )
