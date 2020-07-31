#include "Graphics/TestDOD/Renderer.h"

#include "Utils/Graphics/RenderContext.h"

#include "stbi/stb_image.h"

#include "fstream"
#include "string"

namespace SteelEngine { namespace Graphics { namespace TestDOD {

    std::string loadShader(const char* filename)
    {
        std::ifstream file(filename);

        if(!file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }

        std::string lines;

        for(std::string line; std::getline(file, line);)
        {
            line.append("\n");
            lines.insert(lines.end(), line.begin(), line.end());
        }

        file.close();

        return lines;
    }

    Renderer::Renderer()
    {

    }

    Renderer::~Renderer()
    {

    }

    Result Renderer::Init()
    {
        m_GFX = m_RenderContext->GetRenderDeviceDOD();

        m_GFX->init(m_GFX);

        m_CommandBuffer = m_GFX->construct_command_buffer(m_GFX->data);

        std::string v_src = loadShader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/TestDOD/test.vs");
        std::string f_src = loadShader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/TestDOD/test.fs");

        shader = m_GFX->construct_shader(m_GFX->data, v_src.c_str(), f_src.c_str());
        color = m_GFX->construct_uniform(m_GFX->data, shader, "color", Platform::Graphics::uniform_type_vec3);
        brick = m_GFX->construct_uniform(m_GFX->data, shader, "brick", Platform::Graphics::uniform_type_sampler2d);

        Platform::Graphics::texture_construction_info texInfo = {};

        texInfo.format =        Platform::Graphics::texture_format::se_texture_format_rgba8;
        texInfo.mag_filter =    Platform::Graphics::texture_filtering::nearest;
        texInfo.min_filter =    Platform::Graphics::texture_filtering::nearest;
        texInfo.wrap_s =        Platform::Graphics::texture_wrapping::repeat;
        texInfo.wrap_t =        Platform::Graphics::texture_wrapping::repeat;

        texInfo.data = stbi_load("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2.jpg", (int*)&texInfo.width, (int*)&texInfo.height, (int*)&texInfo.num_components, 4);

        texture = m_GFX->construct_texture(m_GFX->data, texInfo);

        Platform::Graphics::texture_construction_info resultTextureInfo = {};

        resultTextureInfo.width =           1920;
        resultTextureInfo.height =          1080;
        resultTextureInfo.format =          Platform::Graphics::texture_format::se_texture_format_rgba8;
        resultTextureInfo.mag_filter =      Platform::Graphics::texture_filtering::nearest;
        resultTextureInfo.min_filter =      Platform::Graphics::texture_filtering::nearest;
        resultTextureInfo.wrap_s =          Platform::Graphics::texture_wrapping::repeat;
        resultTextureInfo.wrap_t =          Platform::Graphics::texture_wrapping::repeat;

        resultTexture = m_GFX->construct_texture(m_GFX->data, resultTextureInfo);
        result = m_GFX->construct_frame_buffer(m_GFX->data, resultTexture);

        Platform::Graphics::vertex_attribute_type layout[] =
        {
            Platform::Graphics::vertex_attribute_float2,
            Platform::Graphics::vertex_attribute_float2,
        };

        u32 layout_count = sizeof(layout) / sizeof(Platform::Graphics::vertex_attribute_type); 

        f32 v_data[] = 
        {
            0.0f, 0.5f, 0, 0,
            -0.5f, -0.5f, 0, 0.5,
            0.5f, -0.5f, 1, 1
        };

        vb = m_GFX->construct_vertex_buffer(m_GFX->data, layout, layout_count, v_data, sizeof(v_data));

        stbi_image_free(texInfo.data);

        return SE_TRUE;
    }

    void Renderer::Update()
    {

    }

    void Renderer::Cleanup()
    {

    }

    void Renderer::PreRender()
    {

    }

    void Renderer::Render(entt::registry& scene)
    {
        glm::vec3 color_ = { 1, 0, 0 };

        m_GFX->bind_frame_buffer(m_GFX->data, m_CommandBuffer, result);
        m_GFX->set_view_clear(m_GFX->data, m_CommandBuffer, glm::vec4(0, 0, 0, 1));
        m_GFX->bind_shader(m_GFX->data, m_CommandBuffer, shader);
        m_GFX->bind_uniform(m_GFX->data, m_CommandBuffer, color, &color_);
        m_GFX->bind_texture(m_GFX->data, m_CommandBuffer, texture, brick, 0);
        m_GFX->bind_vertex_buffer(m_GFX->data, m_CommandBuffer, vb);
        m_GFX->draw(m_GFX->data, m_CommandBuffer, 0, 3);

        // m_GFX->set_depth_enabled(m_GFX->data, m_CommandBuffer, 1);
        // m_GFX->bind_shader(m_GFX->data, m_CommandBuffer, 2);

        m_GFX->bind_frame_buffer_default(m_GFX->data, m_CommandBuffer);

        m_GFX->submit_command_buffer(m_GFX->data, m_CommandBuffer);
    }

    void Renderer::PostRender()
    {

    }

    void* Renderer::GetFinalTexture()
    {
        return (void*)m_GFX->get_texture_id(m_GFX->data, resultTexture);
    }

    void Renderer::SetRenderTarget(IWindow* window)
    {
        m_Window = window;
    }

    void Renderer::SetSceneManager(ISceneManager* scene)
    {
        m_Scene = scene;
    }

    void Renderer::SetRenderContext(SteelEngine::Utils::RenderContext* context)
    {
        m_RenderContext = context;
    }

    void Renderer::OnEvent(Event::Naive* event)
    {

    }

}}}