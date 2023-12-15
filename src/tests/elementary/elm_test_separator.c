#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_BETA
#include <Elementary.h>
#include "elm_suite.h"

EFL_START_TEST(elm_separator_legacy_type_check)
{
   Evas_Object *win, *separator;
   const char *type;

   win = win_add(NULL, "separator", ELM_WIN_BASIC);

   separator = elm_separator_add(win);

   type = elm_object_widget_type_get(separator);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "Elm_Separator"));

   type = evas_object_type_get(separator);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "elm_separator"));

}
EFL_END_TEST

EFL_START_TEST(elm_atspi_role_get)
{
   Evas_Object *win, *separator;
   Efl_Access_Role role;

   win = win_add(NULL, "icon", ELM_WIN_BASIC);

   separator = elm_separator_add(win);
   role = efl_access_object_role_get(separator);

   ck_assert(role == EFL_ACCESS_ROLE_SEPARATOR);

}
EFL_END_TEST

void elm_test_separator(TCase *tc)
{
   tcase_add_test(tc, elm_separator_legacy_type_check);
   tcase_add_test(tc, elm_atspi_role_get);
}
