#pragma once 

class Window;
class VulkanContext;

namespace vkb {
    struct Instance;
}

namespace setUpContextVulkan {
    void init_instance(Window& window, VulkanContext& context, bool bUseValidationLayers);
    void init_resources(VulkanContext& context, vkb::Instance& vkb_inst);
}

//vk_boot