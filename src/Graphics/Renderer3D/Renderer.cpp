#include "Graphics/Renderer3D/Renderer.h"

#include "Logger/Logger.h"

#include "Core/Core.h"

#include "Editor/ImGUI/EditorWindows/DeferredBuffersViewWindow.h"

#include "SDL_events.h"

namespace SteelEngine { namespace Graphics { namespace Renderer3D {

    void Renderer::InitOpenGL()
    {
        glewExperimental = GL_TRUE;

        GLenum err = glewInit();

        if(GLEW_OK != err)
        {
            SE_ERROR("Error while initializing glew: %s", glewGetErrorString(err));
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_CLAMP);
        glDepthFunc(GL_LESS);
    }

    void Renderer::Create(
        const std::vector<glm::vec3>& vertices,
        const std::vector<glm::vec2>& texCoords,
        const std::vector<glm::vec3>& normals,
        const std::vector<uint32_t>& indices,
        const Transform& trans
    )
    {
        MeshData mesh;

        glGenVertexArrays(1, &mesh.m_VAO);
        glBindVertexArray(mesh.m_VAO);

        glGenBuffers(MeshVBO::SIZE_VB, mesh.m_VBOs);

        mesh.m_SizeOfVBOs = MeshVBO::SIZE_VB;

    // Vertices
        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_VBOs[MeshVBO::POSITION_VB]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // TexCoords
        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_VBOs[MeshVBO::TEXCOORD_VB]);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Normals
        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_VBOs[MeshVBO::NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_VBOs[MeshVBO::INDEX_VB]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);

        mesh.m_DrawCount = indices.size();

        entt::registry& scene = m_Scene->GetCurrentScene();
        entt::entity e0 = scene.create();
        VAO_Component& comp0 = scene.emplace<VAO_Component>(e0);
        TextureComponent& comp1 = scene.emplace<TextureComponent>(e0);

        comp0.m_VAO = mesh.m_VAO;
        comp0.m_DrawCount = mesh.m_DrawCount;
        comp0.m_Transform = trans.GetModel();

        comp1.m_TextureID = m_SomeTexture->GetTextureID();
        comp1.m_TextureUnit = 0;
        comp1.m_TextureTarget = GL_TEXTURE_2D;
    }

    Renderer::Renderer()
    {
        m_RotateCamera = false;
        m_Controlls = false;
    }

    Renderer::~Renderer()
    {

    }

    Result Renderer::Init()
    {
        InitOpenGL();

        m_FinalTexture = new Texture(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, false);
        m_GBuffer = new Framebuffer(1920, 1080,
        {
            new Framebuffer::Attachment(m_FinalTexture, GL_COLOR_ATTACHMENT0)
        });

        m_GShader = new Shader();
        m_Camera = new Camera(Transform(), 1920 / 1080);

        m_GBuffer->Setup();
        m_GShader->Setup("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/Renderer3D/shader");

        m_Uniforms[Uniform::MODEL_U] = m_GShader->GetUniformLocation("model");
        m_Uniforms[Uniform::VIEW_U] = m_GShader->GetUniformLocation("view");
        m_Uniforms[Uniform::PROJECTION_U] = m_GShader->GetUniformLocation("projection");
        m_Uniforms[Uniform::EYE_POSITION] = m_GShader->GetUniformLocation("eyePosition");
        m_Uniforms[Uniform::MATERIAL_SHININESS] = m_GShader->GetUniformLocation("material.shininess");
        m_Uniforms[Uniform::DIRECTIONAL_LIGHT_DIRECTION] = m_GShader->GetUniformLocation("directionalLight.direction");
        m_Uniforms[Uniform::DIRECTIONAL_LIGHT_AMBIENT] = m_GShader->GetUniformLocation("directionalLight.ambient");
        m_Uniforms[Uniform::DIRECTIONAL_LIGHT_DIFFUSE] = m_GShader->GetUniformLocation("directionalLight.diffuse");
        m_Uniforms[Uniform::DIRECTIONAL_LIGHT_SPECULAR] = m_GShader->GetUniformLocation("directionalLight.specular");

        m_SomeTexture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2.jpg");

        m_SomeTexture->Setup();

        Model* model = (Model*)Model::Create("D:/Projects/C++/SteelEngine/bin/Resources/Models/a.obj");
        std::vector<glm::vec3> vertices = model->GetVertices();
        std::vector<glm::vec2> texCoords = model->GetTexCoords();
        std::vector<glm::vec3> normals = model->GetNormals();
        std::vector<uint32_t> indices = model->GetIndices();

        for(Type::uint32 i = 0; i < 100; i++)
        {
            for(Type::uint32 j = 0; j < 10; j++)
            {
                Transform trans;

                trans.SetPosition(glm::vec3(i * 2, 0, j * 2));

                Create(vertices, texCoords, normals, indices, trans);
            }
        }

        Event::GlobalEvent::Broadcast(Editor::ImGUI::DeferredBuffersViewWindow::AddFramebuffer{ m_FinalTexture->GetTextureID() });

        return SE_TRUE;
    }

    void Renderer::Update()
    {
        if(m_Controlls)
        {
            Transform& camTrans = m_Camera->GetTransform();

            if(m_Keys[SDL_SCANCODE_W])
            {
                camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetForward() * 0.1f);
            }

            if(m_Keys[SDL_SCANCODE_S])
            {
                camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetBackward() * 0.1f);
            }

            if(m_Keys[SDL_SCANCODE_A])
            {
                camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetRight() * 0.1f);
            }

            if(m_Keys[SDL_SCANCODE_D])
            {
                camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetLeft() * 0.1f);
            }
        }

        if(m_RotateCamera)
        {
            Event::GlobalEvent::Broadcast(ChangeMousePositionEvent{ 1920 / 2, 1080 / 2 });
        }
    }

    void Renderer::Cleanup()
    {

    }

    void Renderer::PreRender()
    {

    }

    void Renderer::Render(entt::registry& scene)
    {
        m_GBuffer->Bind();

        glClearColor(0.15f, 0.15f, 0.15f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_GShader->Bind();

        m_GShader->SetUniform(m_Uniforms[Uniform::VIEW_U], m_Camera->GetView());
        m_GShader->SetUniform(m_Uniforms[Uniform::PROJECTION_U], m_Camera->GetProjection());

        m_GShader->SetUniform(m_Uniforms[Uniform::EYE_POSITION], -m_Camera->GetTransform().GetPosition());

        m_GShader->SetUniform(m_Uniforms[Uniform::MATERIAL_SHININESS], 0.5f);

    // For every light we need to render whole scene...?
        m_GShader->SetUniform(m_Uniforms[Uniform::DIRECTIONAL_LIGHT_DIRECTION], glm::vec3(0, 1, 0));
        m_GShader->SetUniform(m_Uniforms[Uniform::DIRECTIONAL_LIGHT_AMBIENT], glm::vec3(0.01f, 0.01f, 0.01f));
        m_GShader->SetUniform(m_Uniforms[Uniform::DIRECTIONAL_LIGHT_DIFFUSE], glm::vec3(0.1f, 0.1f, 0.1f));
        m_GShader->SetUniform(m_Uniforms[Uniform::DIRECTIONAL_LIGHT_SPECULAR], glm::vec3(0.1f, 0.1f, 0.1f));

    // This should be shadow pass
        // scene.view<VAO_Component>().each([&](VAO_Component& vao)
        // {
        // // We should not use Transform::GetModel() here cuz it hit performance
        //     m_GShader->SetUniform(m_Uniforms[Uniform::MODEL_U], vao.m_Transform);

        //     glBindVertexArray(vao.m_VAO);
        //     glDrawElements(GL_TRIANGLES, vao.m_DrawCount, GL_UNSIGNED_INT, 0);
        // });

        scene.view<VAO_Component, TextureComponent>().each([&](VAO_Component& vao, TextureComponent& tex)
        {
            glActiveTexture(tex.m_TextureUnit);
            glBindTexture(tex.m_TextureTarget, tex.m_TextureID);

        // We should not use Transform::GetModel() here cuz it hit performance
            m_GShader->SetUniform(m_Uniforms[Uniform::MODEL_U], vao.m_Transform);

            glBindVertexArray(vao.m_VAO);
            glDrawElements(GL_TRIANGLES, vao.m_DrawCount, GL_UNSIGNED_INT, 0);
        });
    }

    void Renderer::PostRender()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void* Renderer::GetFinalTexture()
    {
        return (void*)m_FinalTexture->GetTextureID();
    }

    void Renderer::SetRenderTarget(IWindow* window)
    {
        m_Window = window;
    }

    void Renderer::OnEvent(Event::Naive* event)
    {
        static size_t event0 = KeyDownEvent::GetStaticType();
        static size_t event1 = KeyUpEvent::GetStaticType();
        static size_t event2 = AnyItemActiveChangedEvent::GetStaticType();
        static size_t event3 = MouseMotionEvent::GetStaticType();

        if(event0 == event->GetType())
        {
            KeyDownEvent* eve = (KeyDownEvent*)event;

            m_Keys[eve->m_KeyCode] = true;

            if(eve->m_KeyCode == SDL_SCANCODE_K)
            {
                m_RotateCamera = !m_RotateCamera;
                m_Controlls = true;
            }
        }
        else if(event1 == event->GetType())
        {
            KeyUpEvent* eve = (KeyUpEvent*)event;

            m_Keys[eve->m_KeyCode] = false;
        }
        else if(event2 == event->GetType())
        {
            AnyItemActiveChangedEvent* eve = (AnyItemActiveChangedEvent*)event;

            m_Controlls = eve->m_State;
        }
        else if(event3 == event->GetType())
        {
            MouseMotionEvent* eve = (MouseMotionEvent*)event;
            glm::vec2 deltaPos = glm::vec2(eve->m_PositionX, eve->m_PositionY) - glm::vec2(1920 / 2, 1080 / 2);

            if(m_RotateCamera && (deltaPos.x != 0 || deltaPos.y != 0))
            {
                Transform& camTrans = m_Camera->GetTransform();

                camTrans.SetRotation(glm::normalize(glm::angleAxis(deltaPos.x * -0.001f, glm::vec3(0, 1, 0)) * camTrans.GetRotation()));
                camTrans.SetRotation(glm::normalize(glm::angleAxis(deltaPos.y * 0.001f, camTrans.GetRotation().GetRight()) * camTrans.GetRotation()));
            }
        }
    }

}}}