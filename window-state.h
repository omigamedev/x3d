#pragma once

#include <windows.h>

struct WindowState
{
    SIZE size;
    HWND hWnd = NULL;
    HINSTANCE hInst = NULL;
};


