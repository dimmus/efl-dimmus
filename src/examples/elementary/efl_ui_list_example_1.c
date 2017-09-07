// gcc -o efl_ui_list_example_1 efl_ui_list_example_1.c `pkg-config --cflags --libs elementary`

#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#else
# define EFL_BETA_API_SUPPORT 1
# define EFL_EO_API_SUPPORT 1
#endif

#include <Elementary.h>
#include <Efl.h>
#include <Eio.h>
#include <stdio.h>

#define NUM_ITEMS 200

const char *styles[] = {
        "odd",
        "even"
   };

char edj_path[PATH_MAX];

static void
_realized_cb(void *data, const Efl_Event *event)
{
   Efl_Ui_List_Item_Event *ie = event->info;

   if (!ie->layout) return;

   Efl_Ui_Layout *layout = ie->layout;

   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_obj_widget_focus_allow_set(layout, EINA_TRUE);
}

/*
static void
_unrealized_cb(void *data EINA_UNUSED, const Efl_Event *event)
{
   Efl_Ui_List_Item_Event *ie = event->info;

   efl_ui_view_model_set(ie->layout, NULL);
   efl_del(ie->layout);
}
*/
static Efl_Model*
_make_model()
{
   Eina_Value vtext, vstyle;
   Efl_Model_Item *model, *child;
   unsigned int i, s;
   char buf[256];

   model = efl_add(EFL_MODEL_ITEM_CLASS, NULL);
   eina_value_setup(&vtext, EINA_VALUE_TYPE_STRING);
   eina_value_setup(&vstyle, EINA_VALUE_TYPE_STRING);

   for (i = 0; i < (NUM_ITEMS); i++)
     {
        s = i%2;
        child = efl_model_child_add(model);
        eina_value_set(&vstyle, styles[s]);
        efl_model_property_set(child, "odd_style", &vstyle);

        snprintf(buf, sizeof(buf), "Item # %i", i);
        eina_value_set(&vtext, buf);
        efl_model_property_set(child, "name", &vtext);
     }

   return model;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Efl_Ui_Layout_Factory *factory;
   Evas_Object *win, *li;
   Eo *model;

   win = elm_win_util_standard_add("viewlist", "Viewlist");
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   elm_win_autodel_set(win, EINA_TRUE);

   model = _make_model();

   factory = efl_add(EFL_UI_LAYOUT_FACTORY_CLASS, win);
   efl_ui_model_connect(factory, "signal/elm,state,%v", "odd_style");
   efl_ui_model_connect(factory, "elm.text", "name");
   efl_ui_layout_factory_theme_config(factory, "list", "item", "default");

   li = efl_add(EFL_UI_LIST_CLASS, win);
   efl_ui_list_layout_factory_set(li, factory);
   efl_ui_view_model_set(li, model);

   efl_event_callback_add(li, EFL_UI_LIST_EVENT_ITEM_REALIZED, _realized_cb, NULL);
//   efl_event_callback_add(li, EFL_UI_LIST_EVENT_ITEM_UNREALIZED, _unrealized_cb, NULL);

   elm_win_resize_object_add(win, li);
   evas_object_size_hint_weight_set(li, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   //showall
   evas_object_show(li);
   evas_object_resize(win, 320, 320);
   evas_object_show(win);

   elm_run();

   return 0;
}
ELM_MAIN()
