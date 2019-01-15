/*
* @Author: kmrocki
* @Date:   2016-11-07 18:30:48
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-11-07 18:31:21
*/

#include <iostream>
#include <iomanip>
#include <cell.h>
#include <glMain.h>
#include <vector>
#include <thread>

using namespace std;

std::vector<Cell> cells;

void test(void) {

	Timer t;

	for (size_t i = 0; i < 1024 * 1; i++)
		cells.push_back(Cell(i));

	for (size_t iteration = 0;; iteration++) {

		if (!_pause || _step) {

			display_mutex.lock();

			_step = false;

			t.start();

			for (size_t i = 0; i < cells.size(); i++) {

				float a = MT19937::rand();
				cells[i].active = a < 0.01;
				float p = MT19937::rand();
				cells[i].predicted = p < 0.01;
			}

			std::cout << std::setprecision(6) << t.end() << " s" << std::endl;

			display_mutex.unlock();

			usleep(10000);

		}
	}
}

int main() {


	glinit();

	std::thread main_thread(test);

	// infinite display loop
	glutMainLoop();

	main_thread.join();

	cells.clear();

	return 0;

}