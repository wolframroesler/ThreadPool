/**
 * @brief C++11 multithreading demonstration program
 * @author Wolfram Rösler <wolfram@roesler-ac.de>
 * @date 2017-10-14
 */

#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

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
	sleep(sec);
	Say("Slept for " + std::to_string(sec) + " s");

	return sec;
}

/**
 * Demonstration 1: Simple std::thread example
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
 */
void Demo3() {

	// Start a task and run it in the background
	auto f = std::async(DoSomething,2);

	// Do something else while the background task is running
	DoSomething(1);

	// Wait for the background task to finish and show its result
	const auto result = f.get();
	Say("The result is " + std::to_string(result));
}

/**
 * Demonstrations 4 and 5: Specify std::async launch policy
 *
 * @param defer Launch policy: false=async, true=deferred
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
	Say("The result is " + std::to_string(result));
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
	  Say("The result is " + std::to_string(result));
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
				;
			return EXIT_FAILURE;
	}
}
