#ifndef _MAGE2D_WINDOW_H_
#define _MAGE2D_WINDOW_H_

#include <windows.h>    
#include <windowsx.h>   
#include "Types.h"      
#include <string>       

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

	HINSTANCE AppId() const;											// Returns the application instance handle
	HWND Id() const;													// Returns the window handle
	int Width() const;													// Returns the current window width
	int Height() const;													// Returns the current window height

	void Icon(const uint icon);											// Sets the window icon
	void Cursor(const uint cursor);										// Sets the window cursor
	void Title(const std::string& title);								// Sets the window title
	void Size(int width, int height);									// Sets the window size (width and height)
	void Mode(int mode);												// Sets the window mode (full screen/windowed)

	int Mode() const;													// Returns the current window mode (full screen/windowed)
	float CenterX() const;												// Returns the window center on the x-axis
	float CenterY() const;												// Returns the window center on the y-axis
	std::string Title() const;											// Returns the window title

	void HideCursor(bool hide);											// Enables or disables cursor display
	void Close() const;														// Closes the window immediately

	bool KeyDown(int vkcode) const;										// Checks if a key/button is pressed
	bool KeyUp(int vkcode) const;										// Checks if a key/button is released
	int MouseX() const;													// Returns the mouse position on the x-axis
	int MouseY() const;													// Returns the mouse position on the y-axis

	COLORREF Color() const;												// Returns the window background color
	void Color(int r, int g, int b);									// Sets the window background color
	void Print(const std::string& text, int x, int y, COLORREF color) const;// Displays text using the Windows library (slow)
	bool Create();														// Creates the window with the attribute values

	// Windows event handling
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

// Inline member functions
inline HINSTANCE Window::AppId() const
{
	return m_hInstance;
}

inline HWND Window::Id() const
{
	return m_windowHandle;
}

inline int Window::Width() const
{
	return m_windowWidth;
}

inline int Window::Height() const
{
	return m_windowHeight;
}

inline void Window::Icon(const uint icon)
{
	m_windowIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(icon));
}

inline void Window::Cursor(const uint cursor)
{
	m_windowCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(cursor));
}

inline void Window::Title(const std::string& title)
{
	m_windowTitle = title;
}

inline int Window::Mode() const
{
	return m_windowMode;
}

inline float Window::CenterX() const
{
	return m_windowCenterX;
}

inline float Window::CenterY() const
{
	return m_windowCenterY;
}

inline std::string Window::Title() const
{
	return m_windowTitle;
}

inline void Window::HideCursor(bool hide)
{
	ShowCursor(!hide);
}

inline void Window::Close() const
{
	PostMessage(m_windowHandle, WM_DESTROY, 0, 0);
}

inline bool Window::KeyDown(int vkcode) const
{
	return m_windowKeys[vkcode];
}

inline bool Window::KeyUp(int vkcode) const
{
	return !m_windowKeys[vkcode];
}

inline int Window::MouseX() const
{
	return m_windowMouseX;
}

inline int Window::MouseY() const
{
	return m_windowMouseY;
}

inline COLORREF Window::Color() const
{
	return m_windowColor;
}

inline void Window::Color(int r, int g, int b)
{
	m_windowColor = RGB(r, g, b);
}

#endif
