#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_BETA
#include <Elementary.h>
#include "elm_suite.h"

EFL_START_TEST(elm_fileselector_entry_legacy_type_check)
{
   Evas_Object *win, *fileselector_entry;
   const char *type;

   win = win_add(NULL, "fileselector_entry", ELM_WIN_BASIC);

   fileselector_entry = elm_fileselector_entry_add(win);

   type = elm_object_widget_type_get(fileselector_entry);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "Elm_Fileselector_Entry"));

   type = evas_object_type_get(fileselector_entry);
   ck_assert(type != NULL);
   ck_assert(!strcmp(type, "elm_fileselector_entry"));

}
EFL_END_TEST

EFL_START_TEST(elm_atspi_role_get)
{
   Evas_Object *win, *fs_entry;
   Efl_Access_Role role;

   win = win_add(NULL, "fileselectorentry", ELM_WIN_BASIC);

   fs_entry = elm_fileselector_entry_add(win);
   role = efl_access_object_role_get(fs_entry);

   ck_assert(role == EFL_ACCESS_ROLE_GROUPING);

}
EFL_END_TEST

void elm_test_fileselector_entry(TCase *tc)
{
   tcase_add_test(tc, elm_fileselector_entry_legacy_type_check);
   tcase_add_test(tc, elm_atspi_role_get);
}
