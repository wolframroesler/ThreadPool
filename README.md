ThreadPool
==========

A simple C++11 Thread Pool implementation.

Basic usage:

```c++
// create thread pool with 4 worker threads
ThreadPool pool(4);

// enqueue task and store future
auto result = pool([](int answer) { return answer; }, 42);

// get result from future
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
