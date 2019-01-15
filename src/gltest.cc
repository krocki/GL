/*
* @Author: kmrocki
* @Date:   2016-11-07 18:30:48
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-11-08 20:13:54
*/

#include <containers/matrix.h>
#include <iostream>
#include <rand.h>
#include <thread>
#include <containers/io.h>

#ifdef USE_OPENGL
#include <glWindow.h>
#endif

using namespace std;

int test() {

	Matrix<float> lena(512, 512);
	Matrix<float> lena_rgb(512, 512 * 3);

#ifdef USE_OPENGL
	GLWindowRenderer::set( 1, &lena, MODE_GRAY );
	GLWindowRenderer::set( 2, &lena_rgb, MODE_GRAY );
	GLWindowRenderer::set( 3, &lena_rgb, MODE_RGB );
#endif

	readMatrix ( lena, "images/lena.txt" );
	readMatrix ( lena_rgb, "images/lena_rgb.txt" );

	while (true) {

		usleep(1000);
	}

	return 0;

}

int main( int argc, char* argv[] ) {

#ifdef USE_OPENGL

	GLWindowRenderer::init();
	GLWindowRenderer::add( "lena", 50, 450, 400, 400 );
	GLWindowRenderer::add( "lena_rgb", 100, 50, 1050, 350 );
	GLWindowRenderer::add( "lena_rgb_color", 650, 450, 400, 400 );

	std::thread main_thread( test );

	GLWindowRenderer::main_loop();

	main_thread.join();

#else
	test();
#endif

	return 0;

}
