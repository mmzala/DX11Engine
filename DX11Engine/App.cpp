#include "App.h"

App::App()
	:
	wnd(800, 600, "DX11Engine")
{}

int App::Start()
{
	while (true)
	{
		if (const auto ExitCode = wnd.ProcessMessages())
		{
			return *ExitCode;
		}

		UpdateFrame();
	}
}

void App::UpdateFrame()
{

}
