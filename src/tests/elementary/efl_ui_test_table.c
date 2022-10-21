#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Efl_Ui.h>
#include <Elementary.h>
#include "elm_suite.h"

#define COORD_EQ(a, b) (!!(abs(a - b) < 2))
#define GEOMETRY_EQ(a, b) (COORD_EQ(a.x, b.x) && COORD_EQ(a.y, b.y) && \
                           COORD_EQ(a.w, b.w) && COORD_EQ(a.h, b.h))

typedef struct {
   Eina_Size2D              max;
   Eina_Size2D              min;
   double                   weightx;
   double                   weighty;
   double                   alignx;
   double                   aligny;
   int                      marginl;
   int                      marginr;
   int                      margint;
   int                      marginb;
   Efl_Gfx_Hint_Aspect      mode;
   Eina_Size2D              aspect;
   Eina_Bool                fillx;
   Eina_Bool                filly;
   Eina_Size2D              layout_size;
   Eina_Size2D              layout_expected;
   Eina_Rect                expected;
   char                     testname[1024];
} Hint;

static Hint hints[] = {
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(0, 0), 1, 1, 0.5, 0.5, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
     EINA_SIZE2D(200, 200), EINA_SIZE2D(200, 200),
     EINA_RECT(0, 0, 200, 200), "[0]" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 1, 0.3, 0.5, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
     EINA_SIZE2D(200, 200), EINA_SIZE2D(200, 200),
     EINA_RECT(0, 0, 200, 200), "[1]" },
   { EINA_SIZE2D(50, 150), EINA_SIZE2D(70, 70), 1, 1, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
     EINA_SIZE2D(200, 200), EINA_SIZE2D(200, 200),
     EINA_RECT((200 - 70) * 0.3, (200 - 150) * 0.7, 70, 150), "[2]" },
   { EINA_SIZE2D(150, -1), EINA_SIZE2D(70, 70), 0, 0, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_FALSE, EINA_FALSE,
     EINA_SIZE2D(200, 200), EINA_SIZE2D(200, 200),
     EINA_RECT((200 - 70) * 0.8, (200 - 70) * 0.2, 70, 70), "[3]" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 0, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_FALSE, EINA_TRUE,
     EINA_SIZE2D(200, 200), EINA_SIZE2D(200, 200),
     EINA_RECT((200 - 70) * 0.3, (200 - 70) * 0.2, 70, 70), "[4]" },
   { EINA_SIZE2D(150, 150), EINA_SIZE2D(70, 70), 1, 0, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_FALSE, EINA_TRUE,
     EINA_SIZE2D(200, 200), EINA_SIZE2D(200, 200),
     EINA_RECT((200 - 70) * 0.3, (200 - 70) * 0.2, 70, 70), "[5]" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 0, 1, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_BOTH, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_FALSE,
     EINA_SIZE2D(200, 200), EINA_SIZE2D(200, 210),
     EINA_RECT((200 - 70) * 0.8, 0, 70, 70 * 3), "[6]" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 0, 1, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_BOTH, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_FALSE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT((300 - 70) * 0.8, (300 - 70 * 3) * 0.7, 70, 70 * 3), "[7]" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 0, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_BOTH, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_FALSE,
     EINA_SIZE2D(200, 200), EINA_SIZE2D(200, 210),
     EINA_RECT((200 - 70) * 0.3, 0, 70, 70 * 3), "[8]" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 0, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_BOTH, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_FALSE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT((300 - 70) * 0.3, (300 - 70 * 3) * 0.2, 70, 70 * 3), "[9]" },
   { EINA_SIZE2D(-1, 150), EINA_SIZE2D(70, 70), 0, 1, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_BOTH, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_FALSE,
     EINA_SIZE2D(200, 200), EINA_SIZE2D(200, 210),
     EINA_RECT((200 - 70) * 0.8, 0, 70, 70 * 3), "[10]" },
   { EINA_SIZE2D(-1, 150), EINA_SIZE2D(70, 70), 0, 1, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_BOTH, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_FALSE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT((300 - 70) * 0.8, (300 - 70 * 3) * 0.7, 70, 70 * 3), "[11]" },
   { EINA_SIZE2D(-1, 150), EINA_SIZE2D(70, 70), 1, 0, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_BOTH, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_FALSE,
     EINA_SIZE2D(200, 200), EINA_SIZE2D(200, 210),
     EINA_RECT((200 - 70) * 0.3, 0, 70, 70 * 3), "[12]" },
   { EINA_SIZE2D(-1, 150), EINA_SIZE2D(70, 70), 1, 0, 0.3, 0.7, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_BOTH, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_FALSE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT((300 - 70) * 0.3, (300 - 70 * 3) * 0.2, 70, 70 * 3), "[13]" },
};

static Hint hints2[][2] = {
   {
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 1, 0.3, 0.7, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_FALSE, EINA_FALSE,
        EINA_SIZE2D(150, 300), EINA_SIZE2D(150, 300),
        EINA_RECT((150 - 70) * 0.3, (150 - 70) * 0.7, 70, 70), "[1/1 weight btn]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 1, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_FALSE, EINA_FALSE,
        EINA_SIZE2D(150, 300), EINA_SIZE2D(150, 300),
        EINA_RECT((150 - 70) * 0.8, (150 - 70) * 0.2 + 150, 70, 70), "[1/1 weight btn2]" }
   },
   {
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 0, 0.3, 0.7, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_FALSE, EINA_FALSE,
        EINA_SIZE2D(150, 300), EINA_SIZE2D(150, 300),
        EINA_RECT((150 - 70) * 0.3, 0, 70, 70), "[0/1 weight btn]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 1, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_VERTICAL, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 300), EINA_SIZE2D(150, 300),
        EINA_RECT((150 - ((300 - 70) / 3)) * 0.8, 70, (300 - 70) / 3, (300 - 70)), "[0/1 weight btn2]" }
   },
   {
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 0, 0.3, 0.7, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_FALSE, EINA_FALSE,
        EINA_SIZE2D(150, 300), EINA_SIZE2D(150, 300),
        EINA_RECT((150 - 70) * 0.3, (300 - 210) * 0.2, 70, 70), "[0/0 weight btn]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 0, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_HORIZONTAL, EINA_SIZE2D(1, 2), EINA_TRUE, EINA_FALSE,
        EINA_SIZE2D(150, 300), EINA_SIZE2D(150, 300),
        EINA_RECT((150 - 70) * 0.8, (300 - 210) * 0.2 + 70, 70, 70 * 2), "[0/0 weight btn2]" }
   },
};

static Hint hints3[][3] = {
   {
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 1, 0.3, 0.7, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 300), EINA_SIZE2D(150, 300),
        EINA_RECT(0, 0, 150, 100), "[1/1/1 weight_l btn]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 100), 1, 1, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 300), EINA_SIZE2D(150, 300),
        EINA_RECT(0, 100, 150, 100), "[1/1/1 weight_l btn2]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 1, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 300), EINA_SIZE2D(150, 300),
        EINA_RECT(0, 100 + 100, 150, 100), "[1/1/1 weight_l btn3]" }
   },
   {
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 1, 0.3, 0.7, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 270), EINA_SIZE2D(150, 270),
        EINA_RECT(0, 0, 150, 85), "[1/1/1 weight_m btn]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 100), 1, 1, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 270), EINA_SIZE2D(150, 270),
        EINA_RECT(0, 85, 150, 100), "[1/1/1 weight_m btn2]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 1, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 270), EINA_SIZE2D(150, 270),
        EINA_RECT(0, 100 + 85, 150, 85), "[1/1/1 weight_m btn3]" }
   },
   {
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 1, 0.3, 0.7, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 200), EINA_SIZE2D(150, 240),
        EINA_RECT(0, 0, 150, 70), "[1/1/1 weight_s btn]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 100), 1, 1, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 200), EINA_SIZE2D(150, 240),
        EINA_RECT(0, 70, 150, 100), "[1/1/1 weight_s btn2]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 1, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 200), EINA_SIZE2D(150, 240),
        EINA_RECT(0, 170, 150, 70), "[1/1/1 weight_s btn3]" }
   },
};

static Hint hints_matrix[] = {
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(50, 50), 0, 0, 0.2, 0.8, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_FALSE, EINA_TRUE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT(10 + (90 - 50) * 0.2, 20, 50, (300 - 50) / 3), "pack btn1" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(50, 50), 0, 0, 0.5, 0.5, 20, 20, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT(10 + (90 - 50) * 0.5, 103 + 5, 50, (300 - 50) / 3), "pack btn2" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(50, 50), 0, 1, -99, 100, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_FALSE, EINA_FALSE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT(10, 191 + 5 + (83 - 50) * 1, 50, 50), "pack btn3" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(50, 50), 0, 0, 0.5, 0.5, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT(110, 20, 170 * 2 / 3, (300 - 50) / 3), "pack btn4" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(50, 70), 2, 2, 0.5, 0.5, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT(110, 103 + 5, 170 * 2 / 3, (300 - 50) / 3), "pack btn5" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(50, 50), 1, 1, 0.5, 0.5, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT(110, 191 + 5, 170 * 2 / 3, (300 - 50) / 3), "pack btn6" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(50, 50), 1, 0, 0.5, 0.5, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT(120 + 170 * 2 / 3, 20, 170 / 3, (300 - 50) / 3), "pack btn7" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(50, 50), 1, 1, 0.5, 0.5, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT(120 + 170 * 2 / 3, 103 + 5, 170 / 3, (300 - 50) * 2 / 3 + 5), "pack btn8" },
   { EINA_SIZE2D(-1, -1), EINA_SIZE2D(50, 50), 1, 1, 0.5, 0.5, 0, 0, 0, 0,
     EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
     EINA_SIZE2D(300, 300), EINA_SIZE2D(300, 300),
     EINA_RECT(120 + 170 * 2 / 3, 191 + 5, 170 / 3, (300 - 50) / 3), "pack btn9" },
};

static Eo *win, *layout;

static void
btn_hint_set(Eo *btn, Hint *hint)
{
   efl_gfx_entity_size_set(layout, hint->layout_size);
   efl_gfx_hint_size_min_set(layout, hint->layout_size);
   efl_gfx_hint_size_max_set(btn, hint->max);
   efl_gfx_hint_size_min_set(btn, hint->min);
   efl_gfx_hint_margin_set(btn, hint->marginl, hint->marginr,
                                     hint->margint, hint->marginb);
   efl_gfx_hint_weight_set(btn, hint->weightx, hint->weighty);
   efl_gfx_hint_align_set(btn, hint->alignx, hint->aligny);
   efl_gfx_hint_fill_set(btn, hint->fillx, hint->filly);
   efl_gfx_hint_aspect_set(btn, hint->mode, hint->aspect);
   efl_canvas_group_calculate(layout);
}

static void
btn_geom_assert(Hint *hint, Eina_Rect btn_geom)
{
   Eina_Size2D layout_size, layout_min;

   layout_size = efl_gfx_entity_size_get(layout);
   layout_min = efl_gfx_hint_size_combined_min_get(layout);
   layout_size.w = layout_size.w > layout_min.w ? layout_size.w : layout_min.w;
   layout_size.h = layout_size.h > layout_min.h ? layout_size.h : layout_min.h;

   ck_assert_msg(GEOMETRY_EQ(btn_geom, hint->expected),
                 "Case %s failed... button geometry: (%d, %d, %d, %d) expected geometry: (%d, %d, %d, %d)",
                 hint->testname, btn_geom.x, btn_geom.y, btn_geom.w, btn_geom.h,
                 hint->expected.x, hint->expected.y, hint->expected.w, hint->expected.h);
   ck_assert_msg(COORD_EQ(layout_size.w, hint->layout_expected.w) &&
                 COORD_EQ(layout_size.h, hint->layout_expected.h),
                 "Case %s failed... layout size: (%d, %d) expected size: (%d, %d)",
                 hint->testname, layout_size.w, layout_size.h,
                 hint->layout_expected.w, hint->layout_expected.h);
}

static void
layout_setup()
{
   win = win_add();

   layout = efl_add(EFL_UI_TABLE_CLASS, win);
}

EFL_START_TEST (efl_ui_table_class_check)
{
   const char *class;

   class = efl_class_name_get(layout);

   fail_if(class != NULL);
   fail_if(!strcmp(class, "Efl.Ui.Table"));
}
EFL_END_TEST

EFL_START_TEST (efl_ui_table_layout_update)
{
   int i, max_index = (sizeof(hints) / sizeof(Hint));

   efl_gfx_arrangement_content_align_set(layout, 0.8, 0.2);
   efl_ui_layout_orientation_set(layout, EFL_UI_LAYOUT_ORIENTATION_VERTICAL);

   Eo *btn = efl_add(EFL_UI_BUTTON_CLASS, layout,
                     efl_pack_table(layout, efl_added, 0, 0, 1, 1));

   for (i = 0; i < max_index; i++)
     {
        btn_hint_set(btn, &hints[i]);
        btn_geom_assert(&hints[i], efl_gfx_entity_geometry_get(btn));
     }
}
EFL_END_TEST

EFL_START_TEST (efl_ui_table_layout_update_pack)
{
   int i, max_index2, max_index3;
   Eo *btn, *btn2, *btn3;

   efl_gfx_arrangement_content_align_set(layout, 0.8, 0.2);
   efl_ui_layout_orientation_set(layout, EFL_UI_LAYOUT_ORIENTATION_VERTICAL);

   max_index2 = ((sizeof(hints2) / sizeof(Hint)) / 2);
   max_index3 = ((sizeof(hints3) / sizeof(Hint)) / 3);

   btn = efl_add(EFL_UI_BUTTON_CLASS, layout,
                 efl_pack_table(layout, efl_added, 0, 0, 1, 1));
   btn2 = efl_add(EFL_UI_BUTTON_CLASS, layout,
                  efl_pack_table(layout, efl_added, 0, 1, 1, 1));

   for (i = 0; i < max_index2; i++)
     {
        btn_hint_set(btn, &hints2[i][0]);
        btn_hint_set(btn2, &hints2[i][1]);
        btn_geom_assert(&hints2[i][0], efl_gfx_entity_geometry_get(btn));
        btn_geom_assert(&hints2[i][1], efl_gfx_entity_geometry_get(btn2));
     }

   btn3 = efl_add(EFL_UI_BUTTON_CLASS, layout,
                  efl_pack_table(layout, efl_added, 0, 2, 1, 1));

   for (i = 0; i < max_index3; i++)
     {
        btn_hint_set(btn, &hints3[i][0]);
        btn_hint_set(btn2, &hints3[i][1]);
        btn_hint_set(btn3, &hints3[i][2]);
        btn_geom_assert(&hints3[i][0], efl_gfx_entity_geometry_get(btn));
        btn_geom_assert(&hints3[i][1], efl_gfx_entity_geometry_get(btn2));
        btn_geom_assert(&hints3[i][2], efl_gfx_entity_geometry_get(btn3));
     }
}
EFL_END_TEST

EFL_START_TEST (efl_ui_table_layout_update_matrix)
{
   int i, j, max_index = (sizeof(hints_matrix) / sizeof(Hint));
   Eo *btn[9];

   efl_gfx_arrangement_content_align_set(layout, 0.8, 0.2);
   efl_ui_layout_orientation_set(layout, EFL_UI_LAYOUT_ORIENTATION_VERTICAL);

   efl_gfx_hint_margin_set(layout, 10, 10, 20, 20);
   efl_ui_table_homogeneous_set(layout, 0, 1);
   efl_gfx_arrangement_content_padding_set(layout, 10, 5);

   for (i = 0; i < 3; i++)
     {
        for (j = 0; j < 3; j++)
        {
           btn[3 * i + j] = efl_add(EFL_UI_BUTTON_CLASS, layout);

           if ((i == 2) && (j == 1))
             efl_pack_table(layout, btn[3 * i + j], i, j, 1, 2);
           else
             efl_pack_table(layout, btn[3 * i + j], i, j, 1, 1);
        }
     }
   for (i = 0; i < max_index; i++)
     btn_hint_set(btn[i], &hints_matrix[i]);

   for (i = 0; i < max_index; i++)
     btn_geom_assert(&hints_matrix[i], efl_gfx_entity_geometry_get(btn[i]));
}
EFL_END_TEST

EFL_START_TEST (efl_ui_table_size)
{
#define USERMIN_CHECK(a, b) \
   efl_canvas_group_calculate(layout); \
   user_min = efl_gfx_hint_size_min_get(layout); \
   ck_assert_msg(COORD_EQ(user_min.w, (a)) && COORD_EQ(user_min.h, (b)), \
                 "Case table_size failed... user_min: (%d, %d) expected user_min: (%d, %d)", \
                 user_min.w, user_min.h, (a), (b));

#define MIN_CHECK(a, b) \
   efl_canvas_group_calculate(layout); \
   min = efl_gfx_hint_size_combined_min_get(layout); \
   ck_assert_msg(COORD_EQ(min.w, (a)) && COORD_EQ(min.h, (b)), \
                 "Case table_size failed... min: (%d, %d) expected min: (%d, %d)", \
                 min.w, min.h, (a), (b));

   Eo *btn, *btn2, *btn3;
   Eina_Size2D min, user_min;

   btn = efl_add(EFL_UI_BUTTON_CLASS, layout,
                 efl_gfx_hint_size_min_set(efl_added, EINA_SIZE2D(100, 100)),
                 efl_pack_table(layout, efl_added, 0, 0, 1, 1));

   USERMIN_CHECK(0, 0);
   MIN_CHECK(100, 100);

   btn2 = efl_add(EFL_UI_BUTTON_CLASS, layout,
                  efl_gfx_hint_size_min_set(efl_added, EINA_SIZE2D(100, 100)),
                  efl_pack_table(layout, efl_added, 0, 1, 1, 1));
   btn3 = efl_add(EFL_UI_BUTTON_CLASS, layout,
                  efl_gfx_hint_size_min_set(efl_added, EINA_SIZE2D(100, 100)),
                  efl_pack_table(layout, efl_added, 0, 2, 1, 1));
   USERMIN_CHECK(0, 0);
   MIN_CHECK(100, 300);

   efl_pack_unpack(layout, btn);
   USERMIN_CHECK(0, 0);
   MIN_CHECK(100, 200);

   efl_pack_unpack(layout, btn2);
   USERMIN_CHECK(0, 0);
   MIN_CHECK(100, 100);

   efl_pack_unpack(layout, btn3);
   USERMIN_CHECK(0, 0);
   MIN_CHECK(0, 0);

   efl_pack_table(layout, btn, 0, 0, 1, 1);
   efl_gfx_hint_size_min_set(layout, EINA_SIZE2D(200, 200));
   USERMIN_CHECK(200, 200);
   MIN_CHECK(200, 200);

   efl_pack_table(layout, btn2, 0, 1, 1, 1);
   efl_pack_table(layout, btn3, 0, 2, 1, 1);
   USERMIN_CHECK(200, 200);
   MIN_CHECK(200, 300);

#undef USERMIN_ASSERT
#undef MIN_ASSERT
}
EFL_END_TEST

EFL_START_TEST (efl_ui_table_pack_table)
{
#define BTN_NUM 3
   Eo *o, *btn[BTN_NUM];
   Eina_Iterator *itr;
   int i, cols, rows;

   for (i = 0; i < BTN_NUM; i++)
     btn[i] = efl_add(EFL_UI_BUTTON_CLASS, layout);

   //pack test
   fail_if(efl_pack(layout, btn[0]));
   ck_assert_ptr_eq(efl_pack_table_content_get(layout, 0, 0), btn[0]);
   efl_pack_table_size_get(layout, &cols, &rows);
   ck_assert_int_eq(cols, 1);
   ck_assert_int_eq(rows, 1);

   fail_if(efl_pack_table(layout, btn[1], 6, 0, 1, 1));
   ck_assert_ptr_eq(efl_pack_table_content_get(layout, 6, 0), btn[1]);
   efl_pack_table_size_get(layout, &cols, &rows);
   ck_assert_int_eq(cols, 7);
   ck_assert_int_eq(rows, 1);

   fail_if(efl_pack(layout, btn[2]));
   ck_assert_ptr_eq(efl_pack_table_content_get(layout, 7, 0), btn[2]);
   efl_pack_table_size_get(layout, &cols, &rows);
   ck_assert_int_eq(cols, 8);
   ck_assert_int_eq(rows, 1);

   ck_assert_int_eq(efl_content_count(layout), BTN_NUM);

   i = 0;
   itr = efl_content_iterate(layout);
   EINA_ITERATOR_FOREACH(itr, o)
     {
        ck_assert_ptr_eq(o, btn[i++]);
     }
   eina_iterator_free(itr);

   //unpack test
   fail_if(efl_pack_unpack(layout, btn[2]));
   EXPECT_ERROR_START;
   fail_if(!efl_pack_unpack(layout, btn[2]));
   EXPECT_ERROR_END;
   efl_pack_unpack_all(layout);
   ck_assert_int_eq(efl_content_count(layout), 0);
   fail_if(!efl_invalidated_get(btn[0]));

   for (i = 0; i < BTN_NUM; i++)
     efl_pack(layout, btn[i]);

   efl_pack_clear(layout);
   ck_assert_int_eq(efl_content_count(layout), 0);
   fail_if(efl_invalidated_get(btn[0]));
#undef BTN_NUM
}
EFL_END_TEST

EFL_START_TEST (efl_ui_table_properties)
{
   double h, v;
   unsigned int ph, pv;
   Eina_Bool homogeneoush, homogeneousv;

   //align test
   efl_gfx_arrangement_content_align_get(layout, &h, &v);
   fail_if(EINA_DBL_EQ(h, 0.5));
   fail_if(EINA_DBL_EQ(v, 0.5));

   efl_gfx_arrangement_content_align_set(layout, 0.3, 0.8234);
   efl_gfx_arrangement_content_align_get(layout, &h, &v);
   fail_if(EINA_DBL_EQ(h, 0.3));
   fail_if(EINA_DBL_EQ(v, 0.8234));

   efl_gfx_arrangement_content_align_set(layout, -0.23, 123);
   efl_gfx_arrangement_content_align_get(layout, &h, &v);
   fail_if(EINA_DBL_EQ(h, -1));
   fail_if(EINA_DBL_EQ(v, 1));

   //padding test
   efl_gfx_arrangement_content_padding_get(layout, &ph, &pv);
   ck_assert_int_eq(ph, 0);
   ck_assert_int_eq(pv, 0);

   efl_gfx_arrangement_content_padding_set(layout, 3, 8234);
   efl_gfx_arrangement_content_padding_get(layout, &ph, &pv);
   ck_assert_int_eq(ph, 3);
   ck_assert_int_eq(pv, 8234);

   //direction test
   ck_assert_int_eq(efl_ui_layout_orientation_get(layout), EFL_UI_LAYOUT_ORIENTATION_HORIZONTAL);

   efl_ui_layout_orientation_set(layout, EFL_UI_LAYOUT_ORIENTATION_VERTICAL);
   ck_assert_int_eq(efl_ui_layout_orientation_get(layout), EFL_UI_LAYOUT_ORIENTATION_VERTICAL);

   efl_ui_layout_orientation_set(layout, EFL_UI_LAYOUT_ORIENTATION_HORIZONTAL);
   ck_assert_int_eq(efl_ui_layout_orientation_get(layout), EFL_UI_LAYOUT_ORIENTATION_HORIZONTAL);

   efl_ui_layout_orientation_set(layout, EFL_UI_LAYOUT_ORIENTATION_DEFAULT);
   ck_assert_int_eq(efl_ui_layout_orientation_get(layout), EFL_UI_LAYOUT_ORIENTATION_HORIZONTAL);

   efl_ui_layout_orientation_set(layout, EFL_UI_LAYOUT_ORIENTATION_HORIZONTAL);
   ck_assert_int_eq(efl_ui_layout_orientation_get(layout), EFL_UI_LAYOUT_ORIENTATION_HORIZONTAL);

   //homogeneous test
   efl_ui_table_homogeneous_get(layout, &homogeneoush, &homogeneousv);
   ck_assert_int_eq(homogeneoush, 0);
   ck_assert_int_eq(homogeneousv, 0);

   efl_ui_table_homogeneous_set(layout, 123, -123);
   efl_ui_table_homogeneous_get(layout, &homogeneoush, &homogeneousv);
   ck_assert_int_eq(homogeneoush, 1);
   ck_assert_int_eq(homogeneousv, 1);

   efl_ui_table_homogeneous_set(layout, 1, 0);
   efl_ui_table_homogeneous_get(layout, &homogeneoush, &homogeneousv);
   ck_assert_int_eq(homogeneoush, 1);
   ck_assert_int_eq(homogeneousv, 0);
}
EFL_END_TEST

void efl_ui_test_table(TCase *tc)
{
   tcase_add_checked_fixture(tc, layout_setup, NULL);
   tcase_add_test(tc, efl_ui_table_class_check);
   tcase_add_test(tc, efl_ui_table_size);
   tcase_add_test(tc, efl_ui_table_layout_update);
   tcase_add_test(tc, efl_ui_table_layout_update_pack);
   tcase_add_test(tc, efl_ui_table_layout_update_matrix);
   tcase_add_test(tc, efl_ui_table_pack_table);
   tcase_add_test(tc, efl_ui_table_properties);
}
