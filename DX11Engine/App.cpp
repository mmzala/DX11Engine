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
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.GetGraphics().ClearBuffer(c, c, 1.0f);
	wnd.GetGraphics().EndFrame();
}
