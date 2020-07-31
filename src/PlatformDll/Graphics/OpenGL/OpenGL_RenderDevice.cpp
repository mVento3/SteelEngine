#include "PlatformDll/Graphics/OpenGL/OpenGL_RenderDevice.h"

#include "gl/glew.h"

#define int_2_void_p(i) (void*)(uintptr_t)(i)

#define __get_command_buffer_internal(data, cb) \
	(se_slot_array_get_ptr(((render_data_t*)data)->command_buffers, cb.id))

#define __push_command(cbh, op_code, data, ...) \
do { \
	command_buffer_t* cb = __get_command_buffer_internal(data, cbh); \
	se_byte_buffer_write(&cb->commands, u32, op_code); \
	__VA_ARGS__ \
	cb->num_commands++; \
} while (0)

#define op_case(op_code, ...) \
    case op_code: \
    { \
        __VA_ARGS__ \
    } break

#define declare_render_data(T, ...) \
    typedef struct T \
        __VA_ARGS__ \
    T; \
    se_slot_array_decl(T)

#define __write_uniform_val(bb, type, u_data) \
	se_byte_buffer_write(&bb, type, *((type*)(u_data)));

namespace SteelEngine { namespace Platform { namespace OpenGL {

    typedef enum op_code
    {
        op_set_view_clear,
        op_bind_shader,
        op_bind_texture,
        op_bind_frame_buffer,
        op_bind_uniform,
        op_draw,
        op_draw_indexed,
        op_update_vertex_data,
        op_update_index_data,
        op_bind_vertex_buffer,
        op_bind_index_buffer
    } op_code;

    declare_render_data(command_buffer_t,
    {
        u32 num_commands;
        se_byte_buffer commands;
    });

    declare_render_data(shader_t,
    {
        u32 id;
    });

    declare_render_data(uniform_t,
    {
        Graphics::uniform_type type;
        u32 location;
    });

    declare_render_data(texture_t,
    {
        u16 width;
        u16 height;
        u8 num_components;
        u32 id;
        Graphics::texture_format format;
    });

    declare_render_data(frame_buffer_t,
    {
        u32 fbo;
    });

    declare_render_data(vertex_buffer_t,
    {
        u32 vao;
        u32 vbo;
    });

    declare_render_data(index_buffer_t,
    {
        u32 ibo;
    });

    declare_render_data(vertex_attribute_layout_description_t,
    {
        se_dyn_array(Graphics::vertex_attribute_type) attributes;
    });

    typedef struct render_data_t
    {
        se_slot_array(command_buffer_t) command_buffers;
        se_slot_array(shader_t) shaders;
        se_slot_array(texture_t) textures;
        se_slot_array(uniform_t) uniforms;
        se_slot_array(frame_buffer_t) frame_buffers;
        se_slot_array(vertex_buffer_t) vertex_buffers;
        se_slot_array(index_buffer_t) index_buffers;
        se_slot_array(vertex_attribute_layout_description_t) vertex_layout_descriptions;
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

    void set_view_clear(void* data, Graphics::se_resource_command_buffer cb_handle, const glm::vec4& color)
    {
        __push_command(cb_handle, op_set_view_clear, data,
        {
            se_byte_buffer_write(&cb->commands, glm::vec4, color);
        });
    }

    void bind_shader(void* data, Graphics::se_resource_command_buffer cb_handle, Graphics::se_resource_shader s_handle)
    {
        render_data_t* _data = (render_data_t*)data;

        __push_command(cb_handle, op_bind_shader, data,
        {
            shader_t s = se_slot_array_get(_data->shaders, s_handle.id);

            se_byte_buffer_write(&cb->commands, b32, s.id);
        });
    }

    void bind_frame_buffer(void* data, Graphics::se_resource_command_buffer cb_handle, Graphics::se_resource_frame_buffer fb_handle)
    {
        render_data_t* _data = (render_data_t*)data;

        __push_command(cb_handle, op_bind_frame_buffer, data,
        {
            frame_buffer_t fb = se_slot_array_get(_data->frame_buffers, fb_handle.id);

            se_byte_buffer_write(&cb->commands, b32, fb.fbo);
        });
    }

    void bind_frame_buffer_default(void* data, Graphics::se_resource_command_buffer cb_handle)
    {
        render_data_t* _data = (render_data_t*)data;

        __push_command(cb_handle, op_bind_frame_buffer, data,
        {
            se_byte_buffer_write(&cb->commands, b32, 0);
        });
    }

    void bind_texture(void* data, Graphics::se_resource_command_buffer cb_handle, Graphics::se_resource_texture texture_handle, Graphics::se_resource_uniform u_handle, u32 unit)
    {
        render_data_t* _data = (render_data_t*)data;

        __push_command(cb_handle, op_bind_texture, data,
        {
            texture_t tex = se_slot_array_get(_data->textures, texture_handle.id);
            uniform_t uniform = se_slot_array_get(_data->uniforms, u_handle.id);

            if(uniform.type != Graphics::uniform_type_sampler2d)
            {
                printf("opengl_bind_texture: Must be of uniform type 'gs_uniform_type_sampler2d'");
            }

            se_byte_buffer_write(&cb->commands, u32, tex.id);
            se_byte_buffer_write(&cb->commands, u32, unit);
            se_byte_buffer_write(&cb->commands, u32, uniform.location);
        });
    }

    void draw(void* data, Graphics::se_resource_command_buffer cb_handle, u32 start, u32 count)
    {
        __push_command(cb_handle, op_draw, data,
        {
            se_byte_buffer_write(&cb->commands, u32, start);
            se_byte_buffer_write(&cb->commands, u32, count);
        });
    }

    void draw_indexed(void* data, Graphics::se_resource_command_buffer cb_handle, u32 count, u32 offset)
    {
        __push_command(cb_handle, op_draw_indexed, data,
        {
            se_byte_buffer_write(&cb->commands, u32, count);
            se_byte_buffer_write(&cb->commands, u32, offset);
        });
    }

    void update_vertex_data(void* data, Graphics::se_resource_command_buffer cb_handle, Graphics::se_resource_vertex_buffer vb_handle, const void* vData, usize size)
    {
        render_data_t* _data = (render_data_t*)data;

        __push_command(cb_handle, op_update_vertex_data, data,
        {
            vertex_buffer_t vb = se_slot_array_get(_data->vertex_buffers, vb_handle.id);

            se_byte_buffer_write(&cb->commands, u32, vb.vao);
            se_byte_buffer_write(&cb->commands, u32, vb.vbo);
            se_byte_buffer_write(&cb->commands, u32, size);
            se_byte_buffer_bulk_write(&cb->commands, vData, size);
        });
    }

    void update_index_data(void* data, Graphics::se_resource_command_buffer cb_handle, Graphics::se_resource_index_buffer ib_handle, const void* iData, usize size)
    {
        render_data_t* _data = (render_data_t*)data;

        __push_command(cb_handle, op_update_index_data, data,
        {
            index_buffer_t ib = se_slot_array_get(_data->index_buffers, ib_handle.id);

            se_byte_buffer_write(&cb->commands, u32, ib.ibo);
            se_byte_buffer_write(&cb->commands, u32, size);
            se_byte_buffer_bulk_write(&cb->commands, iData, size);
        });
    }

    void bind_vertex_buffer(void* data, Graphics::se_resource_command_buffer cb_handle, Graphics::se_resource_vertex_buffer vb_handle)
    {
        render_data_t* _data = (render_data_t*)data;

        __push_command(cb_handle, op_bind_vertex_buffer, data,
        {
            vertex_buffer_t vb = se_slot_array_get(_data->vertex_buffers, vb_handle.id);

            se_byte_buffer_write(&cb->commands, u32, vb.vao);
        });
    }

    void bind_index_buffer(void* data, Graphics::se_resource_command_buffer cb_handle, Graphics::se_resource_index_buffer ib_handle)
    {
        render_data_t* _data = (render_data_t*)data;

        __push_command(cb_handle, op_bind_index_buffer, data,
        {
            index_buffer_t ib = se_slot_array_get(_data->index_buffers, ib_handle.id);

            se_byte_buffer_write(&cb->commands, u32, ib.ibo);
        });
    }

    void bind_uniform(void* data, Graphics::se_resource_command_buffer cb_handle, Graphics::se_resource_uniform u_handle, const void* uData)
    {
        render_data_t* _data = (render_data_t*)data;

        __push_command(cb_handle, op_bind_uniform, data,
        {
            uniform_t u = se_slot_array_get(_data->uniforms, u_handle.id);

            se_byte_buffer_write(&cb->commands, u32, u.location);
            se_byte_buffer_write(&cb->commands, u32, u.type);

            switch(u.type)
            {
            case Graphics::uniform_type_int:        __write_uniform_val(cb->commands, s32, uData); break;
            case Graphics::uniform_type_float:      __write_uniform_val(cb->commands, f32, uData); break;
            case Graphics::uniform_type_vec2:       __write_uniform_val(cb->commands, vec2, uData); break;
            case Graphics::uniform_type_vec3:       __write_uniform_val(cb->commands, vec3, uData); break;
            case Graphics::uniform_type_vec4:       __write_uniform_val(cb->commands, vec4, uData); break;
            // case Graphics::uniform_type_mat4:       __write_uniform_val(cb->commands, s32, uData); break;
            case Graphics::uniform_type_sampler2d:  __write_uniform_val(cb->commands, u32, uData); break;
            default:
                break;
            }
        });
    }

    void submit_command_buffer(void* data, Graphics::se_resource_command_buffer cb_handle)
    {
        command_buffer_t* cb = __get_command_buffer_internal(data, cb_handle);
        se_byte_buffer* commands = &cb->commands;

        se_byte_buffer_seek_to_beg(commands);

        u32& num_commands = cb->num_commands;

        se_for_range_i(num_commands)
        {
            op_code opCode = (op_code)se_byte_buffer_read(commands, u32);

            switch(opCode)
            {
                op_case(op_set_view_clear,
                {
                    glm::vec4 color = se_byte_buffer_read(commands, vec4);

                    glClearColor(color.x, color.y, color.z, color.w);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                });
                op_case(op_bind_shader,
                {
                    u32 id = se_byte_buffer_read(commands, u32);

                    glUseProgram(id);
                });
                op_case(op_bind_texture,
                {
                    u32 id = se_byte_buffer_read(commands, u32);
                    u32 unit = se_byte_buffer_read(commands, u32);
                    u32 location = se_byte_buffer_read(commands, u32);

                    glActiveTexture(GL_TEXTURE0 + unit);
                    glBindTexture(GL_TEXTURE_2D, id);
                    glUniform1i(location, unit);
                });
                op_case(op_bind_frame_buffer,
                {
                    u32 fbo = se_byte_buffer_read(commands, u32);

                    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
                });
                op_case(op_draw,
                {
                    u32 start = se_byte_buffer_read(commands, u32);
                    u32 count = se_byte_buffer_read(commands, u32);

                    glDrawArrays(GL_TRIANGLES, start, count);
                });
                op_case(op_draw_indexed,
                {
                    u32 count = se_byte_buffer_read(commands, u32);
                    u32 offset = se_byte_buffer_read(commands, u32);

                    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, int_2_void_p(offset));
                });
                op_case(op_bind_vertex_buffer,
                {
                    u32 vao = se_byte_buffer_read(commands, u32);

                    glBindVertexArray(vao);
                });
                op_case(op_bind_index_buffer,
                {
                    u32 ibo = se_byte_buffer_read(commands, u32);

                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i);
                });
                op_case(op_update_vertex_data,
                {
                    u32 vao = se_byte_buffer_read(commands, u32);
                    u32 vbo = se_byte_buffer_read(commands, u32);
                    u32 data_size = se_byte_buffer_read(commands, u32);

                    void* tmp_data = se_malloc(data_size);
                    memset(tmp_data, 0, data_size);
                    se_byte_buffer_bulk_read(commands, tmp_data, data_size);

                    glBindVertexArray(vao);
                    glBindBuffer(GL_ARRAY_BUFFER, vbo);
                    glBufferData(GL_ARRAY_BUFFER, data_size, tmp_data, GL_STATIC_DRAW);

                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    glBindVertexArray(0);

                    se_free(tmp_data);
                    tmp_data = NULL;
                });
                op_case(op_update_index_data,
                {
                    u32 ibo = se_byte_buffer_read(commands, u32);
                    u32 data_size = se_byte_buffer_read(commands, u32);

                    void* tmp_data = se_malloc(data_size);
                    memset(tmp_data, 0, data_size);
                    se_byte_buffer_bulk_read(&cb->commands, tmp_data, data_size);

                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (u32)ibo);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, tmp_data, GL_STATIC_DRAW);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

                    se_free(tmp_data);
                    tmp_data = NULL;
                });
                op_case(op_bind_uniform,
                {
                    u32 location = se_byte_buffer_read(commands, u32);
                    Graphics::uniform_type type = (Graphics::uniform_type)se_byte_buffer_read(commands, u32);

                    switch(type)
                    {
                    case Graphics::uniform_type_int:
                    {
                        s32 value = se_byte_buffer_read(commands, s32);

                        glUniform1i(location, value);
                    } break;
                    case Graphics::uniform_type_float:
                    {
                        f32 value = se_byte_buffer_read(commands, f32);

                        glUniform1f(location, value);
                    } break;
                    case Graphics::uniform_type_vec2:
                    {
                        vec2 value = se_byte_buffer_read(commands, vec2);

                        glUniform2f(location, value.x, value.y);
                    } break;
                    case Graphics::uniform_type_vec3:
                    {
                        vec3 value = se_byte_buffer_read(commands, vec3);

                        glUniform3f(location, value.x, value.y, value.z);
                    } break;
                    case Graphics::uniform_type_vec4:
                    {
                        vec4 value = se_byte_buffer_read(commands, vec4);

                        glUniform4f(location, value.x, value.y, value.z, value.w);
                    } break;
                    case Graphics::uniform_type_sampler2d:
                    {
                        u32 value = se_byte_buffer_read(commands, u32);

                        glUniform1i(location, value);
                    } break;
                    default:
                        break;
                    }
                });
            }
        }

        __reset_command_buffer_internal(cb);
    }

    Graphics::se_resource_command_buffer construct_command_buffer(void* data)
    {
        render_data_t* _data = (render_data_t*)data;
        command_buffer_t _cb = {};
        u32 cb_handle = se_slot_array_insert(_data->command_buffers, _cb);
        command_buffer_t* cb = se_slot_array_get_ptr(_data->command_buffers, cb_handle);

        cb->num_commands = 0;
        cb->commands = se_byte_buffer_new();

        Graphics::se_resource_command_buffer handle = {};
        handle.id = cb_handle;

        return handle;
    }

    Graphics::se_resource_uniform construct_uniform(void* data, Graphics::se_resource_shader s_handle, const char* name, Graphics::uniform_type type)
    {
        render_data_t* _data = (render_data_t*)data;
        uniform_t _u = {};

        shader_t _s = se_slot_array_get(_data->shaders, s_handle.id);
        u32 location = glGetUniformLocation(_s.id, name);

        _u.location = location;
        _u.type = type;

        u32 u_handle = se_slot_array_insert(_data->uniforms, _u);

        Graphics::se_resource_uniform handle = {};

        handle.id = u_handle;

        return handle;
    }

    void compile_shader(const char* src, u32 id) 
    {
        glShaderSource(id, 1, &src, NULL);
        glCompileShader(id);

        GLint success = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if(success == GL_FALSE)
        {
            GLint max_len = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_len);

            char* log = (char*)se_malloc(max_len);
            memset(log, 0, max_len);

            glGetShaderInfoLog(id, max_len, &max_len, log);
            glDeleteShader(id);

            printf("Opengl::compile_shader::FAILED_TO_COMPILE: %s", log);

            free(log);
            log = NULL;
        }
    }

    void link_shaders(u32 program_id, u32 vert_id, u32 frag_id)
    {
        glAttachShader(program_id, vert_id);
        glAttachShader(program_id, frag_id);
        glLinkProgram(program_id);

        s32 is_linked = 0;
        glGetProgramiv(program_id, GL_LINK_STATUS, (s32*)&is_linked);

        if(is_linked == GL_FALSE)
        {
            GLint max_len = 0;
            glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_len);

            char* log = (char*)se_malloc(max_len);
            memset(log, 0, max_len);
            glGetProgramInfoLog(program_id, max_len, &max_len, log); 

            printf("Fail To Link::link_shaders::%s", log);

            glDeleteProgram(program_id);
            glDeleteShader(vert_id);
            glDeleteShader(frag_id);

            free(log);
            log = NULL;
        }
    }

    Graphics::se_resource_shader construct_shader(void* data, const char* vsSource, const char* fsSource)
    {
        render_data_t* _data = (render_data_t*)data;
        shader_t _s = {};

        _s.id = glCreateProgram();

        u32 vert_id = glCreateShader(GL_VERTEX_SHADER);
        u32 frag_id = glCreateShader(GL_FRAGMENT_SHADER);

        compile_shader(vsSource, vert_id);
        compile_shader(fsSource, frag_id);
        link_shaders(_s.id, vert_id, frag_id);

        u32 s_handle = se_slot_array_insert(_data->shaders, _s);

        Graphics::se_resource_shader handle = {};
        handle.id = s_handle;

        return handle;
    }

    Graphics::se_resource_texture construct_texture(void* data, Graphics::texture_construction_info& info)
    {
        render_data_t* _data = (render_data_t*)data;
        texture_t _t = {};
        
        glGenTextures(1, &_t.id);
        glBindTexture(GL_TEXTURE_2D, _t.id);

        switch(info.format)
        {
        case Graphics::texture_format::se_texture_format_rgb8:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, info.width, info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, info.data);
            break;
        case Graphics::texture_format::se_texture_format_rgba8:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.data);
            break;
        case Graphics::texture_format::se_texture_format_rgba16f:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, info.width, info.height, 0, GL_RGBA, GL_FLOAT, info.data);
            break;
        default:
            break;
        }

        s32 mag_filter = info.mag_filter == Graphics::nearest ? GL_NEAREST : GL_LINEAR;
        s32 min_filter = info.min_filter == Graphics::nearest ? GL_NEAREST : GL_LINEAR;

        s32 texture_wrap_s = info.wrap_s == Graphics::repeat ? GL_REPEAT : 
                            info.wrap_s == Graphics::mirrored_repeat ? GL_MIRRORED_REPEAT : 
                            info.wrap_s == Graphics::clamp_to_edge ? GL_CLAMP_TO_EDGE : 
                            GL_CLAMP_TO_BORDER;
        s32 texture_wrap_t = info.wrap_t == Graphics::repeat ? GL_REPEAT : 
                            info.wrap_t == Graphics::mirrored_repeat ? GL_MIRRORED_REPEAT : 
                            info.wrap_t == Graphics::clamp_to_edge ? GL_CLAMP_TO_EDGE : 
                            GL_CLAMP_TO_BORDER;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

        _t.width = info.width;
        _t.height = info.height;
        _t.num_components = info.num_components;
        _t.format = info.format;

        glBindTexture(GL_TEXTURE_2D, 0);

        u32 t_handle = se_slot_array_insert(_data->textures, _t);

        Graphics::se_resource_texture handle = {};

        handle.id = t_handle;

        return handle;
    }

    Graphics::se_resource_frame_buffer construct_frame_buffer(void* data, Graphics::se_resource_texture t_handle)
    {
        render_data_t* _data = (render_data_t*)data;
        frame_buffer_t _fb = {};
        texture_t _t = se_slot_array_get(_data->textures, t_handle.id);

        glGenFramebuffers(1, &_fb.fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, _fb.fbo);

        GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };

        glDrawBuffers(1, draw_buffers);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, _t.id , 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Error: Frame buffer could not be created.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        u32 _handle = se_slot_array_insert(_data->frame_buffers, _fb);

        Graphics::se_resource_frame_buffer handle = {};

        handle.id = _handle;

        return handle;
    }

    u32 get_byte_size_of_vertex_attribute(Graphics::vertex_attribute_type type)
    {
        u32 byte_size = 0;

        switch(type)
        {
            case Graphics::vertex_attribute_float4:	{ byte_size = sizeof(f32) * 4; } break;
            case Graphics::vertex_attribute_float3:	{ byte_size = sizeof(f32) * 3; } break;
            case Graphics::vertex_attribute_float2:	{ byte_size = sizeof(f32) * 2; } break;
            case Graphics::vertex_attribute_float:	{ byte_size = sizeof(f32) * 1; } break;
            case Graphics::vertex_attribute_uint4:	{ byte_size = sizeof(u32) * 4; } break;
            case Graphics::vertex_attribute_uint3:	{ byte_size = sizeof(u32) * 3; } break;
            case Graphics::vertex_attribute_uint2:	{ byte_size = sizeof(u32) * 2; } break;
            case Graphics::vertex_attribute_uint:	{ byte_size = sizeof(u32) * 1; } break;
        } 

        return byte_size;
    }

    u32 calculate_vertex_size_in_bytes(Graphics::vertex_attribute_type* layout_data, u32 count)
    {
        u32 sz = 0;

        se_for_range_i(count)
        {
            Graphics::vertex_attribute_type type = layout_data[i];
            sz += get_byte_size_of_vertex_attribute(type);
        }

        return sz;
    }

    s32 get_byte_offest(Graphics::vertex_attribute_type* layout_data, u32 index)
    {
        s32 total_offset = 0;

        if(index == 0)
        {
            return total_offset;
        }

        for(u32 i = 0; i < index; ++i)
        {
            total_offset += get_byte_size_of_vertex_attribute(layout_data[i]);
        }

        return total_offset;
    }

    Graphics::se_resource_vertex_buffer construct_vertex_buffer(void* data, Graphics::vertex_attribute_type* layout_data, u32 layout_size, const void* vData, usize vData_size)
    {
        render_data_t* _data = (render_data_t*)data;
        vertex_buffer_t _vb = {};
        u32 vb_handle = se_slot_array_insert(_data->vertex_buffers, _vb);
        vertex_buffer_t* vb = se_slot_array_get_ptr(_data->vertex_buffers, vb_handle);

        glGenVertexArrays(1, &vb->vao);
	    glBindVertexArray(vb->vao);
        glGenBuffers(1, &vb->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vb->vbo);
        glBufferData(GL_ARRAY_BUFFER, vData_size, vData, GL_STATIC_DRAW);

        u32 total_size = calculate_vertex_size_in_bytes(layout_data, layout_size);

        se_for_range_i(layout_size)
        {
            Graphics::vertex_attribute_type type = layout_data[i];
            u32 byte_offset = get_byte_offest(layout_data, i);

            switch (type)
            {
                case Graphics::vertex_attribute_float4: 
                {
                    glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, total_size, int_2_void_p(byte_offset));
                } break;	
                case Graphics::vertex_attribute_float3: 
                {
                    glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, total_size, int_2_void_p(byte_offset));
                } break;	
                case Graphics::vertex_attribute_float2: 
                {
                    glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, total_size, int_2_void_p(byte_offset));
                } break;	
                case Graphics::vertex_attribute_float: 
                {
                    glVertexAttribPointer(i, 1, GL_FLOAT, GL_FALSE, total_size, int_2_void_p(byte_offset));
                } break;	
                case Graphics::vertex_attribute_uint4:
                {
                    glVertexAttribIPointer(i, 4, GL_UNSIGNED_INT, total_size, int_2_void_p(byte_offset));
                } break;	
                case Graphics::vertex_attribute_uint3: 
                {
                    glVertexAttribIPointer(i, 3, GL_UNSIGNED_INT, total_size, int_2_void_p(byte_offset));
                } break;	
                case Graphics::vertex_attribute_uint2: 
                {
                    glVertexAttribIPointer(i, 2, GL_UNSIGNED_INT, total_size, int_2_void_p(byte_offset));
                } break;	
                case Graphics::vertex_attribute_uint: 
                {
                    glVertexAttribIPointer(i, 1, GL_UNSIGNED_INT, total_size, int_2_void_p(byte_offset));
                } break;	
                case Graphics::vertex_attribute_byte: 
                {
                    glVertexAttribPointer(i, 1, GL_UNSIGNED_BYTE, GL_TRUE, total_size, int_2_void_p(byte_offset));
                } break;	
                case Graphics::vertex_attribute_byte2: 
                {
                    glVertexAttribPointer(i, 2, GL_UNSIGNED_BYTE, GL_TRUE, total_size, int_2_void_p(byte_offset));
                } break;	
                case Graphics::vertex_attribute_byte3: 
                {
                    glVertexAttribPointer(i, 3, GL_UNSIGNED_BYTE, GL_TRUE, total_size, int_2_void_p(byte_offset));
                } break;	
                case Graphics::vertex_attribute_byte4: 
                {
                    glVertexAttribPointer(i, 4, GL_UNSIGNED_BYTE, GL_TRUE, total_size, int_2_void_p(byte_offset));
                } break;	

                default: 
                {

                } break;
            }

            glEnableVertexAttribArray(i);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        Graphics::se_resource_vertex_buffer handle = {};

        handle.id = vb_handle;

        return handle;
    }

    Graphics::se_resource_index_buffer construct_index_buffer(void* data, const void* iData, usize size)
    {
        render_data_t* _data = (render_data_t*)data;
        index_buffer_t _ib = {};
        u32 ib_handle = se_slot_array_insert(_data->index_buffers, _ib);
        index_buffer_t* ib = se_slot_array_get_ptr(_data->index_buffers, ib_handle);

        glGenBuffers(1, &ib->ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, iData, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        Graphics::se_resource_index_buffer handle = {};

        handle.id = ib_handle;

        return handle;
    }

    bool init(Graphics::RenderDeviceDOD* renderDevice)
    {
        render_data_t* data = (render_data_t*)malloc(sizeof(render_data_t));

	    renderDevice->data = data;

        data->command_buffers = se_slot_array_new(command_buffer_t);
        data->shaders = se_slot_array_new(shader_t);
        data->textures = se_slot_array_new(texture_t);
        data->uniforms = se_slot_array_new(uniform_t);
        data->frame_buffers = se_slot_array_new(frame_buffer_t);
        data->vertex_buffers = se_slot_array_new(vertex_buffer_t);
        data->index_buffers = se_slot_array_new(index_buffer_t);
        data->vertex_layout_descriptions = se_slot_array_new(vertex_attribute_layout_description_t);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_CLAMP);
        glDepthFunc(GL_LESS);

        return true;
    }

    u32 get_texture_id(void* data, Graphics::se_resource_texture t_handle)
    {
        render_data_t* _data = (render_data_t*)data;
        texture_t _t = se_slot_array_get(_data->textures, t_handle.id);

        return _t.id;
    }

    Graphics::RenderDeviceDOD* render_device_construct()
    {
        Graphics::RenderDeviceDOD* r = (Graphics::RenderDeviceDOD*)malloc(sizeof(Graphics::RenderDeviceDOD));

        r->init = &init;

        r->reset_command_buffer =		&reset_command_buffer;
        r->set_view_clear =			    &set_view_clear;
        r->submit_command_buffer =		&submit_command_buffer;
        r->bind_shader =				&bind_shader;
        r->bind_frame_buffer =          &bind_frame_buffer;
        r->bind_frame_buffer_default =  &bind_frame_buffer_default;
        r->draw =				        &draw;
        r->draw_indexed =               &draw_indexed;
        r->update_vertex_data =		    &update_vertex_data;
        r->update_index_data =          &update_index_data;
        r->bind_vertex_buffer =		    &bind_vertex_buffer;
        r->bind_index_buffer =          &bind_index_buffer;
        r->bind_uniform =               &bind_uniform;
        r->bind_texture =               &bind_texture;

        r->construct_command_buffer =	&construct_command_buffer;
        r->construct_shader =           &construct_shader;
        r->construct_texture =          &construct_texture;
        r->construct_frame_buffer =     &construct_frame_buffer;
        r->construct_vertex_buffer =    &construct_vertex_buffer;
        r->construct_index_buffer =     &construct_index_buffer;
        r->construct_uniform =          &construct_uniform;

        r->get_texture_id = &get_texture_id;

        return r;
    }

}}}