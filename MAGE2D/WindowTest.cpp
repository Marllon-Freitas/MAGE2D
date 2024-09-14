#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include "resources.h"

// Constants for window dimensions
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;
const LPCWSTR WINDOW_CLASS_NAME = L"BasicWindow";
const LPCWSTR WINDOW_TITLE = L"Application";

// Function prototypes
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
bool InitWindowClass(HINSTANCE hInstance);
HWND CreateAppWindow(HINSTANCE hInstance, int nCmdShow);

//--------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Initialize the window class
	if (!InitWindowClass(hInstance))
	{
		MessageBox(NULL, L"Error creating window class!", WINDOW_TITLE, MB_ICONERROR);
		return 0;
	}

	// Create the application window
	HWND hwnd = CreateAppWindow(hInstance, nCmdShow);
	if (!hwnd)
	{
		MessageBox(NULL, L"Error creating window!", WINDOW_TITLE, MB_ICONERROR);
		return 0;
	}

	// Message handling for the application window
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// End of program
	return int(msg.wParam);
}

//--------------------------------------------------------------------------------

// Initialize the window class
bool InitWindowClass(HINSTANCE hInstance)
{
	WNDCLASS wndclass = {};
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WinProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = WINDOW_CLASS_NAME;

	return RegisterClass(&wndclass);
}

// Create the application window
HWND CreateAppWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Calculate the initial position to center the window on the screen
	int windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - WINDOW_WIDTH / 2;
	int windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - WINDOW_HEIGHT / 2;

	// Create the window
	HWND hwnd = CreateWindowEx(
		NULL,                           // extended style
		WINDOW_CLASS_NAME,              // window class
		WINDOW_TITLE,                   // window title
		WS_OVERLAPPED | WS_SYSMENU,     // window style
		windowPosX,                     // initial x position
		windowPosY,                     // initial y position
		WINDOW_WIDTH,                   // initial width
		WINDOW_HEIGHT,                  // initial height
		NULL,                           // parent window handle
		NULL,                           // menu handle
		hInstance,                      // application instance handle
		NULL                            // creation parameters
	);

	if (!hwnd)
		return NULL;

	// Adjust the window size to fit the desired client area
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRectEx(&winRect, GetWindowStyle(hwnd), GetMenu(hwnd) != NULL, GetWindowExStyle(hwnd));

	// Recalculate the position to center the window with the adjusted size
	windowPosX = (GetSystemMetrics(SM_CXSCREEN) / 2) - ((winRect.right - winRect.left) / 2);
	windowPosY = (GetSystemMetrics(SM_CYSCREEN) / 2) - ((winRect.bottom - winRect.top) / 2);

	// Resize and move the window
	MoveWindow(
		hwnd,                           // window handle
		windowPosX,                     // x position
		windowPosY,                     // y position
		winRect.right - winRect.left,   // width
		winRect.bottom - winRect.top,   // height
		TRUE                            // repaint
	);

	// Show and update the window
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	return hwnd;
}

//--------------------------------------------------------------------------------

// Window procedure
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

//--------------------------------------------------------------------------------
