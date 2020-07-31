#include "RuntimeReflectionDll/RuntimeReflection.h"

#define declare_render_data(T, ...) \
    typedef struct T \
        __VA_ARGS__ \
    T; \
    se_slot_array_decl(T)

#define __get_command_buffer_internal(data, cb) \
	(se_slot_array_get_ptr(((runtime_reflection_data_t*)data)->command_buffers, cb.id))

namespace SteelEngine {

    declare_render_data(command_buffer_t,
    {
        u32 num_commands;
        se_byte_buffer commands;
    });

    typedef struct runtime_reflection_data_t
    {
        se_slot_array(command_buffer_t) command_buffers;
    } runtime_reflection_data_t;

    void __runtime_reflection_reset_command_buffer_internal(command_buffer_t* cb)
    {
        se_byte_buffer_clear(&cb->commands);

        cb->num_commands = 0;
    }

    void runtime_reflection_reset_command_buffer(void* data, se_resource_command_buffer cb_handle)
    {
        command_buffer_t* cb = __get_command_buffer_internal(data, cb_handle);

        __runtime_reflection_reset_command_buffer_internal(cb);
    }

    void runtime_reflection_submit_command_buffer(void* data, se_resource_command_buffer cb_handle)
    {

    }

    se_resource_command_buffer runtime_reflection_construct_command_buffer(void* data)
    {
        runtime_reflection_data_t* _data = (runtime_reflection_data_t*)data;
        command_buffer_t _cb = {};
        u32 cb_handle = se_slot_array_insert(_data->command_buffers, _cb);
        command_buffer_t* cb = se_slot_array_get_ptr(_data->command_buffers, cb_handle);

        cb->num_commands = 0;
        cb->commands = se_byte_buffer_new();

        se_resource_command_buffer handle = {};
        handle.id = cb_handle;

        return handle;
    }

    bool runtime_reflection_init(RuntimeReflectionDOD* rr)
    {
        runtime_reflection_data_t* data = (runtime_reflection_data_t*)malloc(sizeof(runtime_reflection_data_t));

	    rr->data = data;

        data->command_buffers = se_slot_array_new(command_buffer_t);
    }

}

SteelEngine::RuntimeReflectionDOD* runtime_reflection_construct()
{
    SteelEngine::RuntimeReflectionDOD* r = (SteelEngine::RuntimeReflectionDOD*)malloc(sizeof(SteelEngine::RuntimeReflectionDOD));

    r->init = &SteelEngine::runtime_reflection_init;

    r->construct_command_buffer = SteelEngine::runtime_reflection_construct_command_buffer;
    r->reset_command_buffer = SteelEngine::runtime_reflection_reset_command_buffer;
    r->submit_command_buffer = SteelEngine::runtime_reflection_submit_command_buffer;

    return r;
}