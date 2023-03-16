#ifndef AT_BUFFER_HPP
#define AT_BUFFER_HPP

#include "at_device.hpp"

namespace ats
{
    using BufferBase = MultiType<VkBuffer, VkDeviceMemory, VmaAllocation>;
    
};

#endif // AT_BUFFER_HPP
