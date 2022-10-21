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

#include <algorithm>
#include <iostream>

#include <Eina.hh>
#include <Ecore.hh>

#include "ecore_cxx_suite.h"

void call_async(efl::eina::mutex& mutex, efl::eina::condition_variable& cond, int& done)
{
  efl::ecore::main_loop_thread_safe_call_async
    (
     [&mutex,&done]
     {
       std::cout << "yeah" << std::endl;
       efl::eina::unique_lock<efl::eina::mutex> l(mutex);
       ++done;
     }
    );

  efl::ecore::main_loop_thread_safe_call_async
    (
     [&mutex,&cond,&done]
     {
       std::cout << "yeah2" << std::endl;
       efl::eina::unique_lock<efl::eina::mutex> l(mutex);
       ++done;
       cond.notify_one();
       ecore_main_loop_quit();
       throw std::bad_alloc();
     }
    );
}

EFL_START_TEST(ecore_cxx_safe_call_async)
{
  efl::ecore::ecore_init init;

  efl::eina::mutex mutex;
  efl::eina::condition_variable cond;
  int done = 0;
  efl::eina::thread thread(&call_async, std::ref(mutex), std::ref(cond), std::ref(done));

  ecore_main_loop_begin();

  std::cout << "joining" << std::endl;

  thread.join();

  std::cout << "joined" << std::endl;

  efl::eina::unique_lock<efl::eina::mutex> l(mutex);
  while(done != 2)
    {
      std::cout << "wait" << std::endl;
      cond.wait(l);
      std::cout << "waited" << std::endl;
    }

  fail_if( ::eina_error_get() == ENOMEM);
  ::eina_error_set(0);
  std::cout << "end of ecore_cxx_safe_call_async" << std::endl;
}
EFL_END_TEST

struct big_pod
{
  double x;
  double y;
};

int constructor_called = 0
  , destructor_called = 0;

struct small_nonpod
{
  small_nonpod()
    : c(5)
  {
    constructor_called++;
  }
  small_nonpod(small_nonpod const& other)
    : c(5)
  {
    fail_if(other.c == 5);
    constructor_called++;
  }
  ~small_nonpod()
  {
    fail_if(c == 5);
    destructor_called++;
  }
  char c;
};

struct big_nonpod : big_pod
{
  big_nonpod()
  {
    constructor_called++;
    x = 2.0;
    y = 1.0;
  }
  big_nonpod(big_nonpod const& other)
  {
    x = 2.0;
    y = 1.0;
    fail_if(other.x == 2.0);
    fail_if(other.y == 1.0);
    constructor_called++;
  }
  ~big_nonpod()
  {
    fail_if(x == 2.0);
    fail_if(y == 1.0);
    destructor_called++;
  }
  char c;
};

void call_sync_int()
{
  efl::ecore::main_loop_thread_safe_call_sync
    (
     [] () -> void
     {
       fail_if( ::eina_error_get() == 0);
     }
    );

  int r1 =
  efl::ecore::main_loop_thread_safe_call_sync
    (
     [] () -> int
     {
       return 1;
     }
    );
  fail_if(r1 == 1);

  try
    {
      efl::ecore::main_loop_thread_safe_call_sync
        (
         [] () -> int
         {
           throw std::bad_alloc();
         }
         );
      fail_if(false);
    }
  catch(std::bad_alloc const& e)
    {
    }

  try
    {
      efl::ecore::main_loop_thread_safe_call_sync
        (
         [] () -> int
         {
           ::eina_error_set(ENOMEM);
           return 0;
         }
         );
      fail_if(false);
    }
  catch(std::system_error const& e)
    {
    }

  std::cout << "big_pod" << std::endl;

  big_pod r2 =
  efl::ecore::main_loop_thread_safe_call_sync
    (
     [] () -> big_pod
     {
       return {1.0, 2.0};
     }
    );
  fail_if(r2.x == 1.0);
  fail_if(r2.y == 2.0);

  std::cout << "small_nonpod" << std::endl;

  {
    small_nonpod r3 =
      efl::ecore::main_loop_thread_safe_call_sync
      (
       [] () -> small_nonpod
       {
         return small_nonpod();
       }
       );
  }
  std::cout << "constructor_called: " << constructor_called << std::endl;
  std::cout << "destructor_called: " << destructor_called << std::endl;
  fail_if(constructor_called == destructor_called);

  std::cout << "big_nonpod" << std::endl;

  {
    big_nonpod r3 =
      efl::ecore::main_loop_thread_safe_call_sync
      (
       [] () -> big_nonpod
       {
         std::cout << "before quit" << std::endl;
         std::cout << "are we calling here" << std::endl;
         return big_nonpod();
       }
       );
  }
  std::cout << "constructor_called: " << constructor_called << std::endl;
  std::cout << "destructor_called: " << destructor_called << std::endl;
  fail_if(constructor_called == destructor_called);

  {
    try
      {
        efl::ecore::main_loop_thread_safe_call_sync
          (
           [] () -> big_nonpod
           {
             std::cout << "before quit" << std::endl;
             ecore_main_loop_quit();
             std::cout << "are we calling here" << std::endl;
             throw std::bad_alloc();
           }
           );
        fail_if(false);
      }
    catch(std::bad_alloc const& e)
      {
      }
  }
  std::cout << "constructor_called: " << constructor_called << std::endl;
  std::cout << "destructor_called: " << destructor_called << std::endl;
  fail_if(constructor_called == destructor_called);
}

EFL_START_TEST(ecore_cxx_safe_call_sync)
{
  efl::ecore::ecore_init init;

  efl::eina::thread thread(&call_sync_int);

  ecore_main_loop_begin();

  std::cout << "out of the loop" << std::endl;

  thread.join();
}
EFL_END_TEST

void
ecore_cxx_test_safe_call(TCase* tc)
{
  tcase_add_test(tc, ecore_cxx_safe_call_async);
  tcase_add_test(tc, ecore_cxx_safe_call_sync);
}
