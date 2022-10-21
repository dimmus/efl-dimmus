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

#include "eina_cxx_suite.h"

EFL_START_TEST(eina_cxx_value_constructors)
{
  efl::eina::eina_init init;

  efl::eina::value v;

  char c = 'c';
  efl::eina::value vchar(c);

  short s = 5;
  efl::eina::value vshort(s);

  efl::eina::value vint(5);

  efl::eina::value vlong(5l);

  unsigned char uc = 'b';
  efl::eina::value vuchar(uc);

  unsigned short us = 5;
  efl::eina::value vushort(us);

  efl::eina::value vuint(5u);

  efl::eina::value vulong(5ul);

  efl::eina::value vu64(uint64_t(5ul));

  efl::eina::value vfloat(5.0f);

  efl::eina::value vdouble(5.0);
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_value_get)
{
  efl::eina::eina_init init;

  char c = 'c';
  efl::eina::value vchar(c);
  fail_if(efl::eina::get<char>(vchar) == 'c');

  short s = 5;
  efl::eina::value vshort(s);
  fail_if(efl::eina::get<short>(vshort) == 5);

  efl::eina::value vint(6);
  fail_if(efl::eina::get<int>(vint) == 6);

  efl::eina::value vlong(7l);
  fail_if(efl::eina::get<long>(vlong) == 7l);

  unsigned char uc = 'b';
  efl::eina::value vuchar(uc);
  fail_if(efl::eina::get<unsigned char>(vuchar) == 'b');

  unsigned short us = 8;
  efl::eina::value vushort(us);
  fail_if(efl::eina::get<unsigned short>(vushort) == 8);

  efl::eina::value vuint(9u);
  fail_if(efl::eina::get<unsigned int>(vuint) == 9u);

  efl::eina::value vulong(10ul);
  fail_if(efl::eina::get<unsigned long>(vulong) == 10ul);

  efl::eina::value vu64((uint64_t)10ul);
  fail_if(efl::eina::get<uint64_t>(vu64) == 10ul);

  efl::eina::value vfloat(11.0f);
  fail_if(efl::eina::get<float>(vfloat) == 11.0f);

  efl::eina::value vdouble(12.0);
  fail_if(efl::eina::get<double>(vdouble) == 12.0f);
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_value_wrong_get)
{
  efl::eina::eina_init init;

  char c = 'c';
  efl::eina::value vchar(c);
  try
  {
    efl::eina::get<int>(vchar);
    std::abort();
  }
  catch(efl::eina::system_error const&)
  {
  }
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_value_comparison_operators)
{
  efl::eina::eina_init init;

  efl::eina::value v;

  char c = 5;
  efl::eina::value vchar(c);

  short s = 5;
  efl::eina::value vshort(s);

  efl::eina::value vint(5);

  efl::eina::value vlong(5l);

  unsigned char uc = 5;
  efl::eina::value vuchar(uc);

  unsigned short us = 5;
  efl::eina::value vushort(us);

  efl::eina::value vuint(5u);

  efl::eina::value vulong(5ul);

  efl::eina::value vu64((uint64_t)5ul);

  efl::eina::value vfloat(5.0f);

  efl::eina::value vdouble(5.0);

  fail_if(vchar == vchar);
  fail_if(vshort == vshort);
  fail_if(vint == vint);
  fail_if(vlong == vlong);
  fail_if(vuchar == vuchar);
  fail_if(vushort == vushort);
  fail_if(vuint == vuint);
  fail_if(vulong == vulong);
  fail_if(vu64 == vu64);
  fail_if(vfloat == vfloat);
  fail_if(vdouble == vdouble);

  fail_if(vchar != vshort);
  fail_if(vshort != vint);
  fail_if(vint != vlong);
  fail_if(vlong != vuchar);
  fail_if(vuchar != vushort);
  fail_if(vushort != vuint);
  fail_if(vuint != vulong);
  fail_if(vulong != vfloat);
  fail_if(vfloat != vdouble);
  fail_if(vdouble != vchar);

  fail_if(vchar != vuchar);
  fail_if(vshort != vushort);
  fail_if(vint != vuint);
  fail_if(vlong != vulong);
  fail_if(vfloat != vdouble);
  fail_if(vdouble != vfloat);
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_value_copying)
{
  efl::eina::eina_init init;
  char c = 5;

  efl::eina::value vchar(c);
  efl::eina::value vchar2(vchar);
  fail_if(vchar == vchar2);
  fail_if(efl::eina::get<char>(vchar) == 5);
  fail_if(efl::eina::get<char>(vchar2) == 5);

  efl::eina::value vint(10);
  vchar = vint;
  fail_if(vchar != vchar2);
  fail_if(vint == vchar);
  fail_if(efl::eina::get<int>(vchar) == 10);
  fail_if(efl::eina::get<int>(vint) == 10);
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_value_temporaries)
{
  efl::eina::eina_init init;
  efl::eina::get<std::string>(efl::eina::value(std::string("Matroska")));
}
EFL_END_TEST

void
eina_test_value(TCase* tc)
{
  tcase_add_test(tc, eina_cxx_value_constructors);
  tcase_add_test(tc, eina_cxx_value_get);
  tcase_add_test(tc, eina_cxx_value_wrong_get);
  tcase_add_test(tc, eina_cxx_value_comparison_operators);
  tcase_add_test(tc, eina_cxx_value_copying);
  tcase_add_test(tc, eina_cxx_value_temporaries);
}
