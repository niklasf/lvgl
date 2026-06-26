/**
 * @file lv_spinner.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_spinner_private.h"
#include "../../lvgl_public.h"
#if LV_USE_SPINNER

#include "../../misc/lv_anim_private.h"
#include "../../core/lv_obj_class_private.h"

/*Check dependencies*/
#if LV_USE_ARC == 0
    #error "lv_spinner: lv_arc is required. Enable it in lv_conf.h (LV_USE_ARC  1) "
#endif

/*Check dependencies*/
#if LV_USE_ARC == 0
    #error "lv_spinner: lv_arc is required. Enable it in lv_conf.h (LV_USE_ARC  1) "
#endif

/*********************
 *      DEFINES
 *********************/
#define DEF_ARC_ANGLE   200
#define DEF_TIME        1000
#define MY_CLASS        (&lv_spinner_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_spinner_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void arc_anim_angles(void * obj, int32_t v);

/**********************
 *  STATIC VARIABLES
 **********************/

#if LV_USE_OBJ_PROPERTY
static const lv_property_ops_t lv_spinner_properties[] = {
    {
        .id = LV_PROPERTY_SPINNER_ANIM_DURATION,
        .setter = lv_spinner_set_anim_duration,
        .getter = lv_spinner_get_anim_duration,
    },
    {
        .id = LV_PROPERTY_SPINNER_ARC_SWEEP,
        .setter = lv_spinner_set_arc_sweep,
        .getter = lv_spinner_get_arc_sweep,
    },
};
#endif

const lv_obj_class_t lv_spinner_class = {
    .base_class = &lv_arc_class,
    .constructor_cb = lv_spinner_constructor,
    .name = "lv_spinner",
    .instance_size = sizeof(lv_spinner_t),
    LV_PROPERTY_CLASS_FIELDS(spinner, SPINNER)
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_spinner_create(lv_obj_t * parent)
{
    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

void lv_spinner_set_anim_params(lv_obj_t * obj, uint32_t t, uint32_t angle)
{
    LV_CHECK_OBJ(obj, MY_CLASS, return);
    lv_spinner_t * spinner = (lv_spinner_t *)obj;

    spinner->duration = t;
    spinner->angle = angle;

    /*Delete the current animation*/
    lv_anim_delete(obj, NULL);

    /*A single animation drives both the start and the end angle.
     *The two angles used to live in two separate (but phase-locked)
     *animations; folding them into one halves the per-frame animation
     *bookkeeping. The animated value `v` is the linear progress in degrees;
     *the end angle uses it directly while the start angle re-applies the
     *original cubic-bezier easing inside the callback.*/
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, arc_anim_angles);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_duration(&a, t);
    lv_anim_set_values(&a, 0, 360);
    lv_anim_start(&a);

    lv_arc_set_bg_angles(obj, 0, 360);
    lv_arc_set_rotation(obj, 270);
}

void lv_spinner_set_anim_duration(lv_obj_t * obj, uint32_t t)
{
    LV_CHECK_OBJ(obj, MY_CLASS, return);
    lv_spinner_t * spinner = (lv_spinner_t *)obj;

    lv_spinner_set_anim_params(obj, t, spinner->angle);
}

void lv_spinner_set_arc_sweep(lv_obj_t * obj, uint32_t angle)
{
    LV_CHECK_OBJ(obj, MY_CLASS, return);
    lv_spinner_t * spinner = (lv_spinner_t *)obj;

    lv_spinner_set_anim_params(obj, spinner->duration, angle);
}

uint32_t lv_spinner_get_anim_duration(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, return 0);
    lv_spinner_t * spinner = (lv_spinner_t *)obj;
    return spinner->duration;
}

uint32_t lv_spinner_get_arc_sweep(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, return 0);
    lv_spinner_t * spinner = (lv_spinner_t *)obj;
    return spinner->angle;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_spinner_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_TRACE_OBJ_CREATE("begin");

    LV_UNUSED(class_p);

    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);

    lv_spinner_set_anim_params(obj, DEF_TIME, DEF_ARC_ANGLE);
}

static void arc_anim_angles(void * var, int32_t v)
{
    lv_obj_t * obj = var;
    lv_spinner_t * spinner = (lv_spinner_t *)obj;

    /*End angle: linear sweep (was the linear end-angle animation)*/
    lv_value_precise_t end = spinner->angle + v;

    /*Start angle: same progress, eased by the original cubic-bezier path
     *(0.42, 0.58, 0, 1) over the value range 0..360*/
    uint32_t t = lv_map(v, 0, 360, 0, LV_BEZIER_VAL_MAX);
    int32_t step = lv_cubic_bezier(t, LV_BEZIER_VAL_FLOAT(0.42f), LV_BEZIER_VAL_FLOAT(0.58f),
                                   LV_BEZIER_VAL_FLOAT(0.0f), LV_BEZIER_VAL_FLOAT(1.0f));
    lv_value_precise_t start = (lv_value_precise_t)((step * 360) >> LV_BEZIER_VAL_SHIFT);

    /*Set both angles in a single call*/
    lv_arc_set_angles(obj, start, end);
}

#endif /*LV_USE_SPINNER*/
