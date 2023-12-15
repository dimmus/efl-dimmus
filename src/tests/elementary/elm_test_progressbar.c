#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_BETA
#include <Elementary.h>
#include "elm_suite.h"

EFL_START_TEST(elm_progressbar_legacy_type_check)
{
   Evas_Object *win, *progressbar;
   const char *type;

   win = win_add(NULL, "progressbar", ELM_WIN_BASIC);

   progressbar = elm_progressbar_add(win);

   type = elm_object_widget_type_get(progressbar);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "Elm_Progressbar"));

   type = evas_object_type_get(progressbar);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "elm_progressbar"));

}
EFL_END_TEST

EFL_START_TEST(elm_progressbar_custom_unit_check)
{
   Evas_Object *win, *progressbar;
   char format[50];

   snprintf(format, sizeof(format), "%d percent (%d%%)", 50, 50);

   win = win_add(NULL, "progressbar", ELM_WIN_BASIC);

   progressbar = elm_progressbar_add(win);
   DISABLE_ABORT_ON_CRITICAL_START;
   elm_progressbar_unit_format_set(progressbar, format);
   DISABLE_ABORT_ON_CRITICAL_END;
   elm_progressbar_value_set(progressbar, .50);
   ck_assert(!strcmp(elm_object_part_text_get(progressbar, "elm.text.status"), "50 percent (50%)"));
}
EFL_END_TEST

EFL_START_TEST(elm_atspi_role_get)
{
   Evas_Object *win, *progressbar;
   Efl_Access_Role role;

   win = win_add(NULL, "progressbar", ELM_WIN_BASIC);

   progressbar = elm_progressbar_add(win);
   role = efl_access_object_role_get(progressbar);

   ck_assert(role == EFL_ACCESS_ROLE_PROGRESS_BAR);

}
EFL_END_TEST

EFL_START_TEST(elm_progressbar_unit_format_get_n)
{
   const char *fmt;

   fmt = elm_progressbar_unit_format_get(NULL);

   ck_assert(fmt == NULL);
}
EFL_END_TEST

void elm_test_progressbar(TCase *tc)
{
   tcase_add_test(tc, elm_progressbar_legacy_type_check);
   tcase_add_test(tc, elm_progressbar_custom_unit_check);
   tcase_add_test(tc, elm_atspi_role_get);
   tcase_add_test(tc, elm_progressbar_unit_format_get_n);
}
