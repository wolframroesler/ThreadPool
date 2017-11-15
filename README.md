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

Complete example:

```c++
#include <iostream>
#include <vector>
#include <chrono>

#include "threadpool.h"

int main()
{
    ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }

    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;

    return 0;
}
```

## Multithreading Demo Program

`threaddemo.cpp` is a compilable demonstration program for various C++11 multithreading techniques, including thread pools. Refer to the source code for details. Build and run it like this:

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./threaddemo
$ time ./threaddemo 1	# etc.
```

## Links

std::thread reference
http://en.cppreference.com/w/cpp/thread/thread

std::async reference
http://en.cppreference.com/w/cpp/thread/async

std::condition_variable reference
http://en.cppreference.com/w/cpp/thread/condition_variable

std::atomic reference
http://en.cppreference.com/w/cpp/atomic/atomic

Scott Meyer's "Effective Modern C++"
http://shop.oreilly.com/product/0636920033707.do

Multithreading tutorials and examples
http://thispointer.com/category/multithreading/

Top 20 multithreading mistakes and how to avoid them
http://www.acodersjourney.com/2017/08/top-20-cplusplus-multithreading-mistakes/

Introduction to lockless programming
https://assets.bitbashing.io/papers/lockless.pdf
