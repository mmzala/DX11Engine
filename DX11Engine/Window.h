#pragma once

#include "BetterWin.h"
#include "DXEException.h"
#include "Keyboard.h"
#include "Mouse.h"

class Window
{
public:
	class Exception : public DXEException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;

		virtual const char* GetType() const noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;

	private:
		HRESULT hr;
	};

private:
	// Singleton manages registration/cleanup of the Window class
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Direct3D Engine";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	//void SetTitle(const std::string& title); Used for testing

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Keyboard kbd;
	Mouse mouse;

private:
	int width;
	int height;
	HWND hWnd;
};


// error exception helpers
#define WND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr);
#define WND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError());