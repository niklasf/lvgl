/**
 * @file lv_x11.h
 *
 */

#ifndef LV_X11_H
#define LV_X11_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "../../display/lv_display.h"
#include "../../indev/lv_indev.h"

#if LV_USE_X11

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef enum {
    LV_X11_MOUSEWHEEL_MODE_ENCODER,
    LV_X11_MOUSEWHEEL_MODE_CROWN,
} lv_x11_mousewheel_mode_t;

/** Header of private display driver user data - for internal use only */
typedef struct {
    struct _XDisplay   *  display;  /**< X11 display object     */
    struct _x11_inp_data * inp_data; /**< input user data object */
} _x11_user_hdr_t;

/** optional window close callback function type
 *  @see lv_x11_window_set_close_cb
*/
typedef void(*lv_x11_close_cb)(void * user_data);

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create the X11 display.
 *
 * The minimal initialisation for initializing the X11 display driver with keyboard/mouse support:
 * @code
 * lv_display_t* disp = lv_x11_window_create("My Window Title", window_width, window_width);
 * lv_x11_inputs_create(disp, NULL);
 * @endcode
 * Or with mouse cursor icon:
 * @code
 * lv_image_dsc_t mouse_symbol = {.....};
 * lv_display_t* disp = lv_x11_window_create("My Window Title", window_width, window_width);
 * lv_x11_inputs_create(disp, &mouse_symbol);
 * @endcode
 *
 * @param[in] title    title of the created X11 window
 * @param[in] hor_res  horizontal resolution (=width) of the X11 window
 * @param[in] ver_res  vertical resolution (=height) of the X11 window
 * @return             pointer to the display object
 */
lv_display_t * lv_x11_window_create(char const * title, int32_t hor_res, int32_t ver_res);

/**
 * Create and add keyboard, mouse and scrollwheel objects and connect them to the X11 display.
 *
 * @param[in] disp      the X11 display object created by @ref lv_x11_window_create
 * @param[in] mouse_img optional image description for the mouse cursor (NULL for no/invisible mouse cursor)
 */
void lv_x11_inputs_create(lv_display_t * disp, lv_image_dsc_t const * mouse_img);

/**
 * Configure the mousewheel to function as an encoder, or as a crown/scrollwheel.
 *
 * Only effective for inputs created with @ref lv_x11_inputs_create.
 *
 * @param[in] disp the X11 display object created by @ref lv_x11_window_create
 * @param[in] mode LV_X11_MOUSEWHEEL_MODE_ENCODER (default) or LV_X11_MOUSEWHEEL_MODE_CROWN
 */
void lv_x11_set_mousewheel_mode(lv_display_t * disp, lv_x11_mousewheel_mode_t mode);

#endif /* LV_USE_X11 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_X11_H */
