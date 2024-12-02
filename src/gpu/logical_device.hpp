#pragma once

#include "src/gpu/gpu.hpp"

#include "src/gpu/physical_device.hpp"


namespace gpu {
namespace logical_device {

LIGHTLY_API VkDevice create_logical_device(const gpu::physical_device::LightlyDevice& physical_device, const std::vector<const char*>& required_device_extensions);

}
}
