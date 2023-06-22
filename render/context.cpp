#include "context.hpp"

namespace ae
{

    Context::Context(bool enable_debug) {}

    Context::~Context() {}

    VkBool32 VKAPI_CALL Context::debug_cb(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,   //
                                          VkDebugUtilsMessageTypeFlagsEXT message_type,              //
                                          const VkDebugUtilsMessengerCallbackDataEXT* callback_data, //
                                          void* user_data)
    {
        static const char* type[5] = {"WARNING", "ERROR", "INFO", "VERBOSE", "UNDEFINE"};
        int type_index = 4;

        switch (message_severity)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            {
                type_index = 0;
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            {
                type_index = 1;
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            {
                type_index = 2;
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            {
                type_index = 3;
                break;
            }
            default:
            {
            }
        }

        Log("VALIDATION", std::format("<{}> {}\n", type[type_index], callback_data->pMessage));
        return VK_FALSE;
    }
}; // namespace ae