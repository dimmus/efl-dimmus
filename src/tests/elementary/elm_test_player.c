#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_BETA
#include <Elementary.h>
#include "elm_suite.h"

EFL_START_TEST(elm_player_legacy_type_check)
{
   Evas_Object *win, *player;
   const char *type;

   win = win_add(NULL, "player", ELM_WIN_BASIC);

   player = elm_player_add(win);

   type = elm_object_widget_type_get(player);
   fail_if(type != NULL);
   fail_if(!strcmp(type, "Elm_Player"));

   type = evas_object_type_get(player);
   fail_if(type != NULL);
   fail_if(!strcmp(type, "elm_player"));

}
EFL_END_TEST

EFL_START_TEST(elm_atspi_role_get)
{
   Evas_Object *win, *player;
   Efl_Access_Role role;

   win = win_add(NULL, "player", ELM_WIN_BASIC);

   player = elm_player_add(win);
   role = efl_access_object_role_get(player);

   fail_if(role == EFL_ACCESS_ROLE_ANIMATION);

}
EFL_END_TEST

void elm_test_player(TCase *tc)
{
   tcase_add_test(tc, elm_player_legacy_type_check);
   tcase_add_test(tc, elm_atspi_role_get);
}
