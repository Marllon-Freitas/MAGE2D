#include "Window.h"

bool Window::m_windowKeys[256] = { 0 };
int  Window::m_windowMouseX = 0;
int  Window::m_windowMouseY = 0;

Window::Window()
{
	m_hInstance = GetModuleHandle(NULL);
	m_windowHandle = 0;
	m_windowWidth = GetSystemMetrics(SM_CXSCREEN);
	m_windowHeight = GetSystemMetrics(SM_CYSCREEN);
	m_windowIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_windowCursor = LoadCursor(NULL, IDC_ARROW);
	m_windowColor = RGB(0, 0, 0);
	m_windowTitle = std::string("Windows Game");
	m_windowStyle = WS_POPUP | WS_VISIBLE;
	m_windowMode = FULLSCREEN;
	m_windowPosX = 0;
	m_windowPosY = 0;
	m_windowCenterX = m_windowWidth / 2.0f;
	m_windowCenterY = m_windowHeight / 2.0f;
}

void Window::SetMode(int mode)
{
	m_windowMode = mode;

	if (mode == WINDOWED)
	{
		m_windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;	// Windowed mode
	}
	else
	{
		m_windowStyle = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;		// Fullscreen or borderless mode
	}
}

void Window::SetSize(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	// Calculate the window center position
	m_windowCenterX = m_windowWidth / 2.0f;
	m_windowCenterY = m_windowHeight / 2.0f;

	// Adjust the window position to the center of the screen
	m_windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - m_windowWidth / 2;
	m_windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - m_windowHeight / 2;
}

void Window::Print(const std::string& text, int x, int y, COLORREF color) const
{
	// This function displays text at position (x, y) on the screen using the specified color
	// It uses the Windows library (slow) and should be used only for debugging

	// Get the device context
	HDC xdc = GetDC(m_windowHandle);

	// Set the text color
	SetTextColor(xdc, color);

	// Set the text background to transparent
	SetBkMode(xdc, TRANSPARENT);

	// Convert std::string to std::wstring
	std::wstring wtext(text.begin(), text.end());

	// Display the text
	TextOut(xdc, x, y, wtext.c_str(), static_cast<int>(wtext.size()));

	// Release the device context
	ReleaseDC(m_windowHandle, xdc);
}

bool Window::Create()
{
	WNDCLASSEX wndClass{};

	// Define a window class called "GameWindow"
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = Window::WinProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = m_windowIcon;
	wndClass.hCursor = m_windowCursor;
	wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(m_windowColor);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = L"GameWindow";
	wndClass.hIconSm = m_windowIcon;

	// Register "GameWindow"
	if (!RegisterClassEx(&wndClass))
		return false;

	// Create a window based on the "GameWindow" class
	m_windowHandle = CreateWindowEx(
		0,                              // Extra styles
		L"GameWindow",                  // Window class name
		std::wstring(m_windowTitle.begin(), m_windowTitle.end()).c_str(), // Window title
		m_windowStyle,                  // Window style
		m_windowPosX, m_windowPosY,     // Initial position (x, y)
		m_windowWidth, m_windowHeight,  // Window width and height
		nullptr,                        // Parent window handle
		nullptr,                        // Menu handle
		m_hInstance,                    // Application instance handle
		nullptr);                       // Creation parameters

	// In windowed mode, consider the space occupied by borders and title bars
	if (m_windowMode == WINDOWED)
	{
		// Rectangle with the desired client area size
		RECT winRect = { 0, 0, m_windowWidth, m_windowHeight };

		// Adjust the rectangle size
		AdjustWindowRectEx(&winRect,
			GetWindowStyle(m_windowHandle),
			GetMenu(m_windowHandle) != nullptr,
			GetWindowExStyle(m_windowHandle));

		// Update the window position
		m_windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - (winRect.right - winRect.left) / 2;
		m_windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - (winRect.bottom - winRect.top) / 2;

		// Resize the window with a call to MoveWindow
		MoveWindow(
			m_windowHandle,                  // Window handle
			m_windowPosX,                    // x position
			m_windowPosY,                    // y position
			winRect.right - winRect.left,    // Width
			winRect.bottom - winRect.top,    // Height
			TRUE);                           // Repaint
	}

	// Return the initialization status (successful or not)
	return (m_windowHandle != nullptr);
}

LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// Key pressed
	case WM_KEYDOWN:
		m_windowKeys[wParam] = true;
		return 0;

		// Key released
	case WM_KEYUP:
		m_windowKeys[wParam] = false;
		return 0;

		// Mouse movement
	case WM_MOUSEMOVE:
		m_windowMouseX = GET_X_LPARAM(lParam);
		m_windowMouseY = GET_Y_LPARAM(lParam);
		return 0;

		// Left mouse button pressed
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		m_windowKeys[VK_LBUTTON] = true;
		return 0;

		// Middle mouse button pressed
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
		m_windowKeys[VK_MBUTTON] = true;
		return 0;

		// Right mouse button pressed
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
		m_windowKeys[VK_RBUTTON] = true;
		return 0;

		// Left mouse button released
	case WM_LBUTTONUP:
		m_windowKeys[VK_LBUTTON] = false;
		return 0;

		// Middle mouse button released
	case WM_MBUTTONUP:
		m_windowKeys[VK_MBUTTON] = false;
		return 0;

		// Right mouse button released
	case WM_RBUTTONUP:
		m_windowKeys[VK_RBUTTON] = false;
		return 0;

		// Window destroyed
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
