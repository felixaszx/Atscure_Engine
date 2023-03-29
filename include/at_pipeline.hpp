#ifndef AT_PIPELINE_HPP
#define AT_PIPELINE_HPP

#include "at_device.hpp"
#include "at_shader.hpp"

namespace ats
{
    class DescriptorLayout : iMultiType(VkDescriptorSetLayout)
    {
      private:
        std::vector<VkDescriptorSetLayoutBinding> bindings_{};

      public:
        void add_binding(uint32_t binding, uint32_t count, VkDescriptorType type, VkShaderStageFlags stage);

        void create(VkDevice device);
        void destroy(VkDevice device);
    };

    class PipelineLayout : iMultiType(VkPipelineLayout)
    {
      private:
        std::vector<VkDescriptorSetLayout> layouts_{};
        std::vector<VkPushConstantRange> ranges_{};

      public:
        void add_layout(VkDescriptorSetLayout layout);
        void add_constant(uint32_t offset, uint32_t size, VkShaderStageFlags stage);

        void create(VkDevice device);
        void destroy(VkDevice device);
    };

    class GraphicPipeline : iMultiType(VkPipeline)
    {
      private:
        std::vector<VkPipelineColorBlendAttachmentState> color_blends_{};
        std::vector<VkDynamicState> dynamic_states_{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        VkPipelineVertexInputStateCreateInfo vertex_input_ //
            {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
        VkPipelineInputAssemblyStateCreateInfo input_assembly_ //
            {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
        VkPipelineViewportStateCreateInfo viewport_state_ //
            {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
        VkPipelineRasterizationStateCreateInfo rasterizer_ //
            {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
        VkPipelineMultisampleStateCreateInfo multisample_ //
            {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
        VkPipelineDepthStencilStateCreateInfo depth_state_ //
            {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};

      public:
        ShaderStages stages_;

        GraphicPipeline(uint32_t stages = 2);

        void set_vertex_states(const std::vector<VkVertexInputBindingDescription>& binding,
                               const std::vector<VkVertexInputAttributeDescription>& attribute);
        void add_dynamic_state(VkDynamicState dynamic_state);
        void add_dynamic_states(std::vector<VkDynamicState> dynamic_states);
        void set_view_port_state(uint32_t view_count, uint32_t scissor_count);
        void set_rasterizer(VkPolygonMode mode, VkCullModeFlags culling);
        void set_multisample() = delete; // this is disable in current deffered pipeline
        void add_color_blending(VkPipelineColorBlendAttachmentState blending);
        void set_depth_stencil(bool depth_test, bool stencil_test, VkCompareOp compare_op = VK_COMPARE_OP_LESS);

        void create(VkDevice device, VkPipelineLayout layout, VkRenderPass render_pass, uint32_t subpass);
        void destroy(VkDevice device);
    };
}; // namespace ats

#endif // AT_PIPELINE_HPP
