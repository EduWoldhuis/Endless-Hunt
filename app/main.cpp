#include "gui.h"

#include <thread>



int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{
	// Create GUI
	const LPCWSTR windowName = L"Endless Hunt";
	const LPCWSTR className = L"Endless Hunt";
	gui::CreateHWindow(windowName, className); // Window name and window class name
	gui::CreateDevice();
	gui::CreateImGui();	
	while (gui::exit)
	{
		gui::BeginRender();
		gui::Render();
		gui::EndRender();
	}

	// Destroy GUI (in reverse order of creation)
	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();


	return EXIT_SUCCESS;
}
