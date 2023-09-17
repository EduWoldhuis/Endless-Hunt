#pragma once


#include <d3d9.h>
#include <d3dx9tex.h>


namespace gui
{	
	// Whether or not the window should be kept on top
	static bool window_topmost = false;

	// Constant window size
	static float WIDTH = 400.0f;
	static float HEIGHT = 500.0f;

	// exit true --> active window
	inline bool exit = true;

	// winapi windows variables
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = { };

	// points for window movement
	inline POINTS position = { };

	// Direct-x state variables
	inline PDIRECT3D9 d3d = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline D3DPRESENT_PARAMETERS presentParameters = { };



	// Creation and destruction of windows
	void CreateHWindow(
		const LPCWSTR windowName,
		const LPCWSTR className) noexcept;
	void DestroyHWindow() noexcept;

	// handle device creation and destruction
	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;

	// Handle ImGUI	creation and destruction
	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	// Handle rendering
	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;




}
