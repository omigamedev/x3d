#pragma once

#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

using namespace Microsoft::WRL;

struct DeviceDX12State
{
    HWND hWnd;
    SIZE window_size;
    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12CommandQueue> queue;
};


