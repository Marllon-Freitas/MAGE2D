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

void Window::Mode(int mode)
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

void Window::Size(int width, int height)
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
	// Esta função exibe texto na posição (x, y) na tela usando a cor especificada
	// Ela usa a biblioteca Windows (lenta) e deve ser usada apenas para depuração

	// Obter o contexto do dispositivo
	HDC xdc = GetDC(m_windowHandle);

	// Definir a cor do texto
	SetTextColor(xdc, color);

	// Definir o fundo do texto como transparente
	SetBkMode(xdc, TRANSPARENT);

	// Converter std::string para std::wstring
	std::wstring wtext(text.begin(), text.end());

	// Exibir o texto
	TextOut(xdc, x, y, wtext.c_str(), static_cast<int>(wtext.size()));

	// Liberar o contexto do dispositivo
	ReleaseDC(m_windowHandle, xdc);
}

bool Window::Create()
{
	WNDCLASSEX wndClass{};

	// Definir uma classe de janela chamada "GameWindow"
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

	// Registrar "GameWindow"
	if (!RegisterClassEx(&wndClass))
		return false;

	// Criar uma janela baseada na classe "GameWindow"
	m_windowHandle = CreateWindowEx(
		0,                              // Estilos extras
		L"GameWindow",                  // Nome da classe da janela
		std::wstring(m_windowTitle.begin(), m_windowTitle.end()).c_str(), // Título da janela
		m_windowStyle,                  // Estilo da janela
		m_windowPosX, m_windowPosY,     // Posição inicial (x, y)
		m_windowWidth, m_windowHeight,  // Largura e altura da janela
		nullptr,                        // Handle da janela pai
		nullptr,                        // Handle do menu
		m_hInstance,                    // Handle da instância da aplicação
		nullptr);                       // Parâmetros de criação

	// No modo janela, considerar o espaço ocupado pelas bordas e barras de título
	if (m_windowMode == WINDOWED)
	{
		// Retângulo com o tamanho desejado da área do cliente
		RECT winRect = { 0, 0, m_windowWidth, m_windowHeight };

		// Ajustar o tamanho do retângulo
		AdjustWindowRectEx(&winRect,
			GetWindowStyle(m_windowHandle),
			GetMenu(m_windowHandle) != nullptr,
			GetWindowExStyle(m_windowHandle));

		// Atualizar a posição da janela
		m_windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - (winRect.right - winRect.left) / 2;
		m_windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - (winRect.bottom - winRect.top) / 2;

		// Redimensionar a janela com uma chamada para MoveWindow
		MoveWindow(
			m_windowHandle,                  // Handle da janela
			m_windowPosX,                    // Posição x
			m_windowPosY,                    // Posição y
			winRect.right - winRect.left,    // Largura
			winRect.bottom - winRect.top,    // Altura
			TRUE);                           // Repaint
	}

	// Retornar o status da inicialização (bem-sucedida ou não)
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
