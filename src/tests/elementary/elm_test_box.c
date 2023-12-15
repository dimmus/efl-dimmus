#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_BETA
#include <Elementary.h>
#include "elm_suite.h"

EFL_START_TEST(elm_box_legacy_type_check)
{
   Evas_Object *win, *box;
   const char *type;

   win = win_add(NULL, "box", ELM_WIN_BASIC);

   box = elm_box_add(win);

   type = elm_object_widget_type_get(box);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "Elm_Box"));

   type = evas_object_type_get(box);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "elm_box"));

}
EFL_END_TEST

EFL_START_TEST(elm_atspi_role_get)
{
   Evas_Object *win, *box;
   Efl_Access_Role role;

   win = win_add(NULL, "box", ELM_WIN_BASIC);

   box = elm_box_add(win);
   role = efl_access_object_role_get(box);

   ck_assert(role == EFL_ACCESS_ROLE_FILLER);

}
EFL_END_TEST

EFL_START_TEST(elm_box_disabled_test)
{
   Evas_Object *win, *box, *o;
   int i;

   win = win_add(NULL, "box", ELM_WIN_BASIC);

   box = elm_box_add(win);
   elm_win_resize_object_add(win, box);

   for (i = 0; i < 3; ++i)
     {
        o = elm_button_add(box);
        elm_box_pack_end(box, o);
     }

   elm_object_disabled_set(box, EINA_TRUE);
   elm_object_disabled_set(box, EINA_TRUE);
   ck_assert_int_eq(elm_object_disabled_get(box), EINA_TRUE);
   ck_assert_int_eq(elm_object_disabled_get(o), EINA_TRUE);
   elm_object_disabled_set(box, EINA_FALSE);
   ck_assert_int_eq(elm_object_disabled_get(box), EINA_FALSE);
   ck_assert_int_eq(elm_object_disabled_get(o), EINA_FALSE);
   elm_object_disabled_set(box, EINA_TRUE);
   elm_object_disabled_set(o, EINA_TRUE);
   ck_assert_int_eq(elm_object_disabled_get(box), EINA_TRUE);
   ck_assert_int_eq(elm_object_disabled_get(o), EINA_TRUE);
   elm_object_disabled_set(box, EINA_FALSE);
   ck_assert_int_eq(elm_object_disabled_get(box), EINA_FALSE);
   ck_assert_int_eq(elm_object_disabled_get(o), EINA_TRUE);
}
EFL_END_TEST

void elm_test_box(TCase *tc)
{
   tcase_add_test(tc, elm_box_legacy_type_check);
   tcase_add_test(tc, elm_atspi_role_get);
   tcase_add_test(tc, elm_box_disabled_test);
}
