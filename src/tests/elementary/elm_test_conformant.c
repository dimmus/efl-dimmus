#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_BETA
#include <Elementary.h>
#include "elm_suite.h"

EFL_START_TEST(elm_conformant_legacy_type_check)
{
   Evas_Object *win, *conformant;
   const char *type;

   win = win_add(NULL, "conformant", ELM_WIN_BASIC);

   conformant = elm_conformant_add(win);

   type = elm_object_widget_type_get(conformant);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "Elm_Conformant"));

   type = evas_object_type_get(conformant);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "elm_conformant"));

}
EFL_END_TEST

EFL_START_TEST(elm_atspi_role_get)
{
   Evas_Object *win, *conformant;
   Efl_Access_Role role;

   win = win_add(NULL, "conformant", ELM_WIN_BASIC);

   conformant = elm_conformant_add(win);
   role = efl_access_object_role_get(conformant);

   ck_assert(role == EFL_ACCESS_ROLE_FILLER);

}
EFL_END_TEST

void elm_test_conformant(TCase *tc)
{
   tcase_add_test(tc, elm_conformant_legacy_type_check);
   tcase_add_test(tc, elm_atspi_role_get);
}
