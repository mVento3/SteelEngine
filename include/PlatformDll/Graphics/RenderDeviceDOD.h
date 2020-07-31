#pragma once

#include "PlatformDll/containers.h"
#include "Serialization/byte_buffer.h"

#include "Graphics/IRenderer.h"

namespace SteelEngine { namespace Platform { namespace Graphics {

    typedef enum texture_format 
    {
        se_texture_format_rgba8,
        se_texture_format_rgb8,
        se_texture_format_rgba16f
    } texture_format;

    typedef enum texture_filtering
    {
        nearest,
        linear
    } texture_filtering;

    typedef enum texture_wrapping
    {
        repeat,
        mirrored_repeat,
        clamp_to_edge,
        clamp_to_border
    } texture_wrapping;

    typedef struct texture_construction_info
    {
        void* data;
        u16 width;
        u16 height;
        u8 num_components;
        texture_format format;
        texture_filtering min_filter;
        texture_filtering mag_filter;
        texture_wrapping wrap_s;
        texture_wrapping wrap_t;
    } texture_construction_info;

    typedef enum uniform_type
    {
        uniform_type_float = 0,
        uniform_type_int,
        uniform_type_vec2,
        uniform_type_vec3,
        uniform_type_vec4,
        uniform_type_mat4,
        uniform_type_sampler2d
    } uniform_type;

    typedef enum vertex_attribute_type
    {
        vertex_attribute_float4 = 0,
        vertex_attribute_float3,
        vertex_attribute_float2,
        vertex_attribute_float,
        vertex_attribute_uint4,
        vertex_attribute_uint3,
        vertex_attribute_uint2,
        vertex_attribute_uint,
        vertex_attribute_byte4,
        vertex_attribute_byte3,
        vertex_attribute_byte2,
        vertex_attribute_byte,
    } vertex_attribute_type;

    se_declare_resource_type(command_buffer);
    se_declare_resource_type(shader);
    se_declare_resource_type(frame_buffer);
    se_declare_resource_type(texture);
    se_declare_resource_type(uniform);
    se_declare_resource_type(vertex_buffer);
    se_declare_resource_type(index_buffer);
    se_declare_resource_type(vertex_attribute_layout_description);

    struct RenderDeviceDOD
    {
        void* data;

        bool (*init)(RenderDeviceDOD*);
        void (*update)(RenderDeviceDOD*);

        void (*set_view_clear)(void*, se_resource(command_buffer), const glm::vec4&);

        void (*bind_shader)(void*, se_resource(command_buffer), se_resource(shader));
        void (*bind_texture)(void*, se_resource(command_buffer), se_resource(texture), se_resource(uniform), u32);
        void (*bind_frame_buffer)(void*, se_resource(command_buffer), se_resource(frame_buffer));
        void (*bind_frame_buffer_default)(void*, se_resource(command_buffer));
        void (*bind_vertex_buffer)(void*, se_resource(command_buffer), se_resource(vertex_buffer));
        void (*bind_index_buffer)(void*, se_resource(command_buffer), se_resource(index_buffer));
        void (*bind_uniform)(void*, se_resource(command_buffer), se_resource(uniform), const void*);

        void (*draw)(void*, se_resource(command_buffer), u32, u32);
	    void (*draw_indexed)(void*, se_resource(command_buffer), u32, u32);

        void (*update_vertex_data)(void*, se_resource(command_buffer), se_resource(vertex_buffer), const void*, usize);
        void (*update_index_data)(void*, se_resource(command_buffer), se_resource(index_buffer), const void*, usize);

        void (*reset_command_buffer)(void*, se_resource(command_buffer));
        void (*submit_command_buffer)(void*, se_resource(command_buffer));

        u32 (*get_texture_id)(void*, se_resource(texture));

        se_resource(command_buffer) (*construct_command_buffer)(void*);
        se_resource(shader) (*construct_shader)(void*, const char*, const char*);
        se_resource(texture) (*construct_texture)(void*, texture_construction_info&);
        se_resource(frame_buffer) (*construct_frame_buffer)(void*, se_resource(texture));
        se_resource(vertex_buffer) (*construct_vertex_buffer)(void*, vertex_attribute_type*, u32, const void*, usize);
        se_resource(index_buffer) (*construct_index_buffer)(void*, const void*, usize);
        se_resource(uniform) (*construct_uniform)(void*, se_resource(shader), const char*, uniform_type);
    };

    typedef RenderDeviceDOD* (*render_device_construct_callback)(SteelEngine::Graphics::IRenderer::API);

}}}

extern "C" __declspec(dllexport) SteelEngine::Platform::Graphics::RenderDeviceDOD* render_device_construct(SteelEngine::Graphics::IRenderer::API api);