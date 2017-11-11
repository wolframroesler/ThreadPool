/**
 * @brief C++11 multithreading demonstration program
 * @author Wolfram Rösler <wolfram@roesler-ac.de>
 * @date 2017-10-14
 */

#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "threadpool.h"

/**
 * Write msg to cout in a thread-safe way.
 *
 * @param msg The message to display. Need not be \n terminated.
 */
void Say(const std::string& msg) {
	static std::mutex m;
	std::lock_guard<std::mutex> _(m);
	std::cout << msg << std::endl;
}

/**
 * Simulate activity that takes a certain time.
 * Write a message to cout when we begin and finish.
 *
 * @param sec The number of seconds after which this function returns.
 *
 * @returns sec.
 */
int DoSomething(int sec) {
	Say("Sleeping for " + std::to_string(sec) + " s ...");
	std::this_thread::sleep_for(std::chrono::seconds(sec));
	Say("Slept for " + std::to_string(sec) + " s.");

	return sec;
}

/**
 * Demonstration 1: Simple std::thread example
 *
 * Things to keep in mind:
 *
 * - The std::thread ctor will perfect-forward parameters to
 *   the thread function.
 * - When using a lambda, be careful with capture-by-reference.
 * - Must call either join or detach on the thread object
 *   (exactly one of them, exactly once). If you don't, the
 *   std::thread dtor will will abort() your program.
 */
void Demo1() {

	// Start a thread and run it in the background
	auto t = std::thread(DoSomething,2);

	// Do something else while the background thread is running
	DoSomething(1);

	// Wait for the background thread to finish
	t.join();
}

/**
 * Demonstration 2: Start several threads and wait for all of them
 *
 * Things to keep in mind:
 *
 * - Can store threads in regular containers, no need for a
 *   dedicated thread group class.
 * - Use emplace_back to perfect-forward to prevent copying.
 */
void Demo2() {

	// Start a number of threads
	std::vector<std::thread> v;
	for(auto i=0,sec=1;i<3;++i,sec*=2) {
		v.emplace_back(DoSomething,sec);
	}

	// Wait for all threads to finish
	for(auto& t : v) {
		t.join();
	}
}

/**
 * Demonstration 3: Simple std::async example
 *
 * Things to keep in mind:
 *
 * - With async we call it "task", not "thread".
 * - Call get to retrieve the task function's return value.
 * - If the task function throws, get will throw.
 * - Looks nice and clean but in reality you'll really want
 *   to specify the launch policy.
 */
void Demo3() {

	// Start a task and run it in the background
	auto f = std::async(DoSomething,2);

	// Do something else while the background task is running
	DoSomething(1);

	// Wait for the background task to finish and show its result
	const auto result = f.get();
	Say("The result is " + std::to_string(result) + ".");
}

/**
 * Demonstrations 4 and 5: Specify std::async launch policy
 *
 * @param defer Launch policy: false=async, true=deferred
 *
 * Things to keep in mind:
 *
 * - deferred = run task function when get is called.
 * - async = run task function in the background now.
 * - none of them = deferred|async. In theory, runtime lib
 *   picks the optimal way to run your task. In practice,
 *   runtime lib may choose to defer all the time.
 */
void Demo45(bool defer) {

	// Start a task and run it in the background
	auto f = defer
		? std::async(std::launch::deferred,DoSomething,2)
		: std::async(std::launch::async,   DoSomething,2);

	// Do something else while the background task is running
	DoSomething(1);

	// Wait for the background task to finish and show its result
	const auto result = f.get();
	Say("The result is " + std::to_string(result) + ".");
}

/**
 * Demonstration 6: Start several async tasks and wait for them
 */
void Demo6() {

	// Start a number of async tasks
	std::vector<std::future<int>> v;
	for(auto i=0,sec=1;i<3;++i,sec*=2) {
		v.push_back(
			std::async(std::launch::async,DoSomething,sec)
		);
	}

	// Wait for all tasks to finish, and show their results
	for(auto& f : v) {
	  const auto result = f.get();
	  Say("The result is " + std::to_string(result) + ".");
	}
}

/**
 * Demonstration 7: Simple thread pool example
 *
 * Things to keep in mind:
 *
 * - Thread pool pre-allocates a number of threads to reduce
 *   thread-creation overhead. You may want to keep your
 *   thread pool object around, no need to delete and re-create
 *   it every time.
 * - Use operator() to add a new task to the pool.
 * - A thread pool has a certain number of threads. Tasks can
 *   be added to the pool any time, even when all threads are
 *   busy (the thread pool will never refuse new tasks because
 *   it is "full"). When all threads are busy, new tasks wait
 *   until a thread becomes available.
 * - Tasks can return values and exceptions just like std::async.
 */
void Demo7() {

	// Create a thread pool
	ThreadPool p;

	// Start a task in the pool
	auto f = p(DoSomething,2);

	// Do something else while the background task is running
	DoSomething(1);

	// Wait for the task to finish and show its result
	const auto result = f.get();
	Say("The result is " + std::to_string(result) + ".");
}

/**
 * Demonstrations 8 and 9: Start several tasks with a thread pool
 *
 * @param toosmall If true, make the pool smaller than the number of tasks
 *
 * Things to keep in mind:
 *
 * - Specify the number of threads when creating the thread pool.
 * - The right number of threads depends on your requirements to
 *   choose wisely.
 * - The default may or may not suit your needs.
 * - It's not the thread pool's job to keep you from overloading
 *   your machine.
 */
void Demo89(bool toosmall) {

	// Create a pool with either enough or too few parallel threads
	ThreadPool p(toosmall ? 2 : 4);

	// Start a number of tasks in the pool
	std::vector<std::future<int>> v;
	for(auto i=0,sec=1;i<3;++i,sec*=2) {
		v.push_back(
			p(DoSomething,sec)
		);
	}

	// Wait for all tasks to finish, and show their results
	for(auto& f : v) {
	  const auto result = f.get();
	  Say("The result is " + std::to_string(result) + ".");
	}
}

/**
 * Program starts here
 */
int main(int argc,char** argv) {
	switch(argc==2 ? atoi(argv[1]) : -1) {

		case 1: Demo1();		break;
		case 2: Demo2();		break;
		case 3: Demo3();		break;
		case 4: Demo45(false);	break;
		case 5: Demo45(true);	break;
		case 6: Demo6();		break;
		case 7: Demo7();		break;
		case 8: Demo89(false);	break;
		case 9: Demo89(true);	break;

		default:
			std::cout
				<< "C++11 multithreading demonstration program\n"
				<< "Usage:\n"
				<< "\t" << argv[0] << " 1\tSimple std::thread example\n"
				<< "\t" << argv[0] << " 2\tStart several threads and wait for all of them\n"
				<< "\t" << argv[0] << " 3\tSimple std::async example\n"
				<< "\t" << argv[0] << " 4\tStart async task with the async policy\n"
				<< "\t" << argv[0] << " 5\tStart async task with the deferred policy\n"
				<< "\t" << argv[0] << " 6\tStart several async tasks and get their results\n"
				<< "\t" << argv[0] << " 7\tSimple thread pool example\n"
				<< "\t" << argv[0] << " 8\tStart several tasks in a big-enough thread pool\n"
				<< "\t" << argv[0] << " 9\tStart more tasks than the size of the thread pool\n"

				;
			return EXIT_FAILURE;
	}
}
