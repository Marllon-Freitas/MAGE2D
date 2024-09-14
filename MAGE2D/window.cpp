#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include "resources.h"

// Function prototypes
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
HWND CreateAppWindow(HINSTANCE hInstance, int nCmdShow);
bool InitWindowClass(HINSTANCE hInstance);
void AdjustAndCenterWindow(HWND hwnd);

// Constants for window dimensions
const int DEFAULT_WINDOW_WIDTH = 960;
const int DEFAULT_WINDOW_HEIGHT = 540;

const LPCWSTR WINDOW_CLASS_NAME = L"BasicWindow";
const LPCWSTR WINDOW_TITLE = L"MAGE2D";

// Global variables
int windowWidth = DEFAULT_WINDOW_WIDTH;
int windowHeight = DEFAULT_WINDOW_HEIGHT;
bool isWindowFullScreen = false;            // Determines if the window is in full screen mode

bool virtualKeysCodes[256] = { 0 };         // States of the keyboard keys
HWND hwnd;                                  // Global window handle

// Game functions
// Initializes the game by allocating resources
void GameInit()
{

}

// Updates the game logic
void GameUpdate()
{
	// Exit the game when the ESC key is pressed
	if (virtualKeysCodes[VK_ESCAPE]) PostMessage(hwnd, WM_DESTROY, 0, 0);
}

// Draws the next frame of the game
void GameDraw()
{

}

// Finalizes the game by deallocating resources
void GameFinalize()
{

}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Initialize the window class
	if (!InitWindowClass(hInstance))
	{
		MessageBox(NULL, L"Error creating window class!", WINDOW_TITLE, MB_ICONERROR);
		return 0;
	}

	// Create the application window
	hwnd = CreateAppWindow(hInstance, nCmdShow);
	if (!hwnd)
	{
		MessageBox(NULL, L"Error creating window!", WINDOW_TITLE, MB_ICONERROR);
		return 0;
	}

	// Initialize the game
	GameInit();

	// Message handling for the application window
	MSG msg;
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameUpdate();
			GameDraw();
		}
	} while (msg.message != WM_QUIT);

	GameFinalize();

	// End of program
	return int(msg.wParam);
}

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
	int windowStyle = WS_OVERLAPPED | WS_SYSMENU;

	// Check if the window is in full screen mode
	if (isWindowFullScreen)
	{
		windowWidth = GetSystemMetrics(SM_CXSCREEN);
		windowHeight = GetSystemMetrics(SM_CYSCREEN);
		windowStyle = WS_POPUP;
	}

	// Create the window
	HWND hwnd = CreateWindowEx(
		NULL,                           // extended style
		WINDOW_CLASS_NAME,              // window class
		WINDOW_TITLE,                   // window title
		windowStyle,                    // window style
		CW_USEDEFAULT,                  // initial x position
		CW_USEDEFAULT,                  // initial y position
		windowWidth,                    // initial width
		windowHeight,                   // initial height
		NULL,                           // parent window handle
		NULL,                           // menu handle
		hInstance,                      // application instance handle
		NULL                            // creation parameters
	);

	if (!hwnd)
		return NULL;

	if (!isWindowFullScreen)
	{
		AdjustAndCenterWindow(hwnd);
	}

	// Show and update the window
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	return hwnd;
}

// Adjust the window size to fit the desired client area and center it
void AdjustAndCenterWindow(HWND hwnd)
{
	// Adjust the window size to fit the desired client area
	RECT winRect = { 0, 0, windowWidth, windowHeight };
	AdjustWindowRectEx(&winRect, GetWindowStyle(hwnd), GetMenu(hwnd) != NULL, GetWindowExStyle(hwnd));

	// Recalculate the position to center the window with the adjusted size
	int windowPosX = (GetSystemMetrics(SM_CXSCREEN) / 2) - ((winRect.right - winRect.left) / 2);
	int windowPosY = (GetSystemMetrics(SM_CYSCREEN) / 2) - ((winRect.bottom - winRect.top) / 2);

	// Resize and move the window
	MoveWindow(
		hwnd,                           // window handle
		windowPosX,                     // x position
		windowPosY,                     // y position
		winRect.right - winRect.left,   // width
		winRect.bottom - winRect.top,   // height
		TRUE                            // repaint
	);
}

// Window procedure
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = nullptr;			// Represents the graphics device
	PAINTSTRUCT ps{};			// Holds the invalidated region of the window
	RECT rect{};				// Rectangular region

	switch (message)
	{
		// Process key presses
	case WM_KEYDOWN:
		virtualKeysCodes[wParam] = true;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

		// Process key releases
	case WM_KEYUP:
		virtualKeysCodes[wParam] = false;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
