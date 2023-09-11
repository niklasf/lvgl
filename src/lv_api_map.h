/**
 * @file lv_api_map.h
 *
 */

#ifndef LV_API_MAP_H
#define LV_API_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

static inline LV_ATTRIBUTE_TIMER_HANDLER uint32_t lv_task_handler(void)
{
    return lv_timer_handler();
}

/**********************
 *      MACROS
 **********************/

#define lv_img_create           lv_img_create
#define lv_img_set_src          lv_img_set_src
#define lv_image_set_offset_x   lv_img_set_offset_x
#define lv_image_set_offset_y   lv_img_set_offset_y
#define lv_image_set_angle      lv_img_set_angle
#define lv_image_set_pivot      lv_img_set_pivot
#define lv_image_set_zoom       lv_img_set_zoom
#define lv_image_set_antialias  lv_img_set_antialias
#define lv_image_set_size_mode  lv_img_set_size_mode
#define lv_image_get_src        lv_img_get_src
#define lv_image_get_offset_x   lv_img_get_offset_x
#define lv_image_get_offset_y   lv_img_get_offset_y
#define lv_image_get_angle      lv_img_get_angle
#define lv_image_get_pivot      lv_img_get_pivot
#define lv_image_get_zoom       lv_img_get_zoom
#define lv_image_get_antialias  lv_img_get_antialias
#define lv_image_get_size_mode  lv_img_get_size_mode

#define lv_list_set_button_text lv_list_set_btn_text
#define lv_list_get_button_text lv_list_get_btn_text
#define lv_list_add_button      lv_list_add_btn

#define lv_button_create        lv_btn_create

#define lv_buttonmatrix_create                  lv_btnmatrix_create
#define lv_buttonmatrix_set_map                 lv_btnmatrix_set_map
#define lv_buttonmatrix_set_ctrl_map            lv_btnmatrix_set_ctrl_map
#define lv_buttonmatrix_set_selected_button     lv_btnmatrix_set_selected_button
#define lv_buttonmatrix_set_button_ctrl         lv_btnmatrix_set_button_ctrl
#define lv_buttonmatrix_clear_button_ctrl       lv_btnmatrix_clear_button_ctrl
#define lv_buttonmatrix_set_button_ctrl_all     lv_btnmatrix_set_button_ctrl_all
#define lv_buttonmatrix_clear_button_ctrl_all   lv_btnmatrix_clear_button_ctrl_all
#define lv_buttonmatrix_set_button_width        lv_btnmatrix_set_button_width
#define lv_buttonmatrix_set_one_checked         lv_btnmatrix_set_one_checked
#define lv_buttonmatrix_get_map                 lv_btnmatrix_get_map
#define lv_buttonmatrix_get_selected_button     lv_btnmatrix_get_selected_button
#define lv_buttonmatrix_get_button_text         lv_btnmatrix_get_button_text
#define lv_buttonmatrix_has_button_ctrl         lv_btnmatrix_has_button_ctrl
#define lv_buttonmatrix_get_one_checked         lv_btnmatrix_get_one_checked


/**********************
 *      MACROS
 **********************/
/** Use this macro to declare an image in a C file*/
#define LV_IMG_DECLARE(var_name) extern const lv_image_dsc_t var_name;

/**********************
 * DEPRECATED FUNCTIONS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_API_MAP_H*/
