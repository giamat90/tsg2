#include <tsg/io.h>
#include "arrow_and_bubbles.h"

int main() {
	tsg::print("Arrow and Bubbles start");
	try {
		triangle_game t("Arrow and Bubbles.exe", 1024, 768, 60);
		t.initialize();
		t.run_game();
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