#include <game_object.h>

class ball : public dynamic_object {
public:
	ball() : dynamic_object() {};
	void update(const float delta_time) override final;
private:

};