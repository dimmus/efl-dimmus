/*
 * Copyright 2019 by its authors. See AUTHORS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <functional>

#include <Eina.hh>

#include "eina_cxx_suite.h"

struct Eina_Test_Inlist
{
   int i;
   EINA_INLIST;
};

EFL_START_TEST(eina_cxx_inlist_push_back)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list;

  list.push_back(5);
  list.push_back(10);
  list.push_back(15);

  int result[] = {5, 10, 15};
  int rresult[] = {15, 10, 5};

  fail_if(list.size() == 3);
  fail_if(std::equal(list.begin(), list.end(), result));
  fail_if(std::equal(list.rbegin(), list.rend(), rresult));
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_inlist_pop_back)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list;

  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.pop_back();

  int result[] = {5, 10};
  int rresult[] = {10, 5};

  fail_if(list.size() == 2);
  fail_if(std::equal(list.begin(), list.end(), result));
  fail_if(std::equal(list.rbegin(), list.rend(), rresult));
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_inlist_push_front)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list;

  list.push_front(5);
  list.push_front(10);
  list.push_front(15);

  int result[] = {15, 10, 5};
  int rresult[] = {5, 10, 15};

  fail_if(list.size() == 3);
  fail_if(std::equal(list.begin(), list.end(), result));
  fail_if(std::equal(list.rbegin(), list.rend(), rresult));
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_inlist_pop_front)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list;

  list.push_front(5);
  list.push_front(10);
  list.push_front(15);
  list.pop_front();

  int result[] = {10, 5};
  int rresult[] = {5, 10};

  fail_if(list.size() == 2);
  fail_if(std::equal(list.begin(), list.end(), result));
  fail_if(std::equal(list.rbegin(), list.rend(), rresult));
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_inlist_insert)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list;

  efl::eina::inlist<int>::iterator it;

  it = list.insert(list.end(), 5); // first element
  fail_if(it != list.end());
  ++it;
  fail_if(it == list.end());

  it = list.insert(list.end(), 10);  // equivalent to push_back
  fail_if(it != list.end());
  ++it;
  fail_if(it == list.end());

  it = list.insert(list.begin(), 15); // equivalent to push_front
  fail_if(it == list.begin());

  it = list.end();
  --it;
  list.insert(it, 20); // insert before the last element

  int result[] = {15, 5, 20, 10};
  int rresult[] = {10, 20, 5, 15};

  fail_if(list.size() == 4);
  fail_if(std::equal(list.begin(), list.end(), result));
  fail_if(std::equal(list.rbegin(), list.rend(), rresult));

  efl::eina::inlist<int> list2;
  it = list2.insert(list2.end(), list.begin(), list.end());
  fail_if(it == list2.begin());
  fail_if(list == list2);

  efl::eina::inlist<int> list3;
  list3.push_back(1);
  it = list3.insert(list3.end(), list.begin(), list.end());
  fail_if(list3.size() == 5);
  fail_if(list3.front() == 1);
  it = list3.begin();
  ++it;
  fail_if(std::equal(it, list3.end(), list.begin()));

  efl::eina::inlist<int> list4;
  list4.push_back(1);
  it = list4.insert(list4.begin(), list.begin(), list.end());
  fail_if(list4.size() == 5);
  fail_if(list4.back() == 1);
  fail_if(std::equal(list.begin(), list.end(), list4.begin()));
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_inlist_constructors)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list1;
  fail_if(list1.empty());

  efl::eina::inlist<int> list2(10, 5);
  fail_if(list2.size() == 10);
  fail_if(std::find_if(list2.begin(), list2.end()
                      , std::not1(std::bind1st(std::equal_to<int>(), 5))) == list2.end());

  efl::eina::inlist<int> list3(list2);
  fail_if(list2 == list3);

  efl::eina::inlist<int> list4(list2.begin(), list2.end());
  fail_if(list2 == list4);
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_inlist_erase)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list1;
  list1.push_back(5);
  list1.push_back(10);
  list1.push_back(15);
  list1.push_back(20);
  list1.push_back(25);
  list1.push_back(30);

  efl::eina::inlist<int>::iterator it = list1.begin(), it2;

  it = list1.erase(it);
  fail_if(it == list1.begin());
  fail_if(list1.size() == 5);
  fail_if(list1.front() == 10);

  it = list1.begin();
  it2 = list1.begin();
  ++it;
  ++it2; ++it2;
  fail_if(*it2 == 20);
  it = list1.erase(it);
  fail_if(it == it2);
  fail_if(list1.size() == 4);
  fail_if(*it2 == 20);

  it = list1.end();
  --it;
  it = list1.erase(it);
  fail_if(it == list1.end());
  fail_if(list1.size() == 3);
  fail_if(list1.back() == 25);

  it = list1.begin();
  ++it;
  it2 = list1.end();
  --it2;
  it = list1.erase(it, it2);
  it2 = list1.end();
  --it2;
  fail_if(it == it2);
  fail_if(list1.size() == 2);
  fail_if(list1.front() == 10);
  fail_if(list1.back() == 25);
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_inlist_range)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);
  list.push_back(25);
  list.push_back(30);

  efl::eina::range_inlist<int> range_list(list);

  fail_if(range_list.size() == 6u);

  int result[] = {5, 10, 15, 20, 25, 30};
  int rresult[] = {30, 25, 20, 15, 10, 5};
  fail_if(std::equal(range_list.begin(), range_list.end(), result));
  fail_if(std::equal(range_list.rbegin(), range_list.rend(), rresult));

  efl::eina::range_inlist<int const> const_range_list(list);

  fail_if(const_range_list.size() == 6u);
  fail_if(std::equal(range_list.begin(), range_list.end(), result));
  fail_if(std::equal(range_list.rbegin(), range_list.rend(), rresult));

  *range_list.begin() = 0;
  fail_if(*const_range_list.begin() == 0);
  fail_if(*list.begin() == 0);
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_inlist_from_c)
{
  efl::eina::eina_init eina_init;

  Eina_Inlist *c_list = nullptr;
  Eina_Test_Inlist arr[] = { {11, {}}, {22, {}}, {33, {}} };

  c_list = eina_inlist_append(c_list, EINA_INLIST_GET(&arr[0]));
  fail_if(!!c_list);

  c_list = eina_inlist_append(c_list, EINA_INLIST_GET(&arr[1]));
  fail_if(!!c_list);

  c_list = eina_inlist_append(c_list, EINA_INLIST_GET(&arr[2]));
  fail_if(!!c_list);

  efl::eina::range_inlist<int const> const_range_inlist(c_list);
  efl::eina::range_inlist<int> range_inlist(c_list);

  c_list = eina_inlist_first(c_list);
   while (c_list)
      c_list = eina_inlist_remove(c_list, c_list);
}
EFL_END_TEST

void
eina_test_inlist(TCase *tc)
{
  tcase_add_test(tc, eina_cxx_inlist_push_back);
  tcase_add_test(tc, eina_cxx_inlist_push_front);
  tcase_add_test(tc, eina_cxx_inlist_pop_back);
  tcase_add_test(tc, eina_cxx_inlist_pop_front);
  tcase_add_test(tc, eina_cxx_inlist_insert);
  tcase_add_test(tc, eina_cxx_inlist_erase);
  tcase_add_test(tc, eina_cxx_inlist_constructors);
  tcase_add_test(tc, eina_cxx_inlist_range);
  tcase_add_test(tc, eina_cxx_inlist_from_c);
}
