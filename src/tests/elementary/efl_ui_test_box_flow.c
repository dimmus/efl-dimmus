#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Efl_Ui.h>
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
   Efl_Gfx_Hint_Aspect mode;
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
        EINA_RECT((150 - 70) * 0.3, (300 - 280) * 0.2, 70, 70), "[0/0 weight btn]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 0, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_HORIZONTAL, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_FALSE,
        EINA_SIZE2D(150, 300), EINA_SIZE2D(150, 300),
        EINA_RECT((150 - 70) * 0.8, (300 - 280) * 0.2 + 70, 70, 70 * 3), "[0/0 weight btn2]" }
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
        EINA_SIZE2D(150, 200), EINA_SIZE2D(150, 200),
        EINA_RECT(0, 0, 75, 100), "[1/1/1 weight_s btn]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 100), 1, 1, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 200), EINA_SIZE2D(150, 200),
        EINA_RECT(0, 100, 75, 100), "[1/1/1 weight_s btn2]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(70, 70), 1, 1, 0.8, 0.2, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_NONE, EINA_SIZE2D(0, 0), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(150, 200), EINA_SIZE2D(150, 200),
        EINA_RECT(75, 0, 75, 200), "[1/1/1 weight_s btn3]" }
   },
   {
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(0, 0), 1, 1, 0.5, 0.5, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_BOTH, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(300, 900), EINA_SIZE2D(300, 900),
        EINA_RECT(100, 0, 100, 300), "[aspect resize btn]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(0, 0), 1, 1, 0.5, 0.5, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_BOTH, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(300, 900), EINA_SIZE2D(300, 900),
        EINA_RECT(100, 300, 100, 300), "[aspect resize btn2]" },
      { EINA_SIZE2D(-1, -1), EINA_SIZE2D(0, 0), 1, 1, 0.5, 0.5, 0, 0, 0, 0,
        EFL_GFX_HINT_ASPECT_BOTH, EINA_SIZE2D(1, 3), EINA_TRUE, EINA_TRUE,
        EINA_SIZE2D(300, 900), EINA_SIZE2D(300, 900),
        EINA_RECT(100, 300 + 300, 100, 300), "[aspect resize btn3]" }
   },
};

static Eo *win, *layout;

static void
btn_hint_set(Eo *btn, Hint *hint)
{
   efl_gfx_entity_size_set(layout, hint->layout_size);
   efl_gfx_hint_size_min_set(layout, hint->layout_size);
   efl_gfx_hint_size_max_set(btn, hint->max);
   efl_gfx_hint_size_min_set(btn, hint->min);
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

   layout = efl_add(EFL_UI_BOX_FLOW_CLASS, win,
                    efl_gfx_arrangement_content_align_set(efl_added, 0.8, 0.2),
                    efl_ui_layout_orientation_set(efl_added, EFL_UI_LAYOUT_ORIENTATION_VERTICAL));
}

EFL_START_TEST (efl_ui_box_flow_class_check)
{
   const char *class;

   class = efl_class_name_get(layout);

   fail_if(class != NULL);
   fail_if(!strcmp(class, "Efl.Ui.Box_Flow"));
}
EFL_END_TEST

EFL_START_TEST (efl_ui_box_flow_layout_update)
{
   int i, max_index = (sizeof(hints) / sizeof(Hint));

   Eo *btn = efl_add(EFL_UI_BUTTON_CLASS, layout,
                     efl_pack_end(layout, efl_added));

   for (i = 0; i < max_index; i++)
     {
        btn_hint_set(btn, &hints[i]);
        btn_geom_assert(&hints[i], efl_gfx_entity_geometry_get(btn));
     }
}
EFL_END_TEST

EFL_START_TEST (efl_ui_box_flow_layout_update_pack)
{
   int i, max_index2, max_index3;
   Eo *btn, *btn2, *btn3;

   max_index2 = ((sizeof(hints2) / sizeof(Hint)) / 2);
   max_index3 = ((sizeof(hints3) / sizeof(Hint)) / 3);

   btn = efl_add(EFL_UI_BUTTON_CLASS, layout,
                 efl_pack_end(layout, efl_added));
   btn2 = efl_add(EFL_UI_BUTTON_CLASS, layout,
                  efl_pack_end(layout, efl_added));

   for (i = 0; i < max_index2; i++)
     {
        btn_hint_set(btn, &hints2[i][0]);
        btn_hint_set(btn2, &hints2[i][1]);
        btn_geom_assert(&hints2[i][0], efl_gfx_entity_geometry_get(btn));
        btn_geom_assert(&hints2[i][1], efl_gfx_entity_geometry_get(btn2));
     }

   btn3 = efl_add(EFL_UI_BUTTON_CLASS, layout,
                  efl_pack_end(layout, efl_added));

   for (i = 0; i < max_index3; i++)
     {
        btn_hint_set(btn, &hints3[i][0]);
        btn_hint_set(btn2, &hints3[i][1]);
        btn_hint_set(btn3, &hints3[i][2]);
        btn_geom_assert(&hints3[i][0], efl_gfx_entity_geometry_get(btn));
        btn_geom_assert(&hints3[i][1], efl_gfx_entity_geometry_get(btn2));
        btn_geom_assert(&hints3[i][2], efl_gfx_entity_geometry_get(btn3));
     }

   // aspect resize test
   hints3[3][0].layout_expected = hints3[3][0].layout_size = EINA_SIZE2D(150, 450);
   hints3[3][1].layout_expected = hints3[3][1].layout_size = EINA_SIZE2D(150, 450);
   hints3[3][2].layout_expected = hints3[3][2].layout_size = EINA_SIZE2D(150, 450);
   hints3[3][0].expected = EINA_RECT(50, 0, 50, 150);
   hints3[3][1].expected = EINA_RECT(50, 150, 50, 150);
   hints3[3][2].expected = EINA_RECT(50, 300, 50, 150);

   btn_hint_set(btn, &hints3[3][0]);
   btn_hint_set(btn2, &hints3[3][1]);
   btn_hint_set(btn3, &hints3[3][2]);
   btn_geom_assert(&hints3[3][0], efl_gfx_entity_geometry_get(btn));
   btn_geom_assert(&hints3[3][1], efl_gfx_entity_geometry_get(btn2));
   btn_geom_assert(&hints3[3][2], efl_gfx_entity_geometry_get(btn3));

   efl_ui_layout_orientation_set(layout, EFL_UI_LAYOUT_ORIENTATION_HORIZONTAL);
   hints3[3][0].layout_expected = hints3[3][0].layout_size = EINA_SIZE2D(300, 900);
   hints3[3][1].layout_expected = hints3[3][1].layout_size = EINA_SIZE2D(300, 900);
   hints3[3][2].layout_expected = hints3[3][2].layout_size = EINA_SIZE2D(300, 900);
   hints3[3][0].expected = EINA_RECT(0, 300, 100, 300);
   hints3[3][1].expected = EINA_RECT(100, 300, 100, 300);
   hints3[3][2].expected = EINA_RECT(200, 300, 100, 300);

   btn_hint_set(btn, &hints3[3][0]);
   btn_hint_set(btn2, &hints3[3][1]);
   btn_hint_set(btn3, &hints3[3][2]);
   btn_geom_assert(&hints3[3][0], efl_gfx_entity_geometry_get(btn));
   btn_geom_assert(&hints3[3][1], efl_gfx_entity_geometry_get(btn2));
   btn_geom_assert(&hints3[3][2], efl_gfx_entity_geometry_get(btn3));

   hints3[3][0].layout_expected = hints3[3][0].layout_size = EINA_SIZE2D(150, 450);
   hints3[3][1].layout_expected = hints3[3][1].layout_size = EINA_SIZE2D(150, 450);
   hints3[3][2].layout_expected = hints3[3][2].layout_size = EINA_SIZE2D(150, 450);
   hints3[3][0].expected = EINA_RECT(0, 150, 50, 150);
   hints3[3][1].expected = EINA_RECT(50, 150, 50, 150);
   hints3[3][2].expected = EINA_RECT(100, 150, 50, 150);

   btn_hint_set(btn, &hints3[3][0]);
   btn_hint_set(btn2, &hints3[3][1]);
   btn_hint_set(btn3, &hints3[3][2]);
   btn_geom_assert(&hints3[3][0], efl_gfx_entity_geometry_get(btn));
   btn_geom_assert(&hints3[3][1], efl_gfx_entity_geometry_get(btn2));
   btn_geom_assert(&hints3[3][2], efl_gfx_entity_geometry_get(btn3));
}
EFL_END_TEST

EFL_START_TEST (efl_ui_box_flow_size)
{
#define USERMIN_CHECK(a, b) \
   efl_canvas_group_calculate(layout); \
   user_min = efl_gfx_hint_size_min_get(layout); \
   ck_assert_msg(COORD_EQ(user_min.w, (a)) && COORD_EQ(user_min.h, (b)), \
                 "Case box_size failed... user_min: (%d, %d) expected user_min: (%d, %d)", \
                 user_min.w, user_min.h, (a), (b));

#define MIN_CHECK(a, b) \
   efl_canvas_group_calculate(layout); \
   min = efl_gfx_hint_size_combined_min_get(layout); \
   ck_assert_msg(COORD_EQ(min.w, (a)) && COORD_EQ(min.h, (b)), \
                 "Case box_size failed... min: (%d, %d) expected min: (%d, %d)", \
                 min.w, min.h, (a), (b));

   Eo *btn, *btn2, *btn3;
   Eina_Size2D min, user_min;

   btn = efl_add(EFL_UI_BUTTON_CLASS, layout,
                 efl_gfx_hint_size_min_set(efl_added, EINA_SIZE2D(100, 100)),
                 efl_pack_end(layout, efl_added));

   USERMIN_CHECK(0, 0);
   MIN_CHECK(100, 100);

   btn2 = efl_add(EFL_UI_BUTTON_CLASS, layout,
                  efl_gfx_hint_size_min_set(efl_added, EINA_SIZE2D(100, 100)),
                  efl_pack_end(layout, efl_added));
   btn3 = efl_add(EFL_UI_BUTTON_CLASS, layout,
                  efl_gfx_hint_size_min_set(efl_added, EINA_SIZE2D(100, 100)),
                  efl_pack_end(layout, efl_added));
   USERMIN_CHECK(0, 0);
   MIN_CHECK(300, 100);

   efl_pack_unpack(layout, btn2);
   USERMIN_CHECK(0, 0);
   MIN_CHECK(200, 100);

   efl_pack_unpack(layout, btn3);
   USERMIN_CHECK(0, 0);
   MIN_CHECK(100, 100);

   efl_pack_unpack(layout, btn);
   USERMIN_CHECK(0, 0);
   MIN_CHECK(0, 0);

   efl_pack_end(layout, btn);
   efl_gfx_hint_size_min_set(layout, EINA_SIZE2D(200, 200));
   USERMIN_CHECK(200, 200);
   MIN_CHECK(200, 200);

   efl_pack_end(layout, btn2);
   efl_pack_end(layout, btn3);
   USERMIN_CHECK(200, 200);
   MIN_CHECK(300, 200);

#undef USERMIN_ASSERT
#undef MIN_ASSERT
}
EFL_END_TEST

void efl_ui_test_box_flow(TCase *tc)
{
   tcase_add_checked_fixture(tc, layout_setup, NULL);
   tcase_add_test(tc, efl_ui_box_flow_class_check);
   tcase_add_test(tc, efl_ui_box_flow_layout_update);
   tcase_add_test(tc, efl_ui_box_flow_layout_update_pack);
   tcase_add_test(tc, efl_ui_box_flow_size);
}
