cmake_minimum_required(VERSION 3.15)
project(xtclipboard C)

find_package(X11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(appindicator REQUIRED IMPORTED_TARGET appindicator3-0.1)
#pkg_check_modules(x11xcb REQUIRED IMPORTED_TARGET x11-xcb)
pkg_check_modules(xcbkeys REQUIRED IMPORTED_TARGET xcb-keysyms)
pkg_check_modules(xaw7 REQUIRED IMPORTED_TARGET xaw7)

add_executable(xtclipboard  main.c)
target_link_libraries(xtclipboard PRIVATE
        Xaw7
        Xmu
        Xt
        X11
        PkgConfig::appindicator
#        PkgConfig::x11xcb
        PkgConfig::xaw7
        PkgConfig::xcbkeys
        )
