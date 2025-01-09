#pragma once

#include <game_object.h>
#include <asset.h>

class field : public static_object
{
public: 
	field();
	~field();
private:
	asset m_asset{"C:\\Workspace\\GiaMat90\\pong\\Project1\\Assets\\paddle-court_dark_green.png"};
};

