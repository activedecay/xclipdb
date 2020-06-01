#ifndef handler_h
#define handler_h
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <X11/keysym.h>

/* print names of modifiers present in mask */
void
modifiers(uint32_t mask) {
  const char *MODIFIERS[] = {
      "Shift", "Lock", "Ctrl", "Alt",
      "Mod2", "Mod3", "Mod4", "Mod5",
      "Button1", "Button2", "Button3", "Button4", "Button5"
  };

//  printf("Modifier mask: ");
  for (const char **modifier = MODIFIERS; mask; mask >>= 1, ++modifier) {
    if (mask & 1) {
//      printf(" %s(%x)", *modifier, mask);
    }
  }
//  printf("\n");
}

void handle(xcb_generic_event_t *event, xcb_window_t *window,
    xcb_connection_t *connection) {
  switch (event->response_type & ~0x80) {
    case XCB_EXPOSE: {
      xcb_expose_event_t *expose = (xcb_expose_event_t *) event;

//      printf("Window %"PRIu32" exposed. "
//             "Region to be redrawn at location (%"PRIu16",%"PRIu16"), "
//             "with dimension (%"PRIu16",%"PRIu16")\n",
//          expose->window, expose->x, expose->y, expose->width, expose->height);
      break;
    }
    case XCB_BUTTON_PRESS: {
      xcb_button_press_event_t *bp = (xcb_button_press_event_t *) event;
      modifiers(bp->state);

      switch (bp->detail) {
        case 4:
//          printf("Wheel Button up in window %"PRIu32
//                 ", at coordinates (%"PRIi16",%"PRIi16")\n",
//              bp->event, bp->event_x, bp->event_y);
          break;
        case 5:
//          printf("Wheel Button down in window %"PRIu32
//                 ", at coordinates (%"PRIi16",%"PRIi16")\n",
//              bp->event, bp->event_x, bp->event_y);
          break;
        default:
//          printf("Button %"PRIu8" pressed in window %"PRIu32
//                 ", at coordinates (%"PRIi16",%"PRIi16")\n",
//              bp->detail, bp->event, bp->event_x, bp->event_y);
          break;
      }
      break;
    }
    case XCB_BUTTON_RELEASE: {
      xcb_button_release_event_t *br = (xcb_button_release_event_t *) event;
      modifiers(br->state);

//      printf("Button %"PRIu8" released in window %"PRIu32
//             ", at coordinates (%"PRIi16",%"PRIi16")\n",
//          br->detail, br->event, br->event_x, br->event_y);
      break;
    }
    case XCB_MOTION_NOTIFY: {
      xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *) event;

      // window:motion->event,
      // printf("(%"PRIi16",%"PRIi16")", motion->event_x, motion->event_y);
      break;
    }
    case XCB_ENTER_NOTIFY: {
      xcb_enter_notify_event_t *enter = (xcb_enter_notify_event_t *) event;

//      printf("Mouse entered window %"PRIu32
//             ", at coordinates (%"PRIi16",%"PRIi16")\n",
//          enter->event, enter->event_x, enter->event_y);
      break;
    }
    case XCB_LEAVE_NOTIFY: {
      xcb_leave_notify_event_t *leave = (xcb_leave_notify_event_t *) event;

//      printf("Mouse left window %"PRIu32
//             ", at coordinates (%"PRIi16",%"PRIi16")\n",
//          leave->event, leave->event_x, leave->event_y);
      break;
    }
    case XCB_KEY_PRESS: {
      xcb_key_press_event_t *kp = (xcb_key_press_event_t *) event;
      modifiers(kp->state);
      xcb_keycode_t keycode = kp->detail;

      xcb_key_symbols_t *syms = xcb_key_symbols_alloc(connection);
      xcb_keysym_t i = xcb_key_press_lookup_keysym(syms, kp, 0);
      xcb_key_symbols_free(syms);

//      printf("Key '%c' pressed in window %"PRIu32"\n",
//          i,
//          kp->event);
      if (i == XK_Escape) {
        xcb_unmap_window(connection, *window);
        xcb_flush(connection);
      }
      break;
    }
    case XCB_KEY_RELEASE: {
      xcb_key_release_event_t *kr = (xcb_key_release_event_t *) event;
      modifiers(kr->state);

//      printf("Key released in window %"PRIu32"\n",
//          kr->event);
      break;
    }
    default:
      /* Unknown event type, ignore it */
//      printf("Unknown event: %"PRIu8"\n",
//          event->response_type);
      break;
  }

  free(event);
}

#endif // handler_hf