/*
* @Author: kmrocki
* @Date:   2016-11-07 18:30:48
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-11-08 18:05:59
*/

#include <containers/matrix.h>
#include <iostream>
#include <rand.h>
#include <thread>

using namespace std;

int test() {

	Matrix<float> x(3, 5);

	/* http://en.cppreference.com/w/cpp/language/lambda */

	// all elements set to normal(0,1)
	x.forall([&] { return MT19937::randn(); });

	// display
	std::cout << "X = " << std::endl << x;

	Matrix<float> y(3, 5);
	Matrix<float> z(3, 5);

	y.forall([&] { return MT19937::randn(); });
	z.forall([&] { return MT19937::randn(); });

	std::cout << "Y = " << std::endl << y;
	std::cout << "Z = " << std::endl << z;

	x.forall([&](auto a, auto b) { return a + b; }, y, z);

	std::cout << "Y + Z = " << std::endl << x;

	x.forall([&](auto a, auto b, auto c) { return a + b + c; }, x, y, z);

	std::cout << "Y + Z + X = " << std::endl << x;

	return 0;

}

int main( int argc, char* argv[] ) {

	test();

	return 0;

}
