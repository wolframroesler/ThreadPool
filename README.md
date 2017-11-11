# ThreadPool

A simple C++11 Thread Pool implementation.

## How To Use

Basic usage:

```c++
#include <threadpool.h>

// Create thread pool with 4 worker threads
ThreadPool pool(4);

// Enqueue a task and store its future
auto result = pool([](int answer) { return answer; }, 42);

// Get result from future
std::cout << result.get() << std::endl;
```

A compilable demonstration for various C++11 multithreading techniques
is in `threaddemo.cpp`. Refer to the source code for details. Build it
like this:

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./threaddemo
```

## Links

std::thread reference: http://en.cppreference.com/w/cpp/thread/thread

std::async reference: http://en.cppreference.com/w/cpp/thread/async

std::condition_variable reference: http://en.cppreference.com/w/cpp/thread/condition_variable

std::atomic reference: http://en.cppreference.com/w/cpp/atomic/atomic

Scott Meyer's "Effective Modern C++": http://shop.oreilly.com/product/0636920033707.do

Multithreading tutorials and examples: http://thispointer.com/category/multithreading/

Top 20 multithreading mistakes and how to avoid them: http://www.acodersjourney.com/2017/08/top-20-cplusplus-multithreading-mistakes/

Introduction to lockless programming: https://assets.bitbashing.io/papers/lockless.pdf
