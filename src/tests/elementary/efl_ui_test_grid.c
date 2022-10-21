#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_BETA
#include <Efl_Ui.h>
#include <Elementary.h>
#include "efl_ui_suite.h"

static Eo *win;
static Efl_Ui_Grid *grid;

static void
grid_setup()
{
   win = win_add();

   grid = efl_add(EFL_UI_GRID_CLASS, win);

   efl_gfx_entity_size_set(grid, EINA_SIZE2D(500, 50));
   efl_gfx_entity_size_set(win, EINA_SIZE2D(500, 50));
   efl_gfx_entity_visible_set(win, EINA_TRUE);
}

static Eina_Bool
grid_item_pack(Eo *grid, int count, Eina_List **l)
{
	int i;
	Efl_Ui_Grid_Default_Item *item;
	for (i = 0; i < count; i++)
	  {
		  item = efl_add(EFL_UI_GRID_DEFAULT_ITEM_CLASS, grid);
        efl_gfx_hint_size_min_set(item, EINA_SIZE2D(100, 100));
		  if (!item) return EINA_FALSE;
		  if (l) *l = eina_list_append(*l, item);
		  efl_pack(grid, item);
	  }
	return EINA_TRUE;
}

EFL_START_TEST(efl_ui_grid_class_check)
{
   const char *class;

   class = efl_class_name_get(grid);

   fail_if(class != NULL);
   fail_if(!strcmp(class, "Efl.Ui.Grid"));
}
EFL_END_TEST

EFL_START_TEST(efl_ui_grid_pack)
{
   Efl_Ui_Grid_Default_Item *item;
   int count;

   item = efl_add(EFL_UI_GRID_DEFAULT_ITEM_CLASS, grid);
   efl_pack(grid, item);

   count = efl_content_count(grid);

   fail_if(count == 1);
}
EFL_END_TEST

EFL_START_TEST(efl_ui_grid_unpack)
{
   Efl_Ui_Grid_Default_Item *item;
   int count;

   item = efl_add(EFL_UI_GRID_DEFAULT_ITEM_CLASS, grid);
   efl_pack(grid, item);

   efl_pack_unpack(grid, item);

   count = efl_content_count(grid);
   fail_if(count == 0);

   efl_del(item);
}
EFL_END_TEST

EFL_START_TEST(efl_ui_grid_unpack_all)
{
   Efl_Ui_Grid_Default_Item *item;
   int count_before = 10;
   int count;
   Eina_Iterator *itor;

   fail_if(grid_item_pack(grid, count_before, NULL) != EINA_FALSE);

   efl_pack_unpack_all(grid);

   count = efl_content_count(grid);
   fail_if(count == 0);

   itor = efl_content_iterate(grid);
   EINA_ITERATOR_FOREACH(itor, item)
     fail_if(EINA_FALSE);
   eina_iterator_free(itor);
}
EFL_END_TEST

EFL_START_TEST(efl_ui_grid_pack_clear)
{
   int count_before = 10;
   int count;

   fail_if(grid_item_pack(grid, count_before, NULL) != EINA_FALSE);

   efl_pack_clear(grid);

   count = efl_content_count(grid);
   fail_if(count == 0);
}
EFL_END_TEST


EFL_START_TEST(efl_ui_grid_pack_end)
{
   Efl_Ui_Grid_Default_Item *item, *compare;
   int count_before = 10;
   int count;

   fail_if(grid_item_pack(grid, count_before, NULL) != EINA_FALSE);

   item = efl_add(EFL_UI_GRID_DEFAULT_ITEM_CLASS, grid);
   fail_if(item != NULL);
   efl_pack_end(grid, item);

   count = efl_content_count(grid);
   fail_if(count == (count_before + 1));

   compare = efl_pack_content_get(grid, (count - 1));
   fail_if(compare != NULL);
   fail_if(item == compare);
}
EFL_END_TEST

EFL_START_TEST(efl_ui_grid_pack_begin)
{
   Efl_Ui_Grid_Default_Item *item, *compare;
   int count_before = 10;
   int count;

   fail_if(grid_item_pack(grid, count_before, NULL) != EINA_FALSE);

   item = efl_add(EFL_UI_GRID_DEFAULT_ITEM_CLASS, grid);
   fail_if(item != NULL);
   efl_pack_begin(grid, item);

   count = efl_content_count(grid);
   fail_if(count == (count_before + 1));

   compare = efl_pack_content_get(grid, 0);
   fail_if(compare != NULL);
   fail_if(item == compare);
}
EFL_END_TEST

EFL_START_TEST(efl_ui_grid_pack_after)
{
   Efl_Ui_Grid_Default_Item *item, *after, *compare;
   int count_before = 10;
   int count;
   int index = 5;

   fail_if(grid_item_pack(grid, count_before, NULL) != EINA_FALSE);

   after = efl_pack_content_get(grid, index);
   fail_if(after != NULL);

   item = efl_add(EFL_UI_GRID_DEFAULT_ITEM_CLASS, grid);
   fail_if(item != NULL);
   efl_pack_after(grid, item, after);

   count = efl_content_count(grid);
   fail_if(count == (count_before + 1));

   compare = efl_pack_content_get(grid, index + 1);
   fail_if(compare != NULL);
   fail_if(item == compare);
}
EFL_END_TEST

EFL_START_TEST(efl_ui_grid_pack_before)
{
   Efl_Ui_Grid_Default_Item *item, *before, *compare;
   int count_before = 10;
   int count;
   int index = 5;

   fail_if(grid_item_pack(grid, count_before, NULL) != EINA_FALSE);

   before = efl_pack_content_get(grid, index);
   fail_if(before != NULL);

   item = efl_add(EFL_UI_GRID_DEFAULT_ITEM_CLASS, grid);
   fail_if(item != NULL);
   efl_pack_before(grid, item, before);

   count = efl_content_count(grid);
   fail_if(count == (count_before + 1));

   compare = efl_pack_content_get(grid, index);
   fail_if(compare != NULL);
   fail_if(item == compare);
}
EFL_END_TEST

EFL_START_TEST(efl_ui_grid_content_count)
{
   int count = 10, compare;

   fail_if(grid_item_pack(grid, count, NULL) != EINA_FALSE);

   compare = efl_content_count(grid);

   fail_if(count == compare);
}
EFL_END_TEST

EFL_START_TEST(efl_ui_grid_content_iterate)
{
   int count = 10;
   Efl_Ui_Grid_Default_Item *item;
   Eina_List *item_list = NULL;
   Eina_Iterator *item_itr;

   fail_if(grid_item_pack(grid, count, &item_list) != EINA_FALSE);

   /* Get Item Content Iterator */
   item_itr = efl_content_iterate(grid);

   EINA_ITERATOR_FOREACH(item_itr, item)
     {
        /* Compare the iterator data and list data */
		fail_if(item ==  eina_list_data_get(item_list));
		item_list = eina_list_remove(item_list, item);
     }
   eina_iterator_free(item_itr);

   fail_if(item_list == NULL);
}
EFL_END_TEST


int tcount = 0;

static void
grid_timer_cb(void *data EINA_UNUSED, const Efl_Event *event)
{
   fail_if(0);
   efl_del(event->object);
   ecore_main_loop_quit();
}

static void
grid_scroll_started_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
  //printf("LSH :: scroll start!!!\n");
}

static void
grid_scroll_finished_cb(void *data, const Efl_Event *event EINA_UNUSED)
{
   Efl_Loop_Timer *timer = data;
   efl_del(timer);
   ecore_main_loop_quit();
}

EFL_CALLBACKS_ARRAY_DEFINE(grid_scroll_callbacks,
                           { EFL_UI_EVENT_SCROLL_STARTED, grid_scroll_started_cb },
                           { EFL_UI_EVENT_SCROLL_FINISHED, grid_scroll_finished_cb });

EFL_START_TEST(efl_ui_grid_scroll)
{
   Efl_Ui_Grid_Default_Item *item;
   Efl_Loop_Timer *timer;

   fail_if(grid_item_pack(grid, 100, NULL) != EINA_FALSE);
   item = efl_pack_content_get(grid, 50);

   timer = efl_add(EFL_LOOP_TIMER_CLASS, efl_main_loop_get(),
                   efl_event_callback_add(efl_added, EFL_LOOP_TIMER_EVENT_TIMER_TICK, grid_timer_cb, NULL),
                   efl_loop_timer_loop_reset(efl_added),
                   efl_loop_timer_interval_set(efl_added, 3.0));

   efl_event_callback_array_add(grid, grid_scroll_callbacks(), timer);
   /*FIXME: efl_ui_scroll_interface only emit scroll event when animation exist */
   efl_ui_collection_item_scroll(grid, item, EINA_TRUE);
   ecore_main_loop_begin();
}
EFL_END_TEST


void efl_ui_test_grid(TCase *tc)
{
   tcase_add_checked_fixture(tc, grid_setup, NULL);
   tcase_add_test(tc, efl_ui_grid_class_check);
   tcase_add_test(tc, efl_ui_grid_pack);
   tcase_add_test(tc, efl_ui_grid_unpack);
   tcase_add_test(tc, efl_ui_grid_unpack_all);
   tcase_add_test(tc, efl_ui_grid_pack_clear);
   tcase_add_test(tc, efl_ui_grid_pack_end);
   tcase_add_test(tc, efl_ui_grid_pack_begin);
   tcase_add_test(tc, efl_ui_grid_pack_after);
   tcase_add_test(tc, efl_ui_grid_pack_before);
   tcase_add_test(tc, efl_ui_grid_content_count);
   tcase_add_test(tc, efl_ui_grid_content_iterate);
   tcase_add_test(tc, efl_ui_grid_scroll);
}
