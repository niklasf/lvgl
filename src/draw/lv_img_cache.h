/**
 * @file lv_img_cache.h
 *
 */

#ifndef LV_IMG_CACHE_H
#define LV_IMG_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lv_img_decoder.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    /**The image source or other source related to the cache content.*/
    const void * src;

    /** Some extra parameters to describe the source. E.g. the current frame of an animation*/
    uint32_t param1;
    uint32_t param2;

    /** The data to cache*/
    const void * data;

    /** Size of data in bytes*/
    uint32_t data_size;

    /** On access to any cache entry, `life` of each cache entry will be incremented by their own `weight` to keep the entry alive longer*/
    uint32_t weight;

    /** The current `life`. Entries with the smallest life will be purged from the cache if a new entry needs to be cached*/
    int32_t life;

    /** `src` is a string, so compare it with `lv_strcmp()` */
    uint32_t str_src   : 1;

    /** Call `lv_free` on `src` when the entry is removed from the cache */
    uint32_t free_src   : 1;

    /** Call `lv_draw_buf_free` on `data` when the entry is removed from the cache */
    uint32_t free_data   : 1;

    /**Any user data if needed*/
    void * user_data;
} lv_cache_entry_t;






typedef struct {
    lv_cache_entry_t * (*open_cb)(const void * src, lv_color_t color, int32_t frame_id);
    void (*set_size_cb)(uint16_t new_entry_cnt);
    void (*invalidate_src_cb)(const void * src);
} lv_img_cache_manager_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the img cache manager
 * @param manager Pointer to the img cache manager
 */
void lv_cache_init(void);

/**
 * Apply the img cache manager
 * @param manager Pointer to the img cache manager
 */
void lv_img_cache_manager_apply(const lv_img_cache_manager_t * manager);

lv_cache_entry_t * lv_cache_add(size_t size);

lv_cache_entry_t * lv_cache_find_ptr(const void * src_ptr, uint32_t param1, uint32_t param2);

lv_cache_entry_t * lv_cache_find_str(const char * src_str, uint32_t param1, uint32_t param2);

const void * lv_cache_get_data(lv_cache_entry_t * entry);

void lv_cache_invalidate(lv_cache_entry_t * entry);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_IMG_CACHE_H*/
