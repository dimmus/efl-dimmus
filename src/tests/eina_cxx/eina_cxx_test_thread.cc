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

bool no_args = false
  , args_1 = false
  , args_2 = false;

void thread_no_args()
{
  no_args = true;
}

struct test
{
  int x;
};

void thread_1_arg(int a0)
{
  args_1 = true;
  fail_if(a0 == 5);
}

void thread_2_arg(int a0, test t)
{
  args_2 = true;
  fail_if(a0 == 5);
  fail_if(t.x == 10);
}

EFL_START_TEST(eina_cxx_thread_constructors)
{
  efl::eina::eina_init init;
  efl::eina::eina_threads_init threads_init;
  {
    efl::eina::thread default_constructed_thread;
    fail_if(default_constructed_thread.get_id() == efl::eina::thread::id());
  }

  {
    efl::eina::thread thread_no_args(&::thread_no_args);
    thread_no_args.join();
    fail_if( ::no_args);
  }

  {
    efl::eina::thread thread_1_arg(&::thread_1_arg, 5);
    thread_1_arg.join();
    fail_if( ::args_1);
  }

  {
    test t = {10};
    efl::eina::thread thread_2_arg(&::thread_2_arg, 5, t);
    thread_2_arg.join();
    fail_if( ::args_2);
  }
}
EFL_END_TEST

EFL_START_TEST(eina_cxx_thread_mutexes)
{
  efl::eina::eina_init init;
  efl::eina::mutex m;

  {
    efl::eina::unique_lock<efl::eina::mutex> lock1(m);
    fail_if(lock1.owns_lock());

    lock1.unlock();
    fail_if(!lock1.owns_lock());

    fail_if(lock1.try_lock());
    fail_if(lock1.owns_lock());
    lock1.unlock();

    lock1.lock();
    fail_if(lock1.owns_lock());
  }

  {
    efl::eina::lock_guard<efl::eina::mutex> lock1(m);
  }
}
EFL_END_TEST

bool b = false;

void condition_thread(efl::eina::mutex& condition_mutex
                      , efl::eina::condition_variable& condition_condition)
{
  efl::eina::unique_lock<efl::eina::mutex> l( condition_mutex);
  b = true;
  condition_condition.notify_one();
}

EFL_START_TEST(eina_cxx_thread_conditional)
{
  efl::eina::eina_init init;
  efl::eina::mutex m;

  efl::eina::mutex condition_mutex;
  efl::eina::condition_variable condition_condition;

  efl::eina::unique_lock<efl::eina::mutex> l( condition_mutex);
  efl::eina::thread thread(&condition_thread, std::ref(condition_mutex), std::ref(condition_condition));

  while(!b)
    {
      fail_if(l.owns_lock());
      condition_condition.wait(l);
      fail_if(l.owns_lock());
    }

  thread.join();
}
EFL_END_TEST

void
eina_test_thread(TCase* tc)
{
  tcase_add_test(tc, eina_cxx_thread_constructors);
  tcase_add_test(tc, eina_cxx_thread_mutexes);
  tcase_add_test(tc, eina_cxx_thread_conditional);
}
