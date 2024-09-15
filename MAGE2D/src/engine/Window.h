#ifndef _MAGE2D_WINDOW_H_
#define _MAGE2D_WINDOW_H_

#include <windows.h>    
#include <string>       
#include <windowsx.h>

#include "Types.h"    

enum WindowModes { FULLSCREEN, WINDOWED };

class Window
{
private:
	HINSTANCE   m_hInstance;											// Application instance handle
	HWND        m_windowHandle;											// Window handle

	int         m_windowWidth;											// Window width
	int         m_windowHeight;											// Window height

	HICON       m_windowIcon;											// Window icon
	HCURSOR     m_windowCursor;											// Window cursor
	COLORREF    m_windowColor;											// Window background color
	std::string m_windowTitle;											// Window title
	DWORD       m_windowStyle;											// Window style

	int         m_windowMode;											// Window mode (full screen or windowed)

	int         m_windowPosX;											// Initial window position on the x-axis
	int         m_windowPosY;											// Initial window position on the y-axis
	float       m_windowCenterX;										// Window center on the x-axis
	float       m_windowCenterY;										// Window center on the y-axis

	static bool m_windowKeys[256];										// Keyboard key states
	static int  m_windowMouseX;											// Mouse position on the x-axis
	static int  m_windowMouseY;											// Mouse position on the y-axis

public:
	Window();															// Window constructor

	HINSTANCE GetAppId() const;											// Returns the application instance handle
	HWND GetWindowId() const;											// Returns the window handle
	int GetWidth() const;												// Returns the current window width
	int GetHeight() const;												// Returns the current window height

	void SetIcon(const uint icon);										// Sets the window icon
	void SetCursor(const uint cursor);									// Sets the window cursor
	void SetTitle(const std::string& title);							// Sets the window title
	void SetSize(int width, int height);								// Sets the window size (width and height)
	void SetMode(int mode);												// Sets the window mode (full screen/windowed)

	int GetMode() const;												// Returns the current window mode (full screen/windowed)
	float GetCenterX() const;											// Returns the window center on the x-axis
	float GetCenterY() const;											// Returns the window center on the y-axis
	std::string GetTitle() const;										// Returns the window title

	void ShowCursor(bool show);											// Enables or disables cursor display
	void CloseWindow() const;											// Closes the window immediately

	bool IsKeyDown(int vkcode) const;									// Checks if a key/button is pressed
	bool IsKeyUp(int vkcode) const;										// Checks if a key/button is released
	int GetMouseX() const;												// Returns the mouse position on the x-axis
	int GetMouseY() const;												// Returns the mouse position on the y-axis

	COLORREF GetBackgroundColor() const;								// Returns the window background color
	void SetBackgroundColor(int r, int g, int b);						// Sets the window background color
	void Print(const std::string& text, int x, int y, COLORREF color) const;// Displays text using the Windows library (slow)
	bool Create();														// Creates the window with the attribute values

	// Windows event handling
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

// Inline member functions

// Returns application id
inline HINSTANCE Window::GetAppId() const
{
	return m_hInstance;
}

// Returns the game window id
inline HWND Window::GetWindowId() const
{
	return m_windowHandle;
}

// Return the current window width
inline int Window::GetWidth() const
{
	return m_windowWidth;
}

// Return the current window height
inline int Window::GetHeight() const
{
	return m_windowHeight;
}

// ---------------------------------------------------------------------------------

// sets the window icon
inline void Window::SetIcon(const uint icon)
{
	m_windowIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(icon));
}

// sets the window cursor
inline void Window::SetCursor(const uint cursor)
{
	m_windowCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(cursor));
}

// sets the window title
inline void Window::SetTitle(const std::string& title)
{
	m_windowTitle = title;
}

// ---------------------------------------------------------------------------------

// returns the current window mode (FULLSCREEN/WINDOWED)
inline int Window::GetMode() const
{
	return m_windowMode;
}

// returns the center of the window on the horizontal axis
inline float Window::GetCenterX() const
{
	return m_windowCenterX;
}

// returns the center of the window on the vertical axis
inline float Window::GetCenterY() const
{
	return m_windowCenterY;
}

// returns the window title
inline std::string Window::GetTitle() const
{
	return m_windowTitle;
}

// ---------------------------------------------------------------------------------
// 
// enables or disables cursor display
inline void Window::ShowCursor(bool show)
{
	::ShowCursor(show);
}

// closes the window immediately
inline void Window::CloseWindow() const
{
	PostMessage(m_windowHandle, WM_DESTROY, 0, 0);
}

// returns true if the key is pressed
inline bool Window::IsKeyDown(int vkcode) const
{
	return m_windowKeys[vkcode];
}

// returns true if the key is released
inline bool Window::IsKeyUp(int vkcode) const
{
	return !m_windowKeys[vkcode];
}

// returns the mouse position on the x-axis
inline int Window::GetMouseX() const
{
	return m_windowMouseX;
}

// returns the mouse position on the y-axis
inline int Window::GetMouseY() const
{
	return m_windowMouseY;
}

// ---------------------------------------------------------------------------------

// returns the window background color
inline COLORREF Window::GetBackgroundColor() const
{
	return m_windowColor;
}

// sets the window background color
inline void Window::SetBackgroundColor(int r, int g, int b)
{
	m_windowColor = RGB(r, g, b);
}

#endif
