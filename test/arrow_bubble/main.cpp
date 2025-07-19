#include <tsg/io.h>
#include "triangle_game.h"

int main() {
	tsg::print("Hello Triangle");
	try {
		triangle_game t("triangle.exe", 1024, 768, 60);
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