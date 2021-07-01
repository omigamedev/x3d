#pragma once

#include <memory>
#include <string>

struct WindowState;
struct WindowProcParams;

class Window
{
    std::unique_ptr<WindowState> m_state;
public:
    Window();
    Window(int width, int height, const std::wstring& title);
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    ~Window();
    bool Create(int width, int height, const std::wstring& title);
    void Destroy();
    const WindowState& State() const;
private:
    void HandleProc(WindowProcParams& params);
    friend void HandleProcBridge(Window* wnd, WindowProcParams& params);
};

