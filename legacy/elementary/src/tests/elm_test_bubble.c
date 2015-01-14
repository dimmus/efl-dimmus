#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define ELM_INTERFACE_ATSPI_ACCESSIBLE_PROTECTED
#include <Elementary.h>
#include "elm_suite.h"

START_TEST (elm_atspi_role_get)
{
   Evas_Object *win, *bubble;
   Elm_Atspi_Role role;

   elm_init(1, NULL);
   win = elm_win_add(NULL, "bubble", ELM_WIN_BASIC);

   bubble = elm_bubble_add(win);
   eo_do(bubble, role = elm_interface_atspi_accessible_role_get());

   ck_assert(role == ELM_ATSPI_ROLE_FILLER);

   elm_shutdown();
}
END_TEST

void elm_test_bubble(TCase *tc)
{
 tcase_add_test(tc, elm_atspi_role_get);
}

