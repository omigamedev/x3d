#include "application.h"
#include "device.h"
#include "filesystem.h"

#include <iostream>
#include <vector>
#include <memory>

class ApplicationSample : public ApplicationBase
{
public:
protected:
    void Init(ApplicationConfig& config) override
    {
        config.window_width = 800;
        config.window_height = 600;
        config.window_title = L"X3DEngine";
        config.api = DeviceAPI::Vulkan;
        config.features.debug = true;
        config.features.raytracing = false;
    }
    void Start() override
    {
    }
    void Update(float dt) override
    {
    }
    void Terminate() override
    {
    }
};

int main()
{
    ApplicationSample app;
    return app.Run();
}
