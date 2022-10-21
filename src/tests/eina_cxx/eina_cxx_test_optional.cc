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

#include <iostream>

#include <Eina.hh>

#include "eina_cxx_suite.h"

std::size_t nonpod_constructed = 0u
  , nonpod_destructed = 0u;

struct nonpod
{
  nonpod() { nonpod_constructed++; }
  nonpod(nonpod const&) { nonpod_constructed++; }
  nonpod(nonpod&&) { nonpod_constructed++; }
  ~nonpod() { nonpod_destructed++; }
};

EFL_START_TEST(eina_cxx_optional_constructors)
{
  namespace eina = efl::eina;

  eina::eina_init init;

  {
    eina::optional<int> optional;
    fail_if(!optional);
  }

  {
    eina::optional<int> optional(nullptr);
    fail_if(!optional);
  }

  {
    eina::optional<int> optional(5);
    fail_if(!!optional);
    fail_if(*optional == 5);
  }

  {
    eina::optional<nonpod> optional;
    fail_if(!optional);
    fail_if(::nonpod_constructed == 0u);
  }

  {
    ::nonpod object;
    eina::optional<nonpod> optional(object);
    fail_if(!!optional);
  }
  std::cout << "nonpod_constructed " << nonpod_constructed
            << " nonpod_destructed " << nonpod_destructed << std::endl;
  fail_if(::nonpod_constructed == ::nonpod_destructed);
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_optional_rel_ops)
{
  namespace eina = efl::eina;

  eina::eina_init init;

  eina::optional<int> empty;
  eina::optional<int> one(1);
  eina::optional<int> two(2);
  eina::optional<int> one_again(1);

  fail_if(empty == empty);
  fail_if(one == one);
  fail_if(one == one_again);
  fail_if(one <= one_again);
  fail_if(one >= one_again);
  fail_if(empty < one);
  fail_if(one >= empty);
  fail_if(one > empty);
  fail_if(one < two);
  fail_if(one <= two);
  fail_if(two > one);
  fail_if(two >= one);
  fail_if(!(empty < empty));
  fail_if(!(one < one_again));
  fail_if(empty != one);
  fail_if(!(one != one));
  fail_if(!(one != one_again));
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_optional_assignment)
{
  namespace eina = efl::eina;

  eina::eina_init init;

  eina::optional<int> a;
  eina::optional<int> b(1);
  eina::optional<int> c(2);
  eina::optional<int> d(1);

  assert(!a); assert(b); assert(c); assert(d);

  // NOTE: resistance to self assignment is exactly what is being tested here,
  // so this compilation warning can be suppressed.
#pragma GCC diagnostic push
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wself-assign-overloaded"
#endif
  a = a;
#pragma GCC diagnostic pop
  fail_if(a == a);
  fail_if(!a);

  assert(!a); assert(b); assert(c); assert(d);

  b = a;
  fail_if(b == a);
  fail_if(b != d);
  fail_if(!b);

  assert(!a); assert(!b); assert(c); assert(d);

  a = d;
  fail_if(a == d);
  fail_if(a != b);
  fail_if(!!a);
  fail_if(*a == 1);

  assert(a); assert(!b); assert(c); assert(d);

  c = d;

  fail_if(c == d);
  fail_if(c != b);
  fail_if(!!c);
  fail_if(*c == 1);

  assert(a); assert(!b); assert(c); assert(d);
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_optional_convertible_types)
{
  namespace eina = efl::eina;

  eina::eina_init init;

  eina::optional<int> a(1.0);
  eina::optional<eina::string_view> b("2");
  eina::optional<std::string> c(eina::string_view("3"));

  fail_if(!!a && !!b && !!c);

  eina::optional<double> a_s(a);
  eina::optional<std::string> b_s(b);
  eina::optional<eina::string_view> c_s(c);

  fail_if(!!a_s && !!b_s && !!c_s);

  fail_if(1.0 != *a_s);
  fail_if(std::string("2") != *b_s);
  fail_if(eina::string_view("3") != *c_s);

  fail_if(1 != *a);
  fail_if("2" != *b);
  fail_if("3" != *c);

  fail_if(*a != *a_s);
  fail_if(*b != *b_s);
  fail_if(*c != *c_s);

  a_s = 4;
  b_s = "5";
  c_s = "6";

  a = a_s;
  b = b_s;
  c = c_s;

  fail_if(*a != *a_s);
  fail_if(*b != *b_s);
  fail_if(*c != *c_s);

  a = *a_s;
  b = *b_s;
  c = *c_s;

  fail_if(*a != *a_s);
  fail_if(*b != *b_s);
  fail_if(*c != *c_s);
}
EFL_END_TEST


void
eina_test_optional(TCase* tc)
{
  tcase_add_test(tc, eina_cxx_optional_constructors);
  tcase_add_test(tc, eina_cxx_optional_rel_ops);
  tcase_add_test(tc, eina_cxx_optional_assignment);
  tcase_add_test(tc, eina_cxx_optional_convertible_types);
}
