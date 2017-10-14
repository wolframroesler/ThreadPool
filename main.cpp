/**
 * @brief C++11 multithreading demonstration program
 * @author Wolfram Rösler <wolfram@roesler-ac.de>
 * @date 2017-10-14
 */

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
 */
void DoSomething(int sec) {
	Say("Sleeping for " + std::to_string(sec) + " s ...");
	sleep(sec);
	Say("Slept for " + std::to_string(sec) + " s");
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
	for(auto &t : v) {
		t.join();
	}
}

/**
 * Program starts here
 */
int main(int argc,char** argv) {
	switch(argc==2 ? atoi(argv[1]) : -1) {

		case 1: Demo1(); break;
		case 2: Demo2(); break;

		default:
		  std::cout
			<< "C++11 multithreading demonstration program\n"
			<< "Usage:\n"
			<< "\t" << argv[0] << " 1\tSimple std::thread example\n"
			<< "\t" << argv[0] << " 2\tStart several threads and wait for all of them\n"
			;
		  return EXIT_FAILURE;
	}
}
