#include "pong.h"
#include <io/io.h>

int main() {
	try {
		pong p;
		if (p.initialize())
		{
			p.run_game();
		}
		p.shutdown();
	}
	catch (std::exception& e) {
		tsg::print(e.what());
	}
	catch (...) {
		tsg::print("Unkown exception occured");
	}
}