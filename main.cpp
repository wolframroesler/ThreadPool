/**
 * @brief C++11 multithreading demonstration program
 * @author Wolfram Rösler <wolfram@roesler-ac.de>
 * @date 2017-10-14
 */

#include <iostream>

/**
 * Demonstration 1: Simple std::thread example
 */
void Demo1() {
	std::cout << "TBD\n";
}

/**
 * Program starts here
 */
int main(int argc,char** argv) {
	switch(argc==2 ? atoi(argv[1]) : -1) {
		case 1: Demo1(); break;

		default:
		  std::cout
			<< "C++11 multithreading demonstration program\n"
			<< "Usage:\n"
			<< "\t" << argv[0] << " 1\tSimple std::thread example\n"
			;
		  return EXIT_FAILURE;
	}
}
