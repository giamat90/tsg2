#include <io/io.h>

int main() {
	tsg::print("Hello Triangle");
	try {
		triangle t;
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