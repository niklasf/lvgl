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
#include "../osal/lv_os.h"

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
static bool drop_yougest(void);

/**********************
 *  STATIC VARIABLES
 **********************/
static uint32_t cur_size;
static lv_ll_t entry_ll;
static lv_mutex_t mutex;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_cache_init(void)
{
    _lv_ll_init(&entry_ll, sizeof(lv_cache_entry_t));
    lv_mutex_init(&mutex);
}

lv_cache_entry_t * lv_cache_add(size_t size)
{
    /*Can't cache data larger than max size*/
    if(size > MAX_SIZE) return NULL;

    lv_mutex_lock(&mutex);
    /*Keep dropping items until there is enough space*/
    while(cur_size + size > MAX_SIZE) {
        bool ret = drop_yougest();

        /*No item could be dropped.
         *It can happen because the usage_count of the remaining items are not zero.
         *Try again later*/
        if(ret == false) {
            lv_mutex_unlock(&mutex);
            return NULL;
        }
    }

    LV_LOG_USER("cache add: %"LV_PRIu32, (uint32_t)size);

    lv_cache_entry_t * e = _lv_ll_ins_head(&entry_ll);
    lv_memzero(e, sizeof(lv_cache_entry_t));
    e->data_size = size;
    e->weight = 1;

    lv_mutex_unlock(&mutex);
    return e;
}

lv_cache_entry_t * lv_cache_find_ptr(const void * src_ptr, uint32_t param1, uint32_t param2)
{
    lv_mutex_lock(&mutex);
    lv_cache_entry_t * e = _lv_ll_get_head(&entry_ll);
    while(e) {
        if(!e->str_src && src_ptr == e->src && param1 == e->param1 && param2 == e->param2) {
            lv_mutex_unlock(&mutex);
            return e;
        }
        e = _lv_ll_get_next(&entry_ll, e);
    }
    lv_mutex_unlock(&mutex);
    return NULL;
}

lv_cache_entry_t * lv_cache_find_str(const char * src_str, uint32_t param1, uint32_t param2)
{
    lv_mutex_lock(&mutex);
    lv_cache_entry_t * e = _lv_ll_get_head(&entry_ll);
    while(e) {
        if(e->str_src && strcmp(src_str, e->src) == 0 && param1 == e->param1 && param2 == e->param2) {
            lv_mutex_unlock(&mutex);
            return e;
        }
        e = _lv_ll_get_next(&entry_ll, e);
    }
    lv_mutex_unlock(&mutex);
    return NULL;
}

lv_cache_entry_t * lv_cache_find_data(const void * data)
{
    lv_mutex_lock(&mutex);
    lv_cache_entry_t * e = _lv_ll_get_head(&entry_ll);
    while(e) {
        if(e->data == data) {
            lv_mutex_unlock(&mutex);
            return e;
        }
        e = _lv_ll_get_next(&entry_ll, e);
    }
    lv_mutex_unlock(&mutex);
    return NULL;
}


const void * lv_cache_get_data(lv_cache_entry_t * entry)
{
    lv_mutex_lock(&mutex);
    lv_cache_entry_t * e = _lv_ll_get_head(&entry_ll);
    while(e) {
        e->life += e->weight;
        e = _lv_ll_get_next(&entry_ll, e);
    }

    entry->usage_count++;

    lv_mutex_unlock(&mutex);
    return entry->data;

}

void lv_cache_release(lv_cache_entry_t * entry)
{
    if(entry == NULL) return;
    lv_mutex_lock(&mutex);

    if(entry->usage_count == 0) {
        LV_LOG_ERROR("More lv_cache_release than lv_cache_get_data");
        lv_mutex_unlock(&mutex);
        return;
    }
    entry->usage_count--;
    lv_mutex_unlock(&mutex);
}

void lv_cache_invalidate(lv_cache_entry_t * entry)
{
    if(entry == NULL) return;
    lv_mutex_lock(&mutex);

    cur_size -= entry->data_size;
    LV_LOG_USER("cache drop %"LV_PRIu32, (uint32_t)entry->data_size);

    if(entry->free_src) lv_free((void *)entry->src);
    if(entry->free_data) lv_draw_buf_free((void *)entry->data);

    _lv_ll_remove(&entry_ll, entry);
    lv_free(entry);
    lv_mutex_unlock(&mutex);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static bool drop_yougest(void)
{

    int32_t life_min = INT32_MAX;
    lv_cache_entry_t * e_min = NULL;

    lv_cache_entry_t * e = _lv_ll_get_head(&entry_ll);
    while(e) {
        if(e->life < life_min && e->usage_count == 0) e_min = e;
        e = _lv_ll_get_next(&entry_ll, e);
    }

    if(e_min == NULL) {
        return false;
    }

    lv_cache_invalidate(e_min);
    return true;

}
