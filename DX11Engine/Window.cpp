#include "Window.h"
#include <sstream>
#include "resource.h"

// Window class methods
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}


// Window methods
Window::Window(int width, int height, const char* name)
{
	// Calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (FAILED(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)))
	{
		throw WND_LAST_EXCEPT();
	}

	// Create window and get hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	// Check if CreateWindow() failed
	if (hWnd == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}

	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// Extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// Set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// Set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		// Forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	// If there is a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	// DefProc doesn't handle this message, the destructor does (destructor destroys the window)
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Window Exception methods
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	DXEException(line, file),
	hr(hr)
{}

const char* Window::Exception::what() const noexcept
{
	// Output error information
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;

	// Windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);

	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}

	// Copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;

	// Free windows buffer
	LocalFree(pMsgBuf);

	return errorString;
}

const char* Window::Exception::GetType() const noexcept
{
	return "Window exception";
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}