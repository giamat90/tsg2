#include <tsg/io.h>
#include "triangle_game.h"

int main() {
	tsg::print("Hello Triangle");
	try {
		triangle_game t;
		if (t.initialize())
		{
			t.run_game();
		}
		t.shutdown();
	}
	catch (std::exception& e) {
		tsg::print(e.what());
	}
	catch (...) {
		tsg::print("Unkown exception occured");
	}
	tsg::print("Goodbye Triangle");
}