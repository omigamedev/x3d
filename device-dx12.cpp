#include "device-dx12.h"
#include "device-dx12-state.h"
#include "swapchain-dx12.h"
#include "window.h"
#include "window-state.h"

#include <stdexcept>

DeviceDX12::DeviceDX12(const DeviceFeatures& features, const Window& window)
{
    m_state = std::make_unique<DeviceDX12State>();
    if (!Create(features, window))
        throw std::runtime_error("Could not create DeviceVK in ctor.");
}

DeviceDX12::~DeviceDX12()
{
    m_state = std::make_unique<DeviceDX12State>();
}

void DeviceDX12::Flush()
{

}

std::shared_ptr<Swapchain> DeviceDX12::CreateSwapchain()
{
    return nullptr;
}

std::shared_ptr<Buffer> DeviceDX12::CreateBuffer()
{
    return nullptr;
}

DeviceDX12State& DeviceDX12::State()
{
    return *m_state;
}

bool DeviceDX12::Create(const DeviceFeatures& features, const Window& window)
{
    m_state->hWnd = window.State().hWnd;
    m_state->window_size = window.State().size;
    if (features.debug)
    {
        EnableDebugLayer();
    }
    FindDevice(features);
    if (features.debug)
    {
        SetupDebugMessages();
    }
    return true;
}

bool DeviceDX12::FindDevice(const DeviceFeatures& features)
{
    ComPtr<IDXGIFactory2> factory;
    if (!SUCCEEDED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory))))
        return false;
    ComPtr<IDXGIAdapter> adapter;
    ComPtr<ID3D12Device> device;
    for (int i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
    {
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);
        if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device))))
        {
            m_state->device = device;
            return true;
        }
    }
    return false;
}

bool DeviceDX12::EnableDebugLayer()
{
    ComPtr<ID3D12Debug> debug;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
    {
        debug->EnableDebugLayer();

        ComPtr<ID3D12Debug1> debug1;
        if (SUCCEEDED(debug.As(&debug1)))
        {
            debug1->SetEnableGPUBasedValidation(TRUE);
        }
    }
    return true;
}

bool DeviceDX12::SetupDebugMessages()
{
    ComPtr<ID3D12InfoQueue> info_queue;
    if (SUCCEEDED(m_state->device.As(&info_queue)))
    {
        info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_MESSAGE, true);
        info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
        info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_INFO, true);
        info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        return true;
    }
    return false;
}
