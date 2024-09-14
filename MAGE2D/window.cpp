#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include "resources.h"

// Constants for window dimensions
const int DEFAULT_WINDOW_WIDTH = 960;
const int DEFAULT_WINDOW_HEIGHT = 540;

const LPCWSTR WINDOW_CLASS_NAME = L"BasicWindow";
const LPCWSTR WINDOW_TITLE = L"MAGE2D";

// Global variables
int windowWidth = DEFAULT_WINDOW_WIDTH;
int windowHeight = DEFAULT_WINDOW_HEIGHT;
bool isWindowFullScreen = false;            // Determines if the window is in full screen mode

const unsigned MAX_INPUT_LENGTH = 64;
wchar_t msg[MAX_INPUT_LENGTH] = { 0 };
bool virtualKeysCodes[256] = { 0 };        // States of the keyboard keys

int mouseX = 0, mouseY = 0;
int mouseWheel = 0;
bool mousLeftButton = false;
bool mouseRightButton = false;
bool mouseMiddleButton = false;

// Function prototypes
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
HWND CreateAppWindow(HINSTANCE hInstance, int nCmdShow);
bool InitWindowClass(HINSTANCE hInstance);
void AdjustAndCenterWindow(HWND hwnd);

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
	HDC hdc;			// represents the graphics device
	PAINTSTRUCT ps{};	// holds the invalidated region of the window
	RECT rect{};		// rectangular region

	switch (message)
	{
		// window loses focus
	case WM_KILLFOCUS:
		wcscpy_s(msg, MAX_INPUT_LENGTH, L"Bye bye!");
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_SETFOCUS:
		wcscpy_s(msg, MAX_INPUT_LENGTH, L"Welcome back!");
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

		// process key presses
	case WM_KEYDOWN:
		virtualKeysCodes[wParam] = true;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

		// process key releases
	case WM_KEYUP:
		virtualKeysCodes[wParam] = false;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_MOUSEMOVE:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		return 0;

	case WM_MOUSEWHEEL:
		mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_LBUTTONDOWN:
		mousLeftButton = true;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_LBUTTONUP:
		mousLeftButton = false;
		return 0;

	case WM_MBUTTONDOWN:
		mouseMiddleButton = true;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_MBUTTONUP:
		mouseMiddleButton = false;
		return 0;
	case WM_RBUTTONDOWN:
		mouseRightButton = true;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_RBUTTONUP:
		mouseRightButton = false;
		return 0;

	case WM_PAINT:
		hdc = GetDC(hwnd);
		SetPixel(hdc, mouseX, mouseY, RGB(0, 0, 0));
		SetPixel(hdc, mouseX + 1, mouseY + 1, RGB(0, 0, 0));
		SetPixel(hdc, mouseX - 1, mouseY - 1, RGB(0, 0, 0));
		SetPixel(hdc, mouseX - 1, mouseY + 1, RGB(0, 0, 0));
		SetPixel(hdc, mouseX + 1, mouseY - 1, RGB(0, 0, 0));
		SetPixel(hdc, mouseX, mouseY + 1, RGB(0, 0, 0));
		SetPixel(hdc, mouseX, mouseY - 1, RGB(0, 0, 0));
		SetPixel(hdc, mouseX + 1, mouseY, RGB(0, 0, 0));
		SetPixel(hdc, mouseX - 1, mouseY, RGB(0, 0, 0));
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
