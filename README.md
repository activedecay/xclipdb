# distribution
make sure the build contains the app-defaults file
`cp app-defaults/Xclipdb /etc/X11/app-defaults/Xclipdb`

# todo
- handle other targets, but at least images (or image's filesystem paths)
- show a dropdown list of clipboards for easy pickings
- show a persistent list of clips saved to a db
- desktop activation keyboard shortcut `mod4-v` should be amazing
- fix NewCurrentClipContents because XawTextReplace aborts when
  there's too much text, e.g., run this testcase:


    head -c 1M </dev/urandom | base64 -w0 | xclip -selection clipboard

- [answer his question](https://stackoverflow.com/q/61271941/823282)


# x toolkit intrinsics - c api
libXt /usr/share/doc/libxt-dev/intrinsics.html

# Athena widgets
libXaw /usr/share/doc/libxaw7-dev/libXaw.html

# x miscellaneous utility
libXmu /usr/share/doc/libxmu-headers/Xmu.html

# wow, actual spec
Clipboard Manager Specification
https://freedesktop.org/wiki/ClipboardManager/

# notes
x window server
x desktop client
they interact with atoms, i guess
i wanna know when the clipboard changes using push events, not polling events.

# holy grail 2.0
https://xcb.freedesktop.org/tutorial/

# apparently
the way to go is just use gtk+libappindicator or qt4/5+systray
https://github.com/3v1n0/indicators-examples-snaps

# holy grail
https://duckduckgo.com/?t=lm&q=tray+icon+x11&ia=web

# x11 lib doc
https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html
and all doc
https://www.x.org/releases/X11R7.7/doc/index.html

# ICCCM - Inter-Client Communication Conventions Manual
X Consortium Standard
https://www.x.org/releases/X11R7.6/doc/xorg-docs/specs/ICCCM/icccm.html

# GDK Reference Manual 
Selections — Functions for transfering data via the X selection mechanism
https://developer.gimp.org/api/2.0/gdk/gdk-Selections.html

# How to upload 32 bit image to server-side pixmap
https://stackoverflow.com/q/10492275/823282

# Xlib ARGB Window Icon
working! nice!
https://stackoverflow.com/a/15595582/823282

# Extended Window Manager Hints
https://specifications.freedesktop.org/wm-spec/wm-spec-1.3.html#idm45722857106464

# XEmbed Protocol Specification
https://specifications.freedesktop.org/xembed-spec/xembed-spec-latest.html

# System Tray Protocol Specification
https://specifications.freedesktop.org/systemtray-spec/systemtray-spec-0.2.html

# the missing documentation for
xcb_key_press_lookup_keysym
git clone --recursive git://anongit.freedesktop.org/git/xcb/util-keysyms

# notes
/usr/include/X11/X.h:221 ShiftMask ... etc
https://stackoverflow.com/a/29001687/823282
Mask        | Value | Key
------------+-------+------------
ShiftMask   |     1 | Shift
LockMask    |     2 | Caps Lock
ControlMask |     4 | Ctrl
Mod1Mask    |     8 | Alt
Mod2Mask    |    16 | Num Lock
Mod3Mask    |    32 | Scroll Lock
Mod4Mask    |    64 | Windows
Mod5Mask    |   128 | ISO_Level3_Shift
