#include "PlatformDll/Graphics/Vulkan/VulkanRenderDevice.h"

#include "vulkan/vulkan.h"

#define __get_command_buffer_internal( data, cb )\
	(se_slot_array_get_ptr(((render_data_t*)data)->command_buffers, cb.id))

#define __push_command( cbh, op_code, data, ... )\
do {\
	command_buffer_t* cb = __get_command_buffer_internal( data, cbh );\
	se_byte_buffer_write( &cb->commands, u32, op_code );\
	__VA_ARGS__\
	cb->num_commands++;\
} while (0)

namespace SteelEngine { namespace Platform { namespace Vulkan {

    typedef enum op_code
    {
        op_set_depth_enabled,
        op_bind_shader
    } op_code;

    typedef struct command_buffer_t
    {
        u32 num_commands;
        se_byte_buffer commands;
    } command_buffer_t;

    se_slot_array_decl(command_buffer_t);

    typedef struct render_data_t
    {
        se_slot_array(command_buffer_t) command_buffers;
        VkInstance instance;
    } render_data_t;

    void __reset_command_buffer_internal(command_buffer_t* cb)
    {
        se_byte_buffer_clear(&cb->commands);

        cb->num_commands = 0;
    }

    void reset_command_buffer(void* data, Graphics::se_resource_command_buffer cb_handle)
    {
        command_buffer_t* cb = __get_command_buffer_internal(data, cb_handle);

        __reset_command_buffer_internal(cb);
    }

    void set_depth_enabled(void* data, Graphics::se_resource_command_buffer cb_handle, u64 depth)
    {
        __push_command(cb_handle, op_set_depth_enabled, data,
            {
                se_byte_buffer_write(&cb->commands, u64, depth);
            });
    }

    void bind_shader(void* data, Graphics::se_resource_command_buffer cb_handle, Graphics::se_resource_shader s_handle)
    {
        // __push_command(cb_handle, op_bind_shader, data,
        //     {
        //         se_byte_buffer_write(&cb->commands, b32, s_handle.);
        //     });
    }

    void submit_command_buffer(void* data, Graphics::se_resource_command_buffer cb_handle)
    {
        command_buffer_t* cb = __get_command_buffer_internal(data, cb_handle);

        se_byte_buffer_seek_to_beg(&cb->commands);

        u32 num_commands = cb->num_commands;

        se_for_range_i(num_commands)
        {
            op_code opCode = (op_code)se_byte_buffer_read(&cb->commands, u32);

            switch(opCode)
            {
            case op_set_depth_enabled:
            {
                u32 enabled = se_byte_buffer_read(&cb->commands, u64);

                printf("%i\n", enabled);
            } break;
            case op_bind_shader:
            {
                u32 id = se_byte_buffer_read(&cb->commands, u32);

                printf("%i\n", id);
            } break;
            default:
            {

            } break;
            }
        }

        __reset_command_buffer_internal(cb);
    }

    Graphics::se_resource_command_buffer construct_command_buffer(void* data)
    {
        render_data_t* _data = (render_data_t*)data;
        command_buffer_t _r = {};
        u32 cb_handle = se_slot_array_insert(_data->command_buffers, _r);
        command_buffer_t* cb = se_slot_array_get_ptr(_data->command_buffers, cb_handle);

        // Initialize command buffer 
        cb->num_commands = 0;
        cb->commands = se_byte_buffer_new();

        Graphics::se_resource_command_buffer handle = { 0 };
        handle.id = cb_handle;

        return handle;
    }

    bool init(Graphics::RenderDeviceDOD* renderDevice)
    {
        render_data_t* data = (render_data_t*)malloc(sizeof(render_data_t));

	    renderDevice->data = data;

    //     VkApplicationInfo applicationInfo = {};
    //     applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    //     applicationInfo.pApplicationName = "TestVulkan";
    //     applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    //     applicationInfo.pEngineName = "SteelEngine";
    //     applicationInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    //     applicationInfo.apiVersion = VK_API_VERSION_1_2;

    //     VkInstanceCreateInfo createInfo = {};
    //     createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    //     createInfo.pApplicationInfo = &applicationInfo;
    // // TODO: 
    //     // createInfo.enabledExtensionCount = static_cast<uint32_t>(m_Extensions.size());
    //     // createInfo.ppEnabledExtensionNames = m_Extensions.data();

    //     if(vkCreateInstance(&createInfo, NULL, &data->instance) != VK_SUCCESS)
    //     {
    //         return false;
    //     }

        data->command_buffers = se_slot_array_new(command_buffer_t);

        return true;
    }

    void update(Graphics::RenderDeviceDOD* renderDevice)
    {

    }

    Graphics::RenderDeviceDOD* render_device_construct()
    {
        Graphics::RenderDeviceDOD* r = (Graphics::RenderDeviceDOD*)malloc(sizeof(Graphics::RenderDeviceDOD));

        r->init = &init;
        r->update = &update;

        r->reset_command_buffer =		&reset_command_buffer;
        r->submit_command_buffer =		&submit_command_buffer;
        r->construct_command_buffer =	&construct_command_buffer;
        r->bind_shader =				&bind_shader;

        return r;
    }

}}}