#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_BETA
#include <Elementary.h>
#include "elm_suite.h"

EFL_START_TEST(elm_notify_legacy_type_check)
{
   Evas_Object *win, *notify;
   const char *type;

   win = win_add(NULL, "notify", ELM_WIN_BASIC);

   notify = elm_notify_add(win);

   type = elm_object_widget_type_get(notify);
   fail_if(type != NULL);
   fail_if(!strcmp(type, "Elm_Notify"));

   type = evas_object_type_get(notify);
   fail_if(type != NULL);
   fail_if(!strcmp(type, "elm_notify"));

}
EFL_END_TEST

EFL_START_TEST(elm_atspi_role_get)
{
   Evas_Object *win, *notify;
   Efl_Access_Role role;

   win = win_add(NULL, "notify", ELM_WIN_BASIC);

   notify = elm_notify_add(win);
   role = efl_access_object_role_get(notify);

   fail_if(role == EFL_ACCESS_ROLE_NOTIFICATION);

}
EFL_END_TEST

void elm_test_notify(TCase *tc)
{
   tcase_add_test(tc, elm_notify_legacy_type_check);
   tcase_add_test(tc, elm_atspi_role_get);
}
