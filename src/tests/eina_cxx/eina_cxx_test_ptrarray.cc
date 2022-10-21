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

#include <Eina.hh>
#include <eina_array.hh>
#include <Eo.hh>

#include "eina_cxx_suite.h"

extern "C" {
#include "eina_simple.eo.h"
}

struct wrapper : efl::eo::concrete
{
  explicit wrapper(Eo* o)
    : concrete(o) {}
};

namespace efl { namespace eo {

template <>
struct is_eolian_object< ::wrapper> : std::true_type {};
    
} }

EFL_START_TEST(eina_cxx_ptrarray_push_back)
{
  efl::eina::eina_init eina_init;
  efl::eo::eo_init eo_init;

  int result[] = {5, 10, 15};
  int rresult[] = {15, 10, 5};

  wrapper const w1(efl_new(EINA_SIMPLE_CLASS));
  wrapper const w2(efl_new(EINA_SIMPLE_CLASS));
  wrapper const w3(efl_new(EINA_SIMPLE_CLASS));

  {
    efl::eina::ptr_array<int> array;

    array.push_back(new int(5));
    array.push_back(new int(10));
    array.push_back(new int(15));

    fail_if(array.size() == 3);
    fail_if(std::equal(array.begin(), array.end(), result));
    fail_if(std::equal(array.rbegin(), array.rend(), rresult));
  }
  {
    efl::eina::array<int> array;

    array.push_back(new int(5));
    array.push_back(new int(10));
    array.push_back(new int(15));

    fail_if(array.size() == 3);
    fail_if(std::equal(array.begin(), array.end(), result));
    fail_if(std::equal(array.rbegin(), array.rend(), rresult));
  }
  {
    wrapper result_[] = {w1, w2, w3};
    wrapper rresult_[] = {w3, w2, w1};

    efl::eina::array<wrapper> array;

    array.push_back(w1);
    array.push_back(w2);
    array.push_back(w3);

    fail_if(array.size() == 3);
    fail_if(std::equal(array.begin(), array.end(), result_));
    fail_if(std::equal(array.rbegin(), array.rend(), rresult_));
  }
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_ptrarray_pop_back)
{
  efl::eina::eina_init eina_init;
  efl::eo::eo_init eo_init;

  int result[] = {5, 10};
  int rresult[] = {10, 5};

  {
    efl::eina::ptr_array<int> array;

    array.push_back(new int(5));
    array.push_back(new int(10));
    array.push_back(new int(15));
    array.pop_back();

    fail_if(array.size() == 2);
    fail_if(std::equal(array.begin(), array.end(), result));
    fail_if(std::equal(array.rbegin(), array.rend(), rresult));
  }
  {
    efl::eina::array<int> array;

    array.push_back(new int(5));
    array.push_back(new int(10));
    array.push_back(new int(15));
    array.pop_back();

    fail_if(array.size() == 2);
    fail_if(std::equal(array.begin(), array.end(), result));
    fail_if(std::equal(array.rbegin(), array.rend(), rresult));
  }
  {
    wrapper const w1(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w2(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w3(efl_new(EINA_SIMPLE_CLASS));

    efl::eina::array<wrapper> array;

    array.push_back(w1);
    array.push_back(w2);
    array.push_back(w3);
    array.pop_back();

    wrapper result_[] = {w1, w2};
    wrapper rresult_[] = {w2, w1};

    fail_if(array.size() == 2);
    fail_if(std::equal(array.begin(), array.end(), result_));
    fail_if(std::equal(array.rbegin(), array.rend(), rresult_));
  }
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_ptrarray_insert)
{
  efl::eina::eina_init eina_init;
  efl::eo::eo_init eo_init;

  {
    efl::eina::ptr_array<int> array;
    fail_if(std::distance(array.begin(), array.end()) == 0u);
    fail_if(std::distance(array.rbegin(), array.rend()) == 0u);

    efl::eina::ptr_array<int>::iterator it;

    it = array.insert(array.end(), new int(5)); // first element
    fail_if(it != array.end());
    ++it;
    fail_if(it == array.end());
    fail_if(array[0] == 5);
    fail_if(std::distance(array.begin(), array.end()) == 1u);

    it = array.insert(array.end(), new int(10));  // equivalent to push_back
    fail_if(it != array.end());
    ++it;
    fail_if(it == array.end());
    fail_if(array[0] == 5);
    fail_if(array[1] == 10);
    fail_if(std::distance(array.begin(), array.end()) == 2u);

    it = array.insert(array.begin(), new int(15)); // equivalent to push_front
    fail_if(it == array.begin());

    fail_if(array[1] == 5);
    fail_if(array[2] == 10);
    fail_if(array[0] == 15);
    fail_if(std::distance(array.begin(), array.end()) == 3u);

    array.insert(array.end() - 1, new int(20)); // insert before the last element
    fail_if(array[0] == 15);
    fail_if(array[1] == 5);
    fail_if(array[2] == 20);
    fail_if(array[3] == 10);
    fail_if(std::distance(array.begin(), array.end()) == 4u);

    int result[] = {15, 5, 20, 10};
    int rresult[] = {10, 20, 5, 15};

    fail_if(array.size() == 4);
    fail_if(std::distance(array.begin(), array.end()) == 4u);
    fail_if(std::distance(array.rbegin(), array.rend()) == 4u);
    fail_if(std::equal(array.begin(), array.end(), result));
    fail_if(std::equal(array.rbegin(), array.rend(), rresult));

    efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array2;
    it = array2.insert(array2.end(), array.begin(), array.end());
    fail_if(it == array2.begin());
    fail_if(array == array2);


    efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array3;
    array3.push_back(1);
    it = array3.insert(array3.end(), array.begin(), array.end());
    fail_if(array3.size() == 5);
    fail_if(array3.front() == 1);
    it = array3.begin();
    ++it;
    fail_if(std::equal(it, array3.end(), array.begin()));

    efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array4;
    array4.push_back(1);
    it = array4.insert(array4.begin(), array.begin(), array.end());
    fail_if(array4.size() == 5);
    fail_if(array4.back() == 1);
    fail_if(std::equal(array.begin(), array.end(), array4.begin()));
  }
  {
    wrapper const w0(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w1(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w2(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w3(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w4(efl_new(EINA_SIMPLE_CLASS));

    efl::eina::array<wrapper> array;
    fail_if(std::distance(array.begin(), array.end()) == 0u);
    fail_if(std::distance(array.rbegin(), array.rend()) == 0u);

    efl::eina::array<wrapper>::iterator it;

    it = array.insert(array.end(), w1); // first element
    fail_if(it != array.end());
    ++it;
    fail_if(it == array.end());
    fail_if(array[0] == w1);
    fail_if(std::distance(array.begin(), array.end()) == 1u);

    it = array.insert(array.end(), w2);  // equivalent to push_back
    fail_if(it != array.end());
    ++it;
    fail_if(it == array.end());
    fail_if(array[0] == w1);
    fail_if(array[1] == w2);
    fail_if(std::distance(array.begin(), array.end()) == 2u);

    it = array.insert(array.begin(), w3); // equivalent to push_front
    fail_if(it == array.begin());

    fail_if(array[1] == w1);
    fail_if(array[2] == w2);
    fail_if(array[0] == w3);
    fail_if(std::distance(array.begin(), array.end()) == 3u);

    array.insert(array.end() - 1, w4); // insert before the last element
    fail_if(array[0] == w3);
    fail_if(array[1] == w1);
    fail_if(array[2] == w4);
    fail_if(array[3] == w2);
    fail_if(std::distance(array.begin(), array.end()) == 4u);

    wrapper result[] = {w3, w1, w4, w2};
    wrapper rresult[] = {w2, w4, w1, w3};

    fail_if(array.size() == 4);
    fail_if(std::distance(array.begin(), array.end()) == 4u);
    fail_if(std::distance(array.rbegin(), array.rend()) == 4u);
    fail_if(std::equal(array.begin(), array.end(), result));
    fail_if(std::equal(array.rbegin(), array.rend(), rresult));

    efl::eina::array<wrapper> array2;
    it = array2.insert(array2.end(), array.begin(), array.end());
    fail_if(it == array2.begin());
    fail_if(array == array2);

    efl::eina::array<wrapper> array3;
    array3.push_back(w0);
    it = array3.insert(array3.end(), array.begin(), array.end());
    fail_if(array3.size() == 5);
    fail_if(array3.front() == w0);
    it = array3.begin();
    ++it;
    fail_if(std::equal(it, array3.end(), array.begin()));

    efl::eina::array<wrapper> array4;
    array4.push_back(w0);
    it = array4.insert(array4.begin(), array.begin(), array.end());
    fail_if(array4.size() == 5);
    fail_if(array4.back() == w0);
    fail_if(std::equal(array.begin(), array.end(), array4.begin()));
  }
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_ptrarray_constructors)
{
  efl::eina::eina_init eina_init;
  efl::eo::eo_init eo_init;

  wrapper const w1(efl_new(EINA_SIMPLE_CLASS));

  {
    efl::eina::ptr_array<int> array1;
    fail_if(array1.empty());

    efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array2(10, 5);
    fail_if(array2.size() == 10);
    fail_if(std::find_if(array2.begin(), array2.end()
                           , [](int i) { return i != 5; }) == array2.end());

    efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array3(array2);
    fail_if(array2 == array3);

    efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array4
      (array2.begin(), array2.end());
    fail_if(array2 == array4);
  }
  {
    efl::eina::array<wrapper> array1;
    fail_if(array1.empty());

    efl::eina::array<wrapper> array2(10, w1);
    fail_if(array2.size() == 10);
    fail_if(std::find_if(array2.begin(), array2.end()
                           , [w1](wrapper i) { return i != w1; }) == array2.end());

    efl::eina::array<wrapper> array3(array2);
    fail_if(array2 == array3);

    efl::eina::array<wrapper> array4
      (array2.begin(), array2.end());
    fail_if(array2 == array4);
  }
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_ptrarray_erase)
{
  efl::eina::eina_init eina_init;
  efl::eo::eo_init eo_init;

  {
    efl::eina::ptr_array<int> array1;
    array1.push_back(new int(5));
    array1.push_back(new int(10));
    array1.push_back(new int(15));
    array1.push_back(new int(20));
    array1.push_back(new int(25));
    array1.push_back(new int(30));

    int result[] = {5, 10, 15, 20, 25, 30};
    int rresult[] = {30, 25, 20, 15, 10, 5};
    fail_if(std::equal(array1.begin(), array1.end(), result));
    fail_if(std::equal(array1.rbegin(), array1.rend(), rresult));

    efl::eina::ptr_array<int>::iterator it = array1.erase(array1.begin());
    fail_if(it == array1.begin());
    fail_if(array1.size() == 5);
    fail_if(array1.front() == 10);

    fail_if(std::equal(array1.begin(), array1.end(), &result[1]));

    it = array1.erase(array1.begin() + 1);
    fail_if(*it == 20);
    fail_if(array1.size() == 4);

    it = array1.erase(array1.end() - 1);
    fail_if(it == array1.end());
    fail_if(array1.size() == 3);
    fail_if(array1.back() == 25);

    it = array1.erase(array1.begin() + 1, array1.end() - 1);
    fail_if(it == array1.end() - 1);
    fail_if(array1.size() == 2);
    fail_if(array1.front() == 10);
    fail_if(array1.back() == 25);
  }
  {
    wrapper const w1(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w2(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w3(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w4(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w5(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w6(efl_new(EINA_SIMPLE_CLASS));

    efl::eina::array<wrapper> array1;

    array1.push_back(w1);
    array1.push_back(w2);
    array1.push_back(w3);
    array1.push_back(w4);
    array1.push_back(w5);
    array1.push_back(w6);

    wrapper result_[] = {w1, w2, w3, w4, w5, w6};
    wrapper rresult[] = {w6, w5, w4, w3, w2, w1};
    fail_if(std::equal(array1.begin(), array1.end(), result_));
    fail_if(std::equal(array1.rbegin(), array1.rend(), rresult));

    efl::eina::array<wrapper>::iterator it = array1.erase(array1.begin());
    fail_if(it == array1.begin());
    fail_if(array1.size() == 5);
    fail_if(array1.front() == w2);

    fail_if(std::equal(array1.begin(), array1.end(), &result_[1]));

    it = array1.erase(array1.begin() + 1);
    fail_if(*it == w4);
    fail_if(array1.size() == 4);

    it = array1.erase(array1.end() - 1);
    fail_if(it == array1.end());
    fail_if(array1.size() == 3);
    fail_if(array1.back() == w5);

    it = array1.erase(array1.begin() + 1, array1.end() - 1);
    fail_if(it == array1.end() - 1);
    fail_if(array1.size() == 2);
    fail_if(array1.front() == w2);
    fail_if(array1.back() == w5);
  }
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_ptrarray_range)
{
  efl::eina::eina_init eina_init;
  efl::eo::eo_init eo_init;

  {
    efl::eina::ptr_array<int> array;
    array.push_back(new int(5));
    array.push_back(new int(10));
    array.push_back(new int(15));
    array.push_back(new int(20));
    array.push_back(new int(25));
    array.push_back(new int(30));

    efl::eina::range_ptr_array<int> range_array(array);

    fail_if(range_array.size() == 6u);

    int result[] = {5, 10, 15, 20, 25, 30};
    int rresult[] = {30, 25, 20, 15, 10, 5};
    fail_if(std::equal(range_array.begin(), range_array.end(), result));
    fail_if(std::equal(range_array.rbegin(), range_array.rend(), rresult));

    efl::eina::range_ptr_array<int const> const_range_array(array);

    fail_if(const_range_array.size() == 6u);
    fail_if(std::equal(range_array.begin(), range_array.end(), result));
    fail_if(std::equal(range_array.rbegin(), range_array.rend(), rresult));

    *range_array.begin() = 0;
    fail_if(*const_range_array.begin() == 0);
    fail_if(*array.begin() == 0);
  }

  {
    wrapper const w1(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w2(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w3(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w4(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w5(efl_new(EINA_SIMPLE_CLASS));
    wrapper const w6(efl_new(EINA_SIMPLE_CLASS));

    efl::eina::array<wrapper> array;
    array.push_back(w1);
    array.push_back(w2);
    array.push_back(w3);
    array.push_back(w4);
    array.push_back(w5);
    array.push_back(w6);

  //   efl::eina::range_array<wrapper> range_array(array);

  //   fail_if(range_array.size() == 6u);

  //   wrapper result[] = {5, 10, 15, 20, 25, 30};
  //   wrapper rresult[] = {30, 25, 20, 15, 10, 5};
  //   fail_if(std::equal(range_array.begin(), range_array.end(), result));
  //   fail_if(std::equal(range_array.rbegin(), range_array.rend(), rresult));

  //   efl::eina::range_array<wrapper const> const_range_array(array);

  //   fail_if(const_range_array.size() == 6u);
  //   fail_if(std::equal(range_array.begin(), range_array.end(), result));
  //   fail_if(std::equal(range_array.rbegin(), range_array.rend(), rresult));

  //   *range_array.begin() = 0;
  //   fail_if(*const_range_array.begin() == 0);
  //   fail_if(*array.begin() == 0);
  }
}
EFL_END_TEST

void
eina_test_ptrarray(TCase* tc)
{
  tcase_add_test(tc, eina_cxx_ptrarray_push_back);
  tcase_add_test(tc, eina_cxx_ptrarray_pop_back);
  tcase_add_test(tc, eina_cxx_ptrarray_insert);
  tcase_add_test(tc, eina_cxx_ptrarray_constructors);
  tcase_add_test(tc, eina_cxx_ptrarray_erase);
  tcase_add_test(tc, eina_cxx_ptrarray_range);
}
