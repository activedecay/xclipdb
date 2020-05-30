x window server
x desktop client
they interact with atoms, i guess
i wanna know when the clipboard changes using push events, not polling events.

# holy grail 2.0
https://xcb.freedesktop.org/tutorial/

# apparently
the way to go is just use gtk+libappindicator or qt4/5+systray
https://github.com/3v1n0/indicators-examples-snaps

# todo answer his question
https://stackoverflow.com/questions/61271941/create-a-system-tray-icon-in-linux-without-gtk-qt

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

/*  Use of the 'col' parameter:

A list of KeySyms is associated with each KeyCode. The list is intended
to convey the set of symbols on the corresponding key. If the list
(ignoring trailing NoSymbol entries) is a single KeySym `K`, then the
list is treated as if it were the list `K NoSymbol K NoSymbol`. If the
list (ignoring trailing NoSymbol entries) is a pair of KeySyms `K1
K2`, then the list is treated as if it were the list `K1 K2 K1 K2`.
If the list (ignoring trailing NoSymbol entries) is a triple of KeySyms
`K1 K2 K3`, then the list is treated as if it were the list `K1 K2 K3
NoSymbol`. When an explicit `void` element is desired in the list,
the value VoidSymbol can be used.

The first four elements of the list are split into two groups of
KeySyms. Group 1 contains the first and second KeySyms; Group 2 contains
the third and fourth KeySyms. Within each group, if the second element
of the group is NoSymbol , then the group should be treated as if the
second element were the same as the first element, except when the first
element is an alphabetic KeySym `K` for which both lowercase and
uppercase forms are defined. In that case, the group should be treated
as if the first element were the lowercase form of `K` and the second
element were the uppercase form of `K.`

The standard rules for obtaining a KeySym from a KeyPress event make use
of only the Group 1 and Group 2 KeySyms; no interpretation of other
KeySyms in the list is given. Which group to use is determined by the
modifier state. Switching between groups is controlled by the KeySym
named MODE SWITCH, by attaching that KeySym to some KeyCode and
attaching that KeyCode to any one of the modifiers Mod1 through Mod5.
This modifier is called the group modifier. For any KeyCode, Group 1 is
used when the group modifier is off, and Group 2 is used when the group
modifier is on.

The Lock modifier is interpreted as CapsLock when the KeySym named
XK_Caps_Lock is attached to some KeyCode and that KeyCode is attached to
the Lock modifier. The Lock modifier is interpreted as ShiftLock when
the KeySym named XK_Shift_Lock is attached to some KeyCode and that
KeyCode is attached to the Lock modifier. If the Lock modifier could be
interpreted as both CapsLock and ShiftLock, the CapsLock interpretation
is used.

The operation of keypad keys is controlled by the KeySym named
XK_Num_Lock, by attaching that KeySym to some KeyCode and attaching that
KeyCode to any one of the modifiers Mod1 through Mod5 . This modifier is
called the numlock modifier. The standard KeySyms with the prefix
`XK_KP_` in their name are called keypad KeySyms; these are KeySyms
with numeric value in the hexadecimal range 0xFF80 to 0xFFBD inclusive.
In addition, vendor-specific KeySyms in the hexadecimal range 0x11000000
to 0x1100FFFF are also keypad KeySyms.

Within a group, the choice of KeySym is determined by applying the first
rule that is satisfied from the following list:

* The numlock modifier is on and the second KeySym is a keypad KeySym. In
  this case, if the Shift modifier is on, or if the Lock modifier is on
  and is interpreted as ShiftLock, then the first KeySym is used,
  otherwise the second KeySym is used.

* The Shift and Lock modifiers are both off. In this case, the first
  KeySym is used.

* The Shift modifier is off, and the Lock modifier is on and is
  interpreted as CapsLock. In this case, the first KeySym is used, but
  if that KeySym is lowercase alphabetic, then the corresponding
  uppercase KeySym is used instead.

* The Shift modifier is on, and the Lock modifier is on and is
  interpreted as CapsLock. In this case, the second KeySym is used, but
  if that KeySym is lowercase alphabetic, then the corresponding
  uppercase KeySym is used instead.

* The Shift modifier is on, or the Lock modifier is on and is
  interpreted as ShiftLock, or both. In this case, the second KeySym is
  used.

*/