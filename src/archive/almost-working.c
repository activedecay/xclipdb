#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <Xlib.h>
#include "libappindicator/app-indicator.h"
#include "libdbusmenu-glib/server.h"
#include "libdbusmenu-glib/menuitem.h"

GMainLoop *mainloop = NULL;
static GtkWidget *local_icon_menu = NULL;
static GtkWidget *local_theme_menu = NULL;
static gboolean active = TRUE;
static gboolean changed_active = FALSE;
static gboolean can_haz_label = TRUE;

#define DEFAULT_ICON "tray-online"
#define DEFAULT_ACTIVE_ICON "tray-new-im"

#define LOCAL_ICON ICON_PATH G_DIR_SEPARATOR_S "simple-client-test-icon.png"
#define LOCAL_ACTIVE_ICON PREFIX G_DIR_SEPARATOR_S ICON_PATH G_DIR_SEPARATOR_S \
                          "simple-client-test-icon-active.png"


unsigned long buffer[] = {
    16, 16,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 338034905,
    3657433343, 0, 184483840, 234881279, 3053453567, 3221225727, 1879048447, 0, 0, 0, 0, 0, 0, 0, 1224737023,
    3305111807, 3875537151, 0, 0, 2063597823, 1291845887, 0, 67109119, 4294901760, 4294901760, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 50266112, 3422552319, 0, 0, 3070230783, 2063597823, 2986344703,
    771752191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3422552319, 0, 0, 3372220671, 1509949695, 704643327, 3355443455, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 0, 3422552319, 0, 134152192,
    3187671295, 251658495, 0, 3439329535, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3422552319, 0, 0, 2332033279, 1342177535,
    167772415, 3338666239, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760,
    4294901760, 0, 3422552319, 0, 0, 436207871, 3322085628, 3456106751, 1375731967, 4278255360, 4026597120,
    3758161664, 3489726208, 3204513536, 2952855296, 2684419840, 2399207168, 2130771712, 1845559040, 1593900800,
    1308688128, 1040252672, 755040000, 486604544, 234946304, 4278255360, 4043374336, 3774938880, 3506503424,
    3221290752, 2952855296, 2667642624, 2399207168, 2130771712, 1862336256, 1627453957, 1359017481, 1073805064,
    788591627, 503379721, 218169088, 4278255360, 4043374336, 3758161664, 3506503424, 3221290752, 2952855296,
    2684419840, 2415984384, 2130771712, 1862336256, 1577123584, 1308688128, 1040252672, 755040000, 486604544,
    218169088, 4278190335, 4026532095, 3758096639, 3489661183, 3221225727, 2952790271, 2667577599, 2415919359,
    2130706687, 1862271231, 1593835775, 1325400319, 1056964863, 771752191, 520093951, 234881279, 4278190335,
    4026532095, 3758096639, 3489661183, 3221225727, 2952790271, 2667577599, 2415919359, 2130706687, 1862271231,
    1593835775, 1325400319, 1056964863, 771752191, 503316735, 234881279, 4278190335, 4026532095, 3758096639,
    3489661183, 3221225727, 2952790271, 2684354815, 2399142143, 2130706687, 1862271231, 1593835775, 1325400319,
    1040187647, 771752191, 520093951, 234881279, 4294901760, 4043243520, 3774808064, 3506372608, 3221159936,
    2952724480, 2684289024, 2399076352, 2147418112, 1862205440, 1593769984, 1308557312, 1040121856, 771686400,
    503250944, 234815488, 4294901760, 4060020736, 3758030848, 3506372608, 3221159936, 2952724480, 2684289024,
    2415853568, 2130640896, 1862205440, 1593769984, 1308557312, 1040121856, 771686400, 503250944, 234815488,
    4294901760, 4043243520, 3774808064, 3489595392, 3237937152, 2952724480, 2684289024, 2415853568, 2147418112,
    1862205440, 1593769984, 1325334528, 1056899072, 788463616, 503250944, 234815488,
    32, 32,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    268369920, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 1509949695,
    3120562431, 4009754879, 4194304255, 3690987775, 2130706687, 83886335, 0, 50331903, 1694499071, 3170894079,
    3992977663, 4211081471, 3657433343, 1879048447, 16777471, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3087007999, 2281701631, 1191182591, 1040187647, 2030043391, 4127195391, 2566914303, 0, 16777471, 3254780159,
    2181038335, 1191182591, 973078783, 2030043391, 4177527039, 2130706687, 4294901760, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 0, 0, 0, 0, 0, 2214592767, 4093640959, 0, 0, 0, 0, 0, 0, 0,
    2298478847, 3909091583, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2214592767, 3607101695, 0,
    0, 0, 0, 0, 0, 0, 1946157311, 4093640959, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760,
    4294901760, 0, 0, 536871167, 1191182591, 2281701631, 3019899135, 637534463, 0, 0, 0, 100597760, 251592704,
    33488896, 0, 3321889023, 2919235839, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2550137087, 4278190335,
    4278190335, 3405775103, 570425599, 0, 0, 0, 0, 0, 0, 2046820607, 4043309311, 620757247, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 33488896, 0, 0, 218104063, 1291845887, 3841982719,
    3388997887, 0, 0, 0, 0, 0, 1996488959, 4093640959, 1073742079, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1761607935, 4278190335, 150995199, 0, 0, 67109119, 2550137087, 3909091583, 889192703, 0, 0,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 0, 0, 0, 0, 0, 2181038335,
    3925868799, 0, 0, 218104063, 3070230783, 3623878911, 570425599, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 805306623, 3288334591, 1795162367, 1040187647, 1023410431, 2231369983, 4211081471, 1694499071, 0, 369099007,
    3456106751, 3825205503, 1174405375, 872415487, 872415487, 872415487, 872415487, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 4293984270, 2046951677, 3422552319, 4110418175, 4177527039,
    3405775103, 1409286399, 0, 0, 1409286399, 4278190335, 4278190335, 4278190335, 4278190335, 4278190335, 4278190335,
    4278190335, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760,
    4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4278255360,
    4144037632, 4009819904, 3875602176, 3741384448, 3607166720, 3472948992, 3338731264, 3204513536, 3053518592,
    2936078080, 2801860352, 2650865408, 2516647680, 2382429952, 2264989440, 2113994496, 1996553984, 1862336256,
    1728118528, 1577123584, 1459683072, 1325465344, 1191247616, 1040252672, 922812160, 771817216, 637599488,
    503381760, 385941248, 234946304, 100728576, 4278255360, 4144037632, 4009819904, 3875602176, 3724607232,
    3607166720, 3472948992, 3338731264, 3204513536, 3070295808, 2936078080, 2801860352, 2667642624, 2516647680,
    2399207168, 2264989440, 2130771712, 1996553984, 1845559040, 1728118528, 1593900800, 1459683072, 1308688128,
    1191247616, 1057029888, 922812160, 788594432, 637599488, 503381760, 369164032, 234946304, 117505792, 4278255360,
    4144037632, 4009819904, 3875602176, 3741384448, 3607166720, 3472948992, 3338731264, 3204513536, 3053518592,
    2919300864, 2801860352, 2650865408, 2533424896, 2399207168, 2264989440, 2113994496, 1996553984, 1862336256,
    1728118528, 1593900800, 1459683072, 1325465344, 1191247616, 1040252672, 906034944, 771817216, 654376704,
    503381760, 369164032, 234946304, 117505792, 4278255360, 4144037632, 4009819904, 3858824960, 3741384448,
    3607166720, 3472948992, 3338731264, 3204513536, 3070295808, 2936078080, 2801860352, 2667642624, 2533424896,
    2382429952, 2264989440, 2130771712, 1979776768, 1862336256, 1728118528, 1577123584, 1442905856, 1325465344,
    1191247616, 1040252672, 922812160, 771817216, 637599488, 503381760, 369164032, 234946304, 100728576, 4278255360,
    4144037632, 4009819904, 3875602176, 3741384448, 3607166720, 3472948992, 3338731264, 3204513536, 3070295808,
    2919300864, 2801860352, 2667642624, 2533424896, 2399207168, 2264989440, 2113994496, 1996553984, 1862336256,
    1728118528, 1593900800, 1442905856, 1342241795, 1174470400, 1057029888, 906034944, 788594432, 654376704,
    503381760, 385941248, 251723520, 100728576, 4278190335, 4160749823, 4026532095, 3892314367, 3741319423,
    3623878911, 3472883967, 3338666239, 3221225727, 3070230783, 2952790271, 2818572543, 2667577599, 2533359871,
    2399142143, 2264924415, 2147483903, 1996488959, 1862271231, 1728053503, 1593835775, 1459618047, 1325400319,
    1191182591, 1056964863, 922747135, 788529407, 654311679, 520093951, 385876223, 251658495, 117440767, 4278190335,
    4160749823, 4026532095, 3892314367, 3741319423, 3623878911, 3489661183, 3355443455, 3221225727, 3087007999,
    2936013055, 2801795327, 2667577599, 2533359871, 2399142143, 2281701631, 2130706687, 1996488959, 1862271231,
    1728053503, 1593835775, 1459618047, 1325400319, 1191182591, 1056964863, 922747135, 788529407, 654311679,
    520093951, 385876223, 234881279, 100663551, 4278190335, 4160749823, 4026532095, 3892314367, 3758096639,
    3623878911, 3489661183, 3355443455, 3221225727, 3087007999, 2936013055, 2801795327, 2667577599, 2550137087,
    2415919359, 2264924415, 2130706687, 1996488959, 1862271231, 1728053503, 1593835775, 1459618047, 1325400319,
    1191182591, 1056964863, 922747135, 788529407, 654311679, 503316735, 369099007, 251658495, 100663551, 4278190335,
    4160749823, 4026532095, 3892314367, 3758096639, 3623878911, 3489661183, 3355443455, 3204448511, 3087007999,
    2936013055, 2818572543, 2667577599, 2533359871, 2399142143, 2264924415, 2130706687, 1996488959, 1879048447,
    1728053503, 1593835775, 1459618047, 1325400319, 1191182591, 1056964863, 922747135, 788529407, 654311679,
    520093951, 385876223, 251658495, 117440767, 4278190335, 4160749823, 4026532095, 3892314367, 3758096639,
    3623878911, 3489661183, 3355443455, 3221225727, 3087007999, 2952790271, 2818572543, 2667577599, 2533359871,
    2399142143, 2264924415, 2147483903, 2013266175, 1862271231, 1744830719, 1610612991, 1476395263, 1342177535,
    1191182591, 1056964863, 922747135, 788529407, 654311679, 520093951, 385876223, 251658495, 100663551, 4294901760,
    4160684032, 4026466304, 3909025792, 3774808064, 3623813120, 3489595392, 3355377664, 3237937152, 3103719424,
    2952724480, 2818506752, 2684289024, 2550071296, 2415853568, 2281635840, 2147418112, 2013200384, 1878982656,
    1744764928, 1593769984, 1476329472, 1325334528, 1207894016, 1056899072, 939458560, 788463616, 654245888,
    520028160, 385810432, 251592704, 117374976, 4294901760, 4177461248, 4043243520, 3909025792, 3774808064,
    3640590336, 3506372608, 3355377664, 3221159936, 3086942208, 2952724480, 2818506752, 2701066240, 2550071296,
    2415853568, 2281635840, 2147418112, 2013200384, 1878982656, 1727987712, 1610547200, 1476329472, 1325334528,
    1191116800, 1073676288, 922681344, 788463616, 654245888, 520028160, 385810432, 251592704, 100597760, 4294901760,
    4177461248, 4043243520, 3909025792, 3774808064, 3640590336, 3489595392, 3372154880, 3237937152, 3103719424,
    2952724480, 2818506752, 2700935170, 2550071296, 2415853568, 2281635840, 2147418112, 2013200384, 1878982656,
    1744764928, 1610547200, 1459552256, 1342111744, 1191116800, 1056899072, 922681344, 788463616, 671023104,
    520028160, 385810432, 251592704, 100597760, 4294901760, 4177461248, 4043243520, 3909025792, 3774808064,
    3640590336, 3489595392, 3372154880, 3237937152, 3086942208, 2969501696, 2818506752, 2684289024, 2550071296,
    2432630784, 2281635840, 2147418112, 2013200384, 1862205440, 1744764928, 1610547200, 1476329472, 1342111744,
    1191116800, 1056899072, 922681344, 788463616, 654245888, 520028160, 385810432, 251592704, 117374976, 4294901760,
    4177461248, 4043243520, 3909025792, 3774808064, 3623813120, 3506372608, 3372154880, 3237937152, 3103719424,
    2952724480, 2835283968, 2684289024, 2550071296, 2432630784, 2281635840, 2147418112, 2046492676, 1862205440,
    1744764928, 1610547200, 1476329472, 1342111744, 1207894016, 1056899072, 939458560, 788463616, 654245888,
    536281096, 385810432, 251592704, 134152192,
};

Display *display;
Window window;
int screen_num;
static char *appname;

/* --------- XEMBED and systray */
#define SYSTEM_TRAY_REQUEST_DOCK   0
#define SYSTEM_TRAY_BEGIN_MESSAGE   1
#define SYSTEM_TRAY_CANCEL_MESSAGE  2

static int trapped_error_code = 0;
static int (*old_error_handler)(Display *, XErrorEvent *);

static int
error_handler(Display *display, XErrorEvent *error) {
  trapped_error_code = error->error_code;
  return 0;
}

void
trap_errors(void) {
  trapped_error_code = 0;
  old_error_handler = XSetErrorHandler(error_handler);
}

int
untrap_errors(void) {
  XSetErrorHandler(old_error_handler);
  return trapped_error_code;
}

void
send_systray_message(Display *d, long message, long tray_window, long data2, long data3) {
  XEvent ev = {};

  Atom net_system_tray = XInternAtom(d, "_NET_SYSTEM_TRAY_S0", False);
  Window tray_owner = XGetSelectionOwner(d, net_system_tray);
  if (tray_owner != None)
    XSelectInput(d, tray_owner, StructureNotifyMask);
  Atom net_system_tray_opcode = XInternAtom(d, "_NET_SYSTEM_TRAY_OPCODE", False);

  ev.xclient.type = ClientMessage;
  ev.xclient.window = tray_owner;
  ev.xclient.message_type = net_system_tray_opcode;
  ev.xclient.format = 32;
  ev.xclient.data.l[0] = CurrentTime;
  ev.xclient.data.l[1] = message;
  ev.xclient.data.l[2] = tray_window;
  ev.xclient.data.l[3] = data2;
  ev.xclient.data.l[4] = data3;

  trap_errors();
  XSendEvent(d, tray_owner, False, NoEventMask, &ev);
  XSync(d, False);
  if (untrap_errors()) {
    /* Handle errors */
    fprintf(stderr, "%s", appname);
  }
}


static void
label_toggle_cb(GtkWidget *widget, gpointer data) {
  can_haz_label = !can_haz_label;

  if (can_haz_label) {
    gtk_menu_item_set_label(GTK_MENU_ITEM(widget), "Hide label");
  } else {
    gtk_menu_item_set_label(GTK_MENU_ITEM(widget), "Show label");
  }

  return;
}

static void
activate_clicked_cb(GtkWidget *widget, gpointer data) {
  AppIndicator *ci = APP_INDICATOR(data);

  if (active) {
    app_indicator_set_status(ci, APP_INDICATOR_STATUS_ATTENTION);
    gtk_menu_item_set_label(GTK_MENU_ITEM(widget), "I'm okay now");
    active = FALSE;
  } else {
    app_indicator_set_status(ci, APP_INDICATOR_STATUS_ACTIVE);
    gtk_menu_item_set_label(GTK_MENU_ITEM(widget), "Get Attention");
    active = TRUE;
  }

  changed_active = TRUE;

  return;
}

static void
reset_icons(AppIndicator *ci) {
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(local_theme_menu))) {
    app_indicator_set_icon_theme_path(ci, ICON_PATH);
    app_indicator_set_icon_full(ci, "custom-themed-icon", "Custom Themed Icon");
    app_indicator_set_attention_icon_full(ci, "custom-themed-icon-attention", "CUstom Themed Icon for Attention");
  } else {
    app_indicator_set_icon_full(ci, DEFAULT_ICON, "System Icon");
    app_indicator_set_attention_icon_full(ci, DEFAULT_ACTIVE_ICON, "System Active Icon");
    app_indicator_set_icon_theme_path(ci, NULL);
  }

  changed_active = TRUE;
  return;
}

static void
local_icon_toggle_cb(GtkWidget *widget, gpointer data) {
  AppIndicator *ci = APP_INDICATOR(data);

  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
    app_indicator_set_icon_full(ci, LOCAL_ICON, "Local Icon");
    app_indicator_set_attention_icon_full(ci, LOCAL_ACTIVE_ICON, "Local Attention Icon");
  } else {
    reset_icons(ci);
  }

  return;
}

static void
local_theme_toggle_cb(GtkWidget *widget, gpointer data) {
  if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(local_icon_menu))) {
    reset_icons(APP_INDICATOR(data));
  }

  return;
}

static void
item_clicked_cb(GtkWidget *widget, gpointer data) {
  const gchar *text = (const gchar *) data;

  g_print("%s clicked!\n", text);
}

static void
toggle_sensitivity_cb(GtkWidget *widget, gpointer data) {
  GtkWidget *target = (GtkWidget *) data;

  gtk_widget_set_sensitive(target, !gtk_widget_is_sensitive(target));
}

static void
image_clicked_cb(GtkWidget *widget, gpointer data) {
  gtk_image_set_from_stock(GTK_IMAGE (gtk_image_menu_item_get_image(
      GTK_IMAGE_MENU_ITEM(widget))),
      GTK_STOCK_OPEN, GTK_ICON_SIZE_MENU);
}

guint percentage = 0;

gpointer go(gpointer data) {
  mainloop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(mainloop);
  return 0;
}

int main(int argc, char *argv[]) {
  int win_x, win_y;
  unsigned int win_width, win_height;
  unsigned int border_width = 1;
  char *window_name = malloc(sizeof(char) * 64);
  char *icon_name = "HelloX";
  char *display_name = NULL;
  unsigned int display_width, display_height;

  /*  Miscellaneous X variables  */
  XSizeHints *size_hints;
  XWMHints *wm_hints;
  XClassHint *class_hints;
  XTextProperty windowName, iconName;
  XEvent report;
  XFontStruct *font_info;
  XGCValues values;
  GC gc;

  appname = argv[0];

  /*  Allocate memory for our structures  */
  if (!(size_hints = XAllocSizeHints()) ||
      !(wm_hints = XAllocWMHints()) ||
      !(class_hints = XAllocClassHint())) {
    fprintf(stderr, "%s: couldn't allocate memory.\n", appname);
    exit(EXIT_FAILURE);
  }

  /*  Connect to X server  */
  if ((display = XOpenDisplay(display_name)) == NULL) {
    fprintf(stderr, "%s: couldn't connect to X server %s\n",
        appname, display_name);
    exit(EXIT_FAILURE);
  }

  /*  Get screen size from display structure macro  */
  screen_num = DefaultScreen(display);
  display_width = DisplayWidth(display, screen_num);
  display_height = DisplayHeight(display, screen_num);
  snprintf(window_name, 64, "lol w%d h%d", display_width, display_height);

  /*  Set initial window size and position, and create it  */
  win_x = win_y = 0;
  win_width = display_width / 5;
  win_height = display_height / 3;

  window = XCreateSimpleWindow(display, RootWindow(display, screen_num),
      win_x, win_y, win_width, win_height, border_width,
      BlackPixel(display, screen_num),
      WhitePixel(display, screen_num));

  /*  Set hints for window manager before mapping window  */
  if (XStringListToTextProperty(&window_name, 1, &windowName) == 0) {
    fprintf(stderr, "%s: structure allocation for windowName failed.\n",
        appname);
    exit(EXIT_FAILURE);
  }

  if (XStringListToTextProperty(&icon_name, 1, &iconName) == 0) {
    fprintf(stderr, "%s: structure allocation for iconName failed.\n",
        appname);
    exit(EXIT_FAILURE);
  }

  size_hints->flags = PPosition | PSize;

  wm_hints->flags = StateHint | InputHint;
  wm_hints->initial_state = NormalState;
  wm_hints->input = True;

  class_hints->res_name = appname;
  class_hints->res_class = "hellox";

  XSetWMProperties(display, window, &windowName, &iconName, argv, argc,
      size_hints, wm_hints, class_hints);

  /*  Choose which events we want to handle  */
  XSelectInput(display, window, ExposureMask | KeyPressMask |
      ButtonPressMask | StructureNotifyMask | PropertyChangeMask);

  /*  Load a font called "9x15"  */
  if ((font_info = XLoadQueryFont(display, "10x20")) == NULL) {
    fprintf(stderr, "%s: cannot open 10x20 font.\n", appname);
    exit(EXIT_FAILURE);
  }

  /*  Create graphics context  */
  gc = XCreateGC(display, window, 0, &values);
  XSetFont(display, gc, font_info->fid);
  XSetForeground(display, gc, BlackPixel(display, screen_num));

  /*  Display Window  */
  XMapWindow(display, window);

  Atom net_wm_icon = XInternAtom(display, "_NET_WM_ICON", False);
  Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
  Atom cardinal = XInternAtom(display, "CARDINAL", False);
  Atom utf8_string = XInternAtom(display, "UTF8_STRING", False);

  int length = 2 + 16 * 16 + 2 + 32 * 32;

  XChangeProperty(display, window, net_wm_icon, cardinal, 32,
      PropModeReplace, (const unsigned char *) buffer, length);
  // todo i have no idea if this worked.
  char *wmname = "wowowae wah";
  XChangeProperty(display, window, net_wm_name, utf8_string, 8,
      PropModeReplace, (const unsigned char *) wmname, 10);


  GtkWidget *menu = NULL;
  AppIndicator *app_indicator = NULL;

  gtk_init(&argc, &argv);

  app_indicator = app_indicator_new("example-simple-client",
      DEFAULT_ICON,
      APP_INDICATOR_CATEGORY_APPLICATION_STATUS);

  g_assert (IS_APP_INDICATOR(app_indicator));
  g_assert (G_IS_OBJECT(app_indicator));

  app_indicator_set_status(app_indicator, APP_INDICATOR_STATUS_ACTIVE);
  app_indicator_set_attention_icon_full(app_indicator, DEFAULT_ACTIVE_ICON, "System Attention Icon");
  app_indicator_set_label(app_indicator, "1%", "100%");
  app_indicator_set_title(app_indicator, "Test Inidcator");

  menu = gtk_menu_new();
  GtkWidget *item;

  item = gtk_menu_item_new_with_label("Get Attention");
  local_icon_menu = item;
  g_signal_connect (item, "activate",
      G_CALLBACK(activate_clicked_cb), app_indicator);
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), item);
  gtk_widget_show(item);
  app_indicator_set_secondary_activate_target(app_indicator, item);

  item = gtk_check_menu_item_new_with_label("Enable Local Theme");
  local_theme_menu = item;
  g_signal_connect (item, "activate",
      G_CALLBACK(local_theme_toggle_cb), app_indicator);
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), item);
  gtk_widget_show(item);

  app_indicator_set_menu(app_indicator, GTK_MENU (menu));

  g_thread_new("main loop", (GThreadFunc) go, 0);

  /*  Enter event loop  */
  while (1) {
    static char *message = "cool nzce job";
    static int length;
    static int font_height;
    static int msg_x, msg_y;

    XNextEvent(display, &report);
    if (changed_active) {
      XMapWindow(display, window);
      changed_active = FALSE;
    }
    KeySym keysym;
    switch (report.type) {
      case Expose:
        if (report.xexpose.count != 0)
          break;

        /*  Output message centrally in window  */
        length = XTextWidth(font_info, message, strlen(message));
        msg_x = (win_width - length) / 2;

        font_height = font_info->ascent + font_info->descent;
        msg_y = (win_height + font_height) / 2;

        XDrawString(display, window, gc, msg_x, msg_y,
            message, strlen(message));
        break;

      case ConfigureNotify:
        /*  Store new window width & height  */
        win_width = report.xconfigure.width;
        win_height = report.xconfigure.height;
        break;

      case ButtonPress:
        printf("wow");
        break;

      case KeyPress:
        keysym = XLookupKeysym(&report.xkey, 0);
        if (keysym == XK_Escape) {
          /*  Clean up and exit  */
          XUnloadFont(display, font_info->fid);
          XFreeGC(display, gc);
          XCloseDisplay(display);
          exit(EXIT_SUCCESS);
        } else if (keysym == XK_q) {
          XUnmapWindow(display, window);
        } else {
          printf("unhandled KeyPress %lu\n", keysym);
        }
    }
  }
}
