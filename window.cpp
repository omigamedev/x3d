#include "window.h"
#include "window-state.h"

#include <stdexcept>

struct WindowProcParams
{
    HWND hWnd;
    UINT msg;
    WPARAM wp;
    LPARAM lp;
    LRESULT result = 0;
};

static void HandleProcBridge(Window* wnd, WindowProcParams& params)
{
    wnd->HandleProc(params);
}

static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    if (msg == WM_CREATE)
    {
        if (auto cs = reinterpret_cast<CREATESTRUCT*>(lp))
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
    }

    if (auto thiz = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)))
    {
        WindowProcParams params{ hWnd, msg, wp, lp };
        HandleProcBridge(thiz, params);
        return params.result;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}

Window::Window(int width, int height, const std::wstring& title)
{
    m_state = std::make_unique<WindowState>();
    if (!Create(width, height, title))
        throw std::runtime_error("Could not create the window in ctor.");
}

Window::Window()
{
    m_state = std::make_unique<WindowState>();
}

Window::~Window()
{
    Destroy();
}

bool Window::Create(int width, int height, const std::wstring& title)
{
    m_state->hInst = GetModuleHandle(NULL);
    m_state->size = { width, height };

    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WinProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_state->hInst;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = TEXT("X3DWindowClass");

    if (!RegisterClass(&wc))
        return false;

    RECT r = { 0, 0, width, height };
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
    m_state->hWnd = CreateWindow(wc.lpszClassName, title.c_str(),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        r.right - r.left, r.bottom - r.top, NULL, NULL, wc.hInstance, this);

    return m_state->hWnd != NULL;
}

void Window::Destroy()
{
    if (m_state && m_state->hWnd)
        CloseWindow(m_state->hWnd);
}

const WindowState& Window::State() const
{
    return *m_state;
}

void Window::HandleProc(WindowProcParams& params)
{
    switch (params.msg)
    {
    case WM_CREATE:
        return;
    case WM_CLOSE:
        PostQuitMessage(EXIT_SUCCESS);
        return;
    }
    params.result = DefWindowProc(params.hWnd, params.msg, params.wp, params.lp);
}
