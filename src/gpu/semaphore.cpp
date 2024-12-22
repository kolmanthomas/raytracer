#include "src/gpu/semaphore.hpp"

namespace gpu {

VkSemaphore create_semaphore(VkDevice device)
{
    VkSemaphoreCreateInfo semaphore_info {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };

    VkSemaphore semaphore;
    if (vkCreateSemaphore(device, &semaphore_info, nullptr, &semaphore) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphore!");
    }

    return semaphore;
}

} // namespace gpu
