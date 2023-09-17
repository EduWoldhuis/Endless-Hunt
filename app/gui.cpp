#include <tchar.h>
#include <Windows.h>
#include <iostream>

#include <chrono>
#include "gui.h"
#include "images.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"
#include "assets.h"
#include <d3dx9tex.h>
#include <string>

#include "dolphinTools.h"
#include "dolphin_stuff/locations.h"
#include "font_stuff.h"

#include "animations/animations.h"

#ifdef _UNICODE
typedef wchar_t TCHAR;
#else
typedef char TCHAR;
#endif // _UNICODE

typedef const TCHAR* LPCTSTR;


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

// Handles all the events windows sends to the window
//long __stdcall WindowProcess(
LRESULT WINAPI WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	



	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
	{
		return true;
	}

	switch (message)
	{
	case WM_SIZE: {
		static int x = 0;
		x++;
		int a = LOWORD(longParameter);
		int b = HIWORD(longParameter);
		std::cout << a << b << std::endl;
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT Application menu (file, view, help etc)
			return 0;
	}break;

		// Closing the window
	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter); // Set position of mouse button when clicked		
		
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON) // If left button is pressed
		{
			const auto points = MAKEPOINTS(longParameter); // Make points where we've clicked
			auto rect = ::RECT{ }; // Make a rect

			GetWindowRect(gui::window, &rect); // Make the rect the size of the window

			// Calculate the position of the mouse relative to the window
			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			
			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
			{
				// Set the window position to where we're moving it to
				
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE
				);
				
			}
		}
	case WM_WINDOWPOSCHANGING: {
		if (gui::window_topmost)
			SetWindowPos(gui::window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		else
			SetWindowPos(gui::window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}

	}
	}
	
	return DefWindowProcW(window, message, wideParameter, longParameter);
}


void gui::CreateHWindow(
	const LPCWSTR windowName,
	const LPCWSTR className) noexcept
{

	const LPCWSTR lpszUnicode = L"";
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess; // Sets the Windows process to our window process function, so Windows knows which window process to call.
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className; // Sets the class-name to the class-name passed into the function
	windowClass.hIconSm = 0;
	
	RegisterClassEx(&windowClass);
	
	
	
	window = ::CreateWindow(
		windowClass.lpszClassName,
		windowName,
		WS_EX_LAYERED | WS_POPUP,  // Sets the window type to a WS_POPUP
		// Sets the position (from top left) in pixels where the window begins
		100,
		100,
		// Sets the size of the window
		static_cast<int>(WIDTH),
		static_cast<int>(HEIGHT),
		0,
		0,
		windowClass.hInstance,
		0
	);
	
	
	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

// directX9 device stuff
bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d) { // If the device failed to create, return false.
		return false;
	}

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

		
	// Creating the device in the if-statement
	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0) // If the function gives a return value below 0, it failed.
	{
		return false;
	}
	
	return true;
}

void gui::ResetDevice() noexcept
{	
	ImGui_ImplDX9_InvalidateDeviceObjects();

	// Resets the device
	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
	{
		IM_ASSERT(0);
	}

	ImGui_ImplDX9_CreateDeviceObjects();

}

void gui::DestroyDevice() noexcept
{
	// If the device is valid, release it and set it to a null-pointer
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	// If d3d is valid, release it and set it to a null-pointer
	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

float font_size = 19.5f;
float font_size_small = 19.0f;
ImFont* icons_font;
ImFont* main_font;
ImFont* main_font_small;
void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL; // Don't make a config file
	
	// Set the ImGUI style (dark)
	ImGui::StyleColorsDark();
	
	ImGuiStyle* style = &ImGui::GetStyle();
	
	// colour palette, got to work on it
	ImColor colour_a = ImColor(1, 3, 8);
	ImColor colour_b = ImColor(9, 14, 21);
	ImColor colour_c = ImColor(3, 14, 41);
	ImColor colour_d = ImColor(13, 26, 41);
	ImColor colour_e = ImColor(9, 48, 116);


	style->WindowBorderSize = 0.0f;
	style->WindowPadding = ImVec2(0.f, 0.f);

	



	// ImGuiIO io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	main_font_small = io.Fonts->AddFontFromMemoryCompressedTTF(test_font_compressed_data, test_font_compressed_size, font_size_small);
	main_font = io.Fonts->AddFontFromMemoryCompressedTTF(test_font_compressed_data, test_font_compressed_size, font_size);
	
	
	static const ImWchar icon_ranges[]{ ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;

	icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 27.5f, &icons_config, icon_ranges);

		
	// Initialize ImGUI window and device
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
	
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


// Gets the Windows message, passes it to windowProcHandler, creates new ImGUI frames
void gui::BeginRender() noexcept
{
	
	
	//	SetWindowPos(gui::window, HWND_TOPMOST, 100, 100, 1000, 1000, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOMOVE);

	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) // Gets the Windows message
	{
		// translates and sends the message to the windowsProcessHandler
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	
	// Start the ImGUI frame
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();
}

// begins, ends, and present the scene.
void gui::EndRender() noexcept
{
	ImGui::EndFrame(); // Ends the frame that was made with the NewFrame() function in BeginRender.

	
	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	// Clear the DirectX buffer
	
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		// Background colour (palette B)
		D3DCOLOR_RGBA(1, 3, 8, 255), 
		1.0f, 0);

	// Begin and end the DirectX scene
	if (device->BeginScene() >= 0) // If DirectX begin scene works
	{
		ImGui::Render(); // Renders ImGUI data
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// Handle loss of DirectX device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) // If device is lost and hasn't been reset
	{
		ResetDevice();
	}

}

// void drawElement(ImDrawList* draw, ImVec2 pos, const char text[]);
int drawElement(ImDrawList* draw, ImVec2 pos, ImVec2 size, const char text[])
{
	draw->AddRectFilledMultiColor(pos, ImVec2(pos.x + size.x, pos.y + size.y), ImColor(24, 24, 26), ImColor(24, 24, 100), ImColor(24, 24, 26), ImColor(50, 24, 50));
	draw->AddText(ImVec2(pos.x, pos.y + 25), ImColor(50.0f, 45.0f, 255.0f, 255.0f), text);
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 40, 40));
	ImGui::Text(ICON_FA_COGS);
	
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 40, 20));
	static int config_enabled = 0;
	if (ImGui::InvisibleButton("##Invisible_button_config", ImVec2(40, 40)))
	{
		config_enabled ? config_enabled = 0 : config_enabled = 1;
	}

	ImGui::SetCursorPosY(20.0f + 50.0f); // Size of the top element, puts the position of the cursor right below the element.

	if (config_enabled == 1)
	{
		return 1;
	}
	return 0;
}

void drawEORoundedButton(ImDrawList* draw, ImVec2 pos, ImVec2 size, const char* text)
{
	static int enabled = 0;
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,50.f);
	ImVec2 lastPos = ImGui::GetCursorScreenPos();
	ImGui::SetCursorPos(pos);
	if (ImGui::InvisibleButton("##Invisible_button", size))
	{
		enabled ? enabled = 0 : enabled = 1;
		
	}
	// NOTE: AddRectFilledMultiColor is custom from https://github.com/ocornut/imgui/issues/3710
	if (ImGui::IsItemActive() == 0)
		draw->AddRectFilledMultiColor(pos, ImVec2(pos.x + size.x, pos.y + size.y), ImColor(24, 24, 26), ImColor(24, 24, 100), ImColor(24, 24, 26), ImColor(50, 24, 50), 500.0f, NULL);
	else
	{
		draw->AddRectFilledMultiColor(pos, ImVec2(pos.x + size.x, pos.y + size.y), ImColor(14, 14, 16), ImColor(14, 14, 90), ImColor(14, 14, 16), ImColor(40, 14, 40), 500.0f, NULL);
	}
	// ImGui::Text("Yippee %d", enabled);
	
	ImGui::SetCursorPos(lastPos);
	

	
	float text_width = ImGui::CalcTextSize(text).x;



	draw->AddText(ImVec2(pos.x + (size.x - text_width) * 0.5f, pos.y + (size.y - font_size) / 2), ImColor(50.0f, 45.0f, 255.0f, 255.0f), text);
	ImGui::PopStyleVar(1);
	return;
}




int inRange(float value, float low, float high)
{
	return (low <= value && value <= high);
}


// Holds what there is to render
void gui::Render() noexcept
{	
	ImGui::SetNextWindowPos({ 0, 0 }); // ImGUI will take up the full space of the window created
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT }); // prevents seeing the background directx window
		

	ImGui::Begin(
		"x64 test", // Title of the window
		&exit, // When clicking the exit button, it will set Exit to false (breaking us out of the loop)
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground
	);

	

	ImVec2 pos = ImGui::GetWindowPos();
	ImDrawList* draw = ImGui::GetWindowDrawList();
	
	static PDIRECT3DTEXTURE9 my_texture = NULL;
	static PDIRECT3DTEXTURE9 icon_texture = NULL;

	static int images_loaded = 0;

	if (images_loaded == 0)
	{
		int my_image_width = 255;
		int my_image_height = 255;
		D3DXCreateTextureFromFileInMemoryEx(device, &eo_testing_image, sizeof(eo_testing_image), my_image_width, my_image_height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &my_texture);
		// For if I want to change the icon to an image
		// D3DXCreateTextureFromFileInMemoryEx(device, &compass_icon_image, sizeof(compass_icon_image), 64, 64, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &icon_texture);
		images_loaded = 1;
	}
	
	// Palette: https://coolors.co/palette/010308-090e15-030e29-0d1a29-093074
	ImColor colour_a = ImColor(1, 3, 8);
	
	ImColor colour_b = ImColor(9, 14, 21);
	ImColor colour_c = ImColor(3, 14, 41);
	ImColor colour_d = ImColor(13, 26, 41);
	ImColor colour_e = ImColor(9, 48, 116);


	static ImColor background_colour = colour_a;
	
	
	// Load image from memory
	// Only load the image once
	static bool config_enabled = false;
	ImGui::PushFont(icons_font);
	
	if (drawElement(draw, ImVec2(0.0f, 20.0f), ImVec2(WIDTH, 50.0f), "   " ICON_FA_COMPASS "  Endless hunt"))
	{
		config_enabled = true;
	}
	else
	{
		config_enabled = false;
	}

	drawEORoundedButton(draw, ImVec2(127.5f, 350.0f), ImVec2(150.0f, 50.0f),  "Next level");
	

	static int eo_enabled = 0;				// Set only once
	static char* m_scanRAMCache = nullptr;
	if (eo_enabled == 0)
	{
		DolphinComm::DolphinAccessor::init();
		DolphinComm::DolphinAccessor::hook();
		eo_enabled = 1;
	}

	if (DolphinComm::DolphinAccessor::getStatus() == DolphinComm::DolphinAccessor::DolphinStatus::hooked)
	{
		std::cout << "hooked :D" << std::endl;		
		if (eo_enabled == 1)
		{
			// WindowDolphin::obtainEmuRAMInformations();
			// std::cout << "PID: " << DolphinComm::DolphinAccessor::getPID() << ", Emu RAM Addres start: " << std::hex << address_start << std::dec << std::endl;
			u64 address_start = DolphinComm::DolphinAccessor::getEmuRAMAddressStart();
			eo_enabled = 2;
		}

		// Scanning
		char* m_scanRAMCache = new char[250];
		DolphinComm::DolphinAccessor::readFromRAM(Common::dolphinAddrToOffset(Common::MEM1_START + 0x64E4D0, false), m_scanRAMCache, 200, false);
		eo_position pos = get_position(m_scanRAMCache);

		char* m_scanRAMCache2 = new char[250];
		DolphinComm::DolphinAccessor::readFromRAM(Common::dolphinAddrToOffset(Common::MEM1_START + 0x64E76C, false), m_scanRAMCache2, 200, false);
		std::string current_map = get_map(&m_scanRAMCache2[0x0]);
		

		if(inRange(pos.x, -400.0f, -370.0f) && inRange(pos.y, -80.0f, -50.0f) && inRange(pos.z, -440.0f, -400.0f))
		{
			ImGui::Text("Foud it :D");
		}

		delete(m_scanRAMCache);
		delete(m_scanRAMCache2);

		// Display the image in the middle
		ImVec2 lastPos = ImGui::GetCursorScreenPos();
		ImGui::SetCursorPos(ImVec2(((WIDTH - 307) / 2), ((HEIGHT - 172) / 2)));
		ImGui::Image((void*)my_texture, ImVec2(307.0f, 172.0f));
		ImGui::SetCursorPos(lastPos);


		static bool show_map = false;
		static bool show_coordinates = false;
		static int config_frame = 0;
		if (config_enabled == true || config_frame > 0)
		{
			if (config_enabled == true && config_frame < 25)
			{
				config_frame++;
			}
			if (config_enabled == false)
			{
				// config_frame > 0 clause already included above
				config_frame--;
			}

			ImVec2 lastPos = ImGui::GetCursorScreenPos();

			

			float delta_movement_x = WIDTH / 25;
			draw->AddRectFilled(ImVec2(WIDTH - delta_movement_x * config_frame, 70.0f), ImVec2(WIDTH, HEIGHT), background_colour);
			ImGui::SetCursorPosY(70.0f);


			// Set the text you want inside of the menu here.
			ImGui::SetCursorPosX(WIDTH - delta_movement_x * config_frame);
			ImGui::Text("Config menu");

			ImGui::SetCursorPosX(WIDTH - delta_movement_x * config_frame);
			ImGui::Toggle("Show the current map", &show_map);

			ImGui::SetCursorPosX(WIDTH - delta_movement_x * config_frame);
			ImGui::Toggle("Show the current coordinates", &show_coordinates);
			
			ImGui::SetCursorPosX(WIDTH - delta_movement_x * config_frame);
			ImGui::Toggle("Keep the windows on top", &gui::window_topmost);

			ImGui::SetCursorPos(lastPos);
					
		}
		
		ImGui::PushFont(main_font_small);
		if (show_map == true) {

			lastPos = ImGui::GetCursorScreenPos();

			ImGui::SetCursorPos(ImVec2(0, HEIGHT - font_size_small - 2 - font_size_small));
			// if colour:
			// ImGui::TextColored(ImColor(199, 191, 255), "Map: %s", current_map.c_str());
			ImGui::Text("Map: %s", current_map.c_str());
			ImGui::SetCursorPos(lastPos);
		}

		if (show_coordinates == true)
		{
			lastPos = ImGui::GetCursorScreenPos();
			// Make the XYZ coordinates evenly spaced
			ImGui::SetCursorPos(ImVec2(0, HEIGHT - font_size_small - 2));
			ImGui::Text("X: %.3f", pos.x);

			ImGui::SetCursorPos(ImVec2(ImGui::CalcTextSize("X: 12345678 ").x, HEIGHT - font_size_small - 2));
			ImGui::Text("Y: %.3f", pos.y);

			ImGui::SetCursorPos(ImVec2(ImGui::CalcTextSize("X: 12345678 Y: 12345678 ").x, HEIGHT - font_size_small - 2));
			ImGui::Text("Z: %.3f", pos.z);

			ImGui::SetCursorPos(lastPos);
		}
		ImGui::PopFont();
	}

	// Maybe optimize the GetStatus() into a variable or such, so it doesn't need to be called every time.
	else if (DolphinComm::DolphinAccessor::getStatus() == DolphinComm::DolphinAccessor::DolphinStatus::noEmu) {
		ImVec2 lastPos = ImGui::GetCursorScreenPos();
		ImGui::SetCursorPosY(HEIGHT - font_size);
		ImGui::TextColored(ImColor(255, 0, 0), "Emulator not running.");
		ImGui::SetCursorPos(lastPos);
		DolphinComm::DolphinAccessor::hook();
	}
	else if (DolphinComm::DolphinAccessor::getStatus() == DolphinComm::DolphinAccessor::DolphinStatus::notRunning) {
		ImVec2 lastPos = ImGui::GetCursorScreenPos();
		ImGui::SetCursorPosY(HEIGHT - font_size);
		ImGui::TextColored(ImColor(255, 0, 0), "Dolphin is not running.");
		ImGui::SetCursorPos(lastPos);
		DolphinComm::DolphinAccessor::hook();
	}
	else if (DolphinComm::DolphinAccessor::getStatus() == DolphinComm::DolphinAccessor::DolphinStatus::unHooked) {
		ImVec2 lastPos = ImGui::GetCursorScreenPos();
		ImGui::SetCursorPosY(HEIGHT - font_size);
		ImGui::TextColored(ImColor(255, 0, 0), "Can't hook to dolphin :<");
		ImGui::SetCursorPos(lastPos);
		DolphinComm::DolphinAccessor::hook();
	}

	
	// ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
	ImGui::PopFont();
	ImGui::End();
}

