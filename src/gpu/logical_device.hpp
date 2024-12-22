#pragma once

#include "src/gpu/gpu.hpp"

#include "src/gpu/physical_device.hpp"


namespace gpu {

VkQueue get_graphics_queue(const gpu::LightlyDevice& physical_device, VkDevice logical_device);
LIGHTLY_API VkDevice create_logical_device(const gpu::LightlyDevice& physical_device, const std::vector<const char*>& required_device_extensions);

}
