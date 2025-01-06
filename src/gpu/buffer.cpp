#include "src/gpu/buffer.hpp"

namespace gpu {

uint32_t find_memory_type(
    VkPhysicalDevice physical_device, 
    uint32_t type_filter, 
    VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_properties);

    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
        if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

Buffer create_buffer(
    VkDevice device,
    VkPhysicalDevice physical_device,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties)
{
    VkBufferCreateInfo buffer_info {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };

    VkBuffer buffer;
    if (vkCreateBuffer(device, &buffer_info, nullptr, &buffer) != VK_SUCCESS) {
        SPDLOG_ERROR("Failed to create vertex buffer!");
        throw std::runtime_error("Failed to create vertex buffer!");
    }

    // Check 
    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(device, buffer, &mem_requirements);
    
    uint32_t mem_type = find_memory_type(physical_device, mem_requirements.memoryTypeBits, properties);
    VkMemoryAllocateInfo alloc_info {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = mem_requirements.size,
        .memoryTypeIndex = mem_type
    };

    VkDeviceMemory buffer_memory;
    if (vkAllocateMemory(device, &alloc_info, nullptr, &buffer_memory) != VK_SUCCESS) {
        SPDLOG_ERROR("Failed to allocate vertex buffer memory!");
        throw std::runtime_error("Failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(device, buffer, buffer_memory, 0);

    Buffer buffer_s {
        .data = buffer,
        .memory = buffer_memory
    };
    return buffer_s;
}

void copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size, VkCommandPool command_pool, VkDevice device, VkQueue graphics_queue)
{
    VkCommandBufferAllocateInfo alloc_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(device, &alloc_info, &command_buffer);

    VkCommandBufferBeginInfo begin_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    vkBeginCommandBuffer(command_buffer, &begin_info);
        
        VkBufferCopy copy_region {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = size
        };
        vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);

    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submit_info {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &command_buffer
    };

    vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphics_queue);

    vkFreeCommandBuffers(device, command_pool, 1, &command_buffer);
}

Buffer create_vertex_buffer(VkPhysicalDevice physical_device, VkDevice device, std::vector<Vertex> vertices, VkCommandPool command_pool, VkQueue graphics_queue) 
{
    VkDeviceSize buffer_size = sizeof(vertices[0]) * vertices.size();

    Buffer staging_buffer = create_buffer(device, physical_device, buffer_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(device, staging_buffer.memory, 0, buffer_size, 0, &data);
    memcpy(data, vertices.data(), (size_t) buffer_size);
    vkUnmapMemory(device, staging_buffer.memory);

    Buffer buffer = create_buffer(device, physical_device, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    copy_buffer(staging_buffer.data, buffer.data, buffer_size, command_pool, device, graphics_queue);

    vkDestroyBuffer(device, staging_buffer.data, nullptr);
    vkFreeMemory(device, staging_buffer.memory, nullptr);

    return buffer;
}

Buffer create_index_buffer(VkPhysicalDevice physical_device, VkDevice device, std::vector<uint16_t> indices, VkCommandPool command_pool, VkQueue graphics_queue)
{
    VkDeviceSize buffer_size = sizeof(indices[0]) * indices.size();

    Buffer staging_buffer = create_buffer(device, physical_device, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(device, staging_buffer.memory, 0, buffer_size, 0, &data);
    memcpy(data, indices.data(), (size_t) buffer_size);
    vkUnmapMemory(device, staging_buffer.memory);

    Buffer buffer = create_buffer(device, physical_device, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    copy_buffer(staging_buffer.data, buffer.data, buffer_size, command_pool, device, graphics_queue);

    vkDestroyBuffer(device, staging_buffer.data, nullptr);
    vkFreeMemory(device, staging_buffer.memory, nullptr);

    return buffer;
}

UniformBuffer create_uniform_buffer(VkPhysicalDevice physical_device, VkDevice device) 
{
    VkDeviceSize buffer_size = sizeof(UniformBufferObject);

    UniformBuffer ub;

    for (size_t i = 0; i < global::max_frames_in_flight; i++) {
        auto buffer = create_buffer(device, physical_device, buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT); 
        ub.uniform_buffers.push_back(buffer.data);
        ub.uniform_buffers_memory.push_back(buffer.memory);

        vkMapMemory(device, ub.uniform_buffers_memory.back(), 0, buffer_size, 0, &ub.uniform_buffers_mapped.back());
    }

    return ub;
}

} // namespace gpu
