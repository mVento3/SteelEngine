#include "Graphics/Renderer2D/Renderer.h"

#include "Logger/Logger.h"

#include "Core/Core.h"

#include "SDL.h"

#include "Math/Transform.h"

namespace SteelEngine { namespace Graphics { namespace Renderer2D {

    const Type::uint32 Renderer::MAX_QUAD_COUNT = 500;
    const Type::uint32 Renderer::MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
    const Type::uint32 Renderer::MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;

    static Vertex* createQuad(Vertex* target, float x, float y, float texID)
    {
        float size = 5.f;

        target->m_Position = glm::vec4(glm::vec3(x, y, 0.f), 1.f);
        target->m_Color = { 1.f, 1.f, 1.f, 1.f };
        target->m_TexCoord = { 0.f, 0.f };
        target->m_TexID = texID;
        target++;

        target->m_Position = glm::vec4(glm::vec3(x + size, y, 0.f), 1.f);
        target->m_Color = { 1.f, 1.f, 1.f, 1.f };
        target->m_TexCoord = { 1.f, 0.f };
        target->m_TexID = texID;
        target++;

        target->m_Position = glm::vec4(glm::vec3(x + size, y + size, 0.f), 1.f);
        target->m_Color = { 1.f, 1.f, 1.f, 1.f };
        target->m_TexCoord = { 1.f, 1.f };
        target->m_TexID = texID;
        target++;

        target->m_Position = glm::vec4(glm::vec3(x, y + size, 0.f), 1.f);
        target->m_Color = { 1.f, 1.f, 1.f, 1.f };
        target->m_TexCoord = { 0.f, 1.f };
        target->m_TexID = texID;
        target++;

        return target;
    }

    entt::entity Renderer::AddQuad(float x, float y, int texID)
    {
        entt::registry& scene = m_Scene->GetCurrentScene();
        entt::entity quad = scene.create();
        QuadComponent& quadC = scene.assign<QuadComponent>(quad);
        Vertex* vert = (Vertex*)quadC.m_Vertices;

        vert = createQuad(vert, x, y, texID);

        return quad;
    }

    void Renderer::Flush(Type::uint32 indexCount, Type::uint32 verticesSize)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, m_Vertices);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }

    Renderer::Renderer()
    {
        m_CameraPosition = glm::vec2(0, 0);
        m_Projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, -1.f, 1.f);
        m_Scale = 1.f;
        m_Width = 100;
        m_Height = 100;
        m_Vertices = new Vertex[MAX_VERTEX_COUNT];
        m_Buffer = m_Vertices;
        m_DrawCalls = 0;
        m_DrawIndexCount = 0;
    }

    Renderer::~Renderer()
    {

    }

    Result Renderer::Init()
    {
        m_DeltaTimeVariant = Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::DELTA_TIME);

        glewExperimental = GL_TRUE;

        GLenum err = glewInit();

        if(GLEW_OK != err)
        {
            SE_ERROR("Error while initializing glew: %s", glewGetErrorString(err));

            return SE_FALSE;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_CLAMP);
        glDepthFunc(GL_LESS);

        m_Shader = new Shader();
        m_FinalTexture = new Texture(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, false);
        m_G_Framebuffer = new Framebuffer(1920, 1080,
        {
            new Framebuffer::Attachment(m_FinalTexture, GL_COLOR_ATTACHMENT0)
        });

        m_G_Framebuffer->Setup();
        m_Shader->Setup("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/Renderer2D/shader");

        m_Uniforms[ShaderUniformLocation::VIEW] = m_Shader->GetUniformLocation("view");
        m_Uniforms[ShaderUniformLocation::PROJECTION] = m_Shader->GetUniformLocation("projection");

        m_Shader->Bind();

        auto loc = m_Shader->GetUniformLocation("textures");
        int samplers[] = { 0, 1 };

        glUniform1iv(loc, 2, samplers);

        glCreateVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glCreateBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Vertex), 0, GL_DYNAMIC_DRAW);

        glEnableVertexArrayAttrib(m_VBO, 0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, m_Position));

        glEnableVertexArrayAttrib(m_VBO, 1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, m_Color));

        glEnableVertexArrayAttrib(m_VBO, 2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, m_TexCoord));

        glEnableVertexArrayAttrib(m_VBO, 3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, m_TexID));

        Type::uint32 indices[MAX_INDEX_COUNT];
        Type::uint32 offset = 0;

        for(Type::uint32 i = 0; i < MAX_INDEX_COUNT; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;

            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;

            offset += 4;
        }

        glCreateBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        m_TestTexture1 = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2.jpg");
        m_TestTexture2 = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2_disp.jpg");

        m_TestTexture1->Setup();
        m_TestTexture2->Setup();

        Event::GlobalEvent::Add<ViewportSizeChangedEvent>(this);

        for(Type::uint32 i = 0; i < 500; i++)
        {
            for(Type::uint32 j = 0; j < 500; j++)
            {
                AddQuad(i * 6, j * 6, 0);
            }
        }

        // test = AddQuad(1000, 500, 1);

        return SE_TRUE;
    }

    void Renderer::Update()
    {
        IDeltaTime* dt = m_DeltaTimeVariant->Convert<IDeltaTime*>();
        static float counter = 0;
        float speed = 1.f;

        counter += dt->GetDeltaTime();

        if(m_Keys[SDL_SCANCODE_LSHIFT])
        {
            speed = 5.f;
        }

        if(m_Keys[SDL_SCANCODE_W])
        {
            m_CameraPosition -= glm::vec2(0.f, 200.f) * dt->GetDeltaTime() * speed;
        }

        if(m_Keys[SDL_SCANCODE_S])
        {
            m_CameraPosition += glm::vec2(0.f, 200.f) * dt->GetDeltaTime() * speed;
        }

        if(m_Keys[SDL_SCANCODE_A])
        {
            m_CameraPosition += glm::vec2(200.f, 0.f) * dt->GetDeltaTime() * speed;
        }

        if(m_Keys[SDL_SCANCODE_D])
        {
            m_CameraPosition -= glm::vec2(200.f, 0.f) * dt->GetDeltaTime() * speed;
        }

        if(counter >= 1.f)
        {
            counter = 0;

            printf("Draw calls: %u\n", m_DrawCalls);
        }

        // entt::registry& scene = m_Scene->GetCurrentScene();
        // QuadComponent& quad = scene.get<QuadComponent>(test);

        // quad.m_Vertices[0].m_Position.x += counter;
        // quad.m_Vertices[1].m_Position.x += counter;
        // quad.m_Vertices[2].m_Position.x += counter;
        // quad.m_Vertices[3].m_Position.x += counter;

        // scene.view<QuadComponent>().each([&](QuadComponent& quad)
        // {
        //     quad.m_Vertices[0].m_Position.y -= counter;
        //     quad.m_Vertices[1].m_Position.y -= counter;
        //     quad.m_Vertices[2].m_Position.y -= counter;
        //     quad.m_Vertices[3].m_Position.y -= counter;
        // });
    }

    void Renderer::Cleanup()
    {

    }

    void Renderer::PreRender()
    {
        m_DrawCalls = 0;
    }

    void Renderer::Render(entt::registry& scene)
    {
        const size_t verticesSize = MAX_VERTEX_COUNT * sizeof(Vertex);

        m_G_Framebuffer->Bind();

        glClearColor(0.15f, 0.15f, 0.15f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_Shader->Bind();

        glBindTextureUnit(0, m_TestTexture1->GetTextureID());
        glBindTextureUnit(1, m_TestTexture2->GetTextureID());

        m_Shader->SetUniform(m_Uniforms[ShaderUniformLocation::PROJECTION], m_Projection);
        m_Shader->SetUniform(m_Uniforms[ShaderUniformLocation::VIEW], glm::translate(glm::vec3(m_CameraPosition, 0.f)));

        scene.view<QuadComponent>().each([&](QuadComponent& quad)
        {
            if(m_DrawIndexCount >= MAX_INDEX_COUNT)
            {
                Flush(m_DrawIndexCount, verticesSize);

                m_DrawIndexCount = 0;
                m_Buffer = m_Vertices;
            }

            *m_Buffer = quad.m_Vertices[0];
            m_Buffer++;
            *m_Buffer = quad.m_Vertices[1];
            m_Buffer++;
            *m_Buffer = quad.m_Vertices[2];
            m_Buffer++;
            *m_Buffer = quad.m_Vertices[3];
            m_Buffer++;

            m_DrawIndexCount += 6;
        });

        Flush(m_DrawIndexCount, verticesSize);

        // for(Type::uint32 i = 0; i < 87; i++)
        // {
        //     for(Type::uint32 j = 0; j < 4; j++)
        //     {
        //         m_Shader->SetUniform(m_Shader->GetUniformLocation("model"), glm::translate(glm::vec3(600.f * i, 1000.f * j, 0.f)));
        //         glBindVertexArray(m_VAO);
        //         glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        //     }
        // }
    }

    void Renderer::PostRender()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void* Renderer::GetFinalTexture()
    {
        return (void*)m_FinalTexture->GetTextureID();
    }

    void Renderer::OnEvent(Event::Naive* event)
    {
        static size_t event0 = KeyDownEvent::GetStaticType();
        static size_t event1 = KeyUpEvent::GetStaticType();
        static size_t event2 = MouseWheelEvent::GetStaticType();

        if(event0 == event->GetType())
        {
            KeyDownEvent* eve = (KeyDownEvent*)event;

            m_Keys[eve->m_KeyCode] = true;
        }
        else if(event1 == event->GetType())
        {
            KeyUpEvent* eve = (KeyUpEvent*)event;

            m_Keys[eve->m_KeyCode] = false;
        }
        else if(event2 == event->GetType())
        {
            MouseWheelEvent* eve = (MouseWheelEvent*)event;

            m_Scale -= eve->m_Y * 0.01f;

            if(m_Keys[SDL_SCANCODE_LSHIFT])
            {
                m_Scale -= eve->m_Y * 0.1f;
            }
            else
            {
                m_Scale -= eve->m_Y * 0.01f;
            }

            if(m_Scale <= 0.f)
            {
                m_Scale = 0.01f;
            }

            m_Projection = glm::ortho(m_Width * -1.f * m_Scale, m_Width * 1.f * m_Scale, m_Height * -1.f * m_Scale, m_Height * 1.f * m_Scale, -1.f, 1.f);
        }
    }

    void Renderer::operator()(const ViewportSizeChangedEvent& event)
    {
        m_Width = event.m_X;
        m_Height = event.m_Y;
        m_Projection = glm::ortho(m_Width * -1.f * m_Scale, m_Width * 1.f * m_Scale, m_Height * -1.f * m_Scale, m_Height * 1.f * m_Scale, -1.f, 1.f);
    }

}}}