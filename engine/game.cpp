#include "game.h"
#include <tsg/logger.h>
#include <tsg/string.h>
#include <tsg/os.h>


game::game() {
	m_physics = new physics();
	//tsg::logger::make_instance(
	//	tsg::string("engine_log_{}.txt", tsg::date::get_date(tsg::date::TYPE::FULL_DATE, false)).replace_c({ ' ', ':' }, '_')
	//);
}

game::~game() {
	quit(); // quit libraries
	delete m_renderer;
	delete m_window;
	delete m_input;
	delete m_physics;
	delete m_timer;
}


