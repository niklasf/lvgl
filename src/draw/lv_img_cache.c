/**
 * @file lv_img_cache.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_img_cache.h"
#include "../stdlib/lv_string.h"
#include "../core/lv_global.h"
#include "../misc/lv_ll.h"

/*********************
 *      DEFINES
 *********************/
#define img_cache_manager LV_GLOBAL_DEFAULT()->img_cache_mgr
#define MAX_SIZE    1024 * 1024 /*bytes*/


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void drop_yougest(void);

/**********************
 *  STATIC VARIABLES
 **********************/
static uint32_t cur_size;
lv_ll_t entry_ll;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_cache_manager_init(void)
{
    _lv_ll_init(&entry_ll, sizeof(_lv_img_cache_entry_t));
}

_lv_img_cache_entry_t * _lv_cache_add(size_t size)
{
    /*Can't cache data larger than max size*/
    if(size > MAX_SIZE) return NULL;

    /*Keep dropping items until there is enough space*/
    while(cur_size + size > MAX_SIZE) {
        drop_yougest();
    }

    LV_LOG_USER("cache add: %d", size);

    _lv_img_cache_entry_t * e = _lv_ll_ins_head(&entry_ll);
    lv_memzero(e, sizeof(_lv_img_cache_entry_t));
    e->data_size = size;
    e->weight = 1;
    return e;
}

_lv_img_cache_entry_t * _lv_cache_find_ptr(const void * src_ptr, uint32_t param1, uint32_t param2)
{
    _lv_img_cache_entry_t * e = _lv_ll_get_head(&entry_ll);
    while(e) {
        if(!e->str_src && src_ptr == e->src && param1 == e->param1 && param2 == e->param2) {
            return e;
        }
        e = _lv_ll_get_next(&entry_ll, e);
    }

    return NULL;
}

_lv_img_cache_entry_t * _lv_cache_find_str(const char * src_str, uint32_t param1, uint32_t param2)
{
    _lv_img_cache_entry_t * e = _lv_ll_get_head(&entry_ll);
    while(e) {
        if(e->str_src && strcmp(src_str, e->src) == 0 && param1 == e->param1 && param2 == e->param2) {
            return e;
        }
        e = _lv_ll_get_next(&entry_ll, e);
    }
    return NULL;
}

const void * _lv_cache_get_data(_lv_img_cache_entry_t * entry)
{
    _lv_img_cache_entry_t * e = _lv_ll_get_head(&entry_ll);
    while(e) {
        e->life += e->weight;
        e = _lv_ll_get_next(&entry_ll, e);
    }

    return entry->data;

}

void lv_cache_invalidate(_lv_img_cache_entry_t * entry)
{
    cur_size -= entry->data_size;
    LV_LOG_USER("cache drop %d", entry->data_size);

    if(entry->free_src) lv_free((void *)entry->src);
    if(entry->free_data) lv_draw_buf_free((void *)entry->data);

    _lv_ll_remove(&entry_ll, entry);
    lv_free(entry);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void drop_yougest(void)
{

    int32_t life_min = INT32_MAX;
    _lv_img_cache_entry_t * e_min = NULL;

    _lv_img_cache_entry_t * e = _lv_ll_get_head(&entry_ll);
    while(e) {
        if(e->life < life_min) e_min = e;
        e = _lv_ll_get_next(&entry_ll, e);
    }

    if(e_min == NULL) {
        LV_LOG_ERROR("Cache error: no yougest item");
        return;
    }

    lv_cache_invalidate(e_min);

}
