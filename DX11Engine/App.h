#pragma once

#include "Window.h"
#include "EngineTimer.h"

class App
{
public:
	App();
	// master frame / message loop
	int Start();

private:
	void UpdateFrame();

private:
	Window wnd;
	EngineTimer timer;
};

