#include "Graphics/OpenGL/Renderer.h"

#include "SDL_events.h"

#include "RuntimeReflection/Reflection.h"

#include "Graphics/ECS_Components/TransformComponent.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    entt::entity Renderer::AddModel(IMesh* mesh, entt::registry* scene, const Transform& transform)
    {
        Mesh* casted = (Mesh*)mesh;

        casted->Setup();

        auto model = scene->create();

        scene->assign<RenderableComponent>(model, RenderableComponent(casted->GetVAO(), casted->GetDrawCount(), m_G_Shader->GetShaderID()));
        scene->assign<TransformComponent>(model, TransformComponent(transform));

        return model;
    }

    Renderer::Renderer(IWindow* window) :
        m_Window(window)
    {
        m_QuadShader = new Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/quadShader");
        m_QuadMesh = new QuadMesh();

        m_Mesh = new Mesh("D:/Projects/C++/SteelEngine/bin/Resources/Models/test.obj");
        // m_Mesh2 = new Mesh("D:/Projects/C++/SteelEngine/bin/Resources/Models/cube.obj");
        // m_Mesh3 = new Mesh("D:/Projects/C++/SteelEngine/bin/Resources/Models/a.obj");
        m_Texture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2.jpg");
        m_NormalMapTexture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2_normal.jpg");
        m_DispMapTexture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2_disp.jpg");

        m_PositionTexture = new Texture(GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, false);
        m_NormalTexture = new Texture(GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, false);
        m_AlbedoTexture = new Texture(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, false);
        m_ShadowTexture = new Texture(GL_R32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, false);

        m_G_Shader = new Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/gBuffer");
        m_G_Buffer = new Framebuffer(1920, 1080,
        {
            new Framebuffer::Attachment(m_PositionTexture, GL_COLOR_ATTACHMENT0),
            new Framebuffer::Attachment(m_NormalTexture, GL_COLOR_ATTACHMENT1),
            new Framebuffer::Attachment(m_AlbedoTexture, GL_COLOR_ATTACHMENT2)
        });

        m_ShadowShader = new Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/shadowShader");
        m_ShadowCamera = new Camera(Transform(), 1920 / 1080);

        m_DirectionalLightTransform.SetRotation(
            glm::quat(glm::rotate(glm::radians(-45.f), glm::vec3(1, 0, 0))) *
            glm::quat(glm::rotate(glm::radians(45.f), glm::vec3(0, 1, 0)))
        );

        m_Counter = 0;
        m_Camera = new Camera(Transform(glm::vec3(0, 0, -10)), 1920 / 1080);

        m_SpotLight = new SpotLight(
            PointLight{ BaseLight{ glm::vec3(1, 0, 1), 100.f },
            Attenuation{ 0, 0, 1 }, glm::vec3(0, 0, 0) }, Quaternion(glm::rotate(glm::radians(90.f), glm::vec3(0, 1, 0))), glm::radians(91.f));

        m_SpotLight->GetPointLight().m_Position = glm::vec3(0, 1, 6);
        m_SpotRotation = glm::quat(glm::rotate(glm::radians(180.f), glm::vec3(0, 1, 0)));
        m_SpotLight->SetRotation(m_SpotRotation);

        m_Trans.SetScale(glm::vec3(100, 100, 100));
        m_Trans2.SetPosition(glm::vec3(0, 1, 0));
        m_Trans2.SetRotation(glm::quat(glm::rotate(glm::radians(45.f), glm::vec3(0, 1, 0))));
        m_Trans3.SetPosition(glm::vec3(1, 20, 0));

        m_DirectionalLight = new DirectionalLight(BaseLight{ glm::vec3(1, 1, 1), 0.1f }, m_DirectionalLightTransform.GetRotation());

        m_Lights.push_back(m_DirectionalLight);
        m_Lights.push_back(m_SpotLight);

        m_Models.push_back(Model(
            m_Trans, m_Mesh, glm::vec3(1, 1, 1)
        ));

        for(Type::uint32 i = 0; i < 50; i++)
        {
            m_Mesh2 = new Mesh("D:/Projects/C++/SteelEngine/bin/Resources/Models/cube.obj");

            m_Models.push_back(Model(
                Transform(glm::vec3(i * 2, 1, 0)), m_Mesh2, glm::vec3(1, 1, 1)
            ));
        }

        for(Type::uint32 i = 0; i < 100; i++)
        {
            m_Mesh3 = new Mesh("D:/Projects/C++/SteelEngine/bin/Resources/Models/a.obj");

            m_Models.push_back(Model(
                Transform(glm::vec3(0, i * 2, 0)), m_Mesh3, glm::vec3(1, 1, 1)
            ));
        }

        m_Controlls = false;
    }

    Renderer::~Renderer()
    {

    }

    Result Renderer::Init()
    {
        glewExperimental = GL_TRUE;

        GLenum err = glewInit();

        if(GLEW_OK != err)
        {
            printf("Error: %s\n", glewGetErrorString(err));

            return SE_FALSE;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_CLAMP);
        glDepthFunc(GL_LESS);

    // Models in scene -------------------
        m_Texture->Setup();

        for(Type::uint32 i = 0; i < m_Models.size(); i++)
        {
            m_Models[i].m_Mesh->Setup();
        }

        m_NormalMapTexture->Setup();
        m_DispMapTexture->Setup();
    //

    // GBuffer shader ---------------------------
        m_G_Shader->Init();
        m_G_Shader->Bind();
        m_G_Shader->SetInt("albedoMap", 0);
        m_G_Shader->SetInt("normalMap", 1);
        m_G_Shader->SetInt("dispMap", 2);
        m_G_Shader->SetInt("directionalLightShadowMap", 3);
        m_G_Shader->SetInt("spotLightShadowMap", 4);
    //

    // Quad shader ------------------------
        m_QuadShader->Init();
        m_QuadShader->Bind();
        m_QuadShader->SetInt("gPosition", 0);
        m_QuadShader->SetInt("gNormal", 1);
        m_QuadShader->SetInt("gAlbedoSpec", 2);
        m_QuadMesh->Setup();
    //

    // Shadow shader -------------------------------------
        m_ShadowShader->Init();
    //

    // GBuffer framebuffer ------------------------
        m_G_Buffer->Setup();
    //

        for(Light* light : m_Lights)
        {
            light->Setup();
        }

        return SE_TRUE;
    }

    void Renderer::Update()
    {
        m_Counter += 0.01f;

        Transform& camTrans = m_Camera->GetTransform();

        if(!m_Controlls)
        {
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
        Type::uint32 w;
        Type::uint32 h;

        m_Window->GetWindowSize(w, h);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0, 0, w, h);

        glClearColor(0.2f, 0.2f, 0.2f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Render(entt::registry* scene)
    {
    // Shadows
        m_ShadowShader->Bind();

        for(Light* light : m_Lights)
        {
            light->GetShadowFramebuffer()->Bind();
            glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_ShadowCamera->SetProjection(light->GetShadowInfo()->GetProjection());

            light->Update(m_ShadowCamera->GetTransform());
            light->GetShadowInfo()->m_LightMatrix = m_ShadowCamera->GetProjection() * m_ShadowCamera->GetView();

            for(Type::uint32 i = 0; i < m_Models.size(); i++)
            {
                Model model = m_Models[i];

                m_ShadowShader->Update(model.m_Transform, *m_ShadowCamera, 0, 0);
                model.m_Mesh->Draw();
            }

            scene->view<RenderableComponent, TransformComponent>().each([&](RenderableComponent& model, TransformComponent& trans)
            {
                m_ShadowShader->Update(trans.m_Transform, *m_ShadowCamera, 0, 0);

                glBindVertexArray(model.m_VAO);
                glDrawElements(GL_TRIANGLES, model.m_DrawCount, GL_UNSIGNED_INT, 0);
            });
        }

    // Objects data
        m_G_Buffer->Bind();
        glClearColor(0.2f, 0.2f, 0.2f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_G_Shader->Bind();
        m_Texture->Bind(GL_TEXTURE0);
        m_NormalMapTexture->Bind(GL_TEXTURE1);
        m_DispMapTexture->Bind(GL_TEXTURE2);
        m_DirectionalLight->GetShadowMapTexture()->Bind(GL_TEXTURE3);
        m_SpotLight->GetShadowMapTexture()->Bind(GL_TEXTURE4);

        m_G_Shader->SetVec3("ambientLight", glm::vec3(0.1f, 0.1f, 0.1f));
        m_G_Shader->SetVec3("baseColor", glm::vec3(1, 1, 1));

        m_G_Shader->SetVec3("eyePosition", m_Camera->GetTransform().GetPosition());

        m_G_Shader->SetFloat("specularIntensity", 2.f);
        m_G_Shader->SetFloat("specularPower", 32.f);

        float dispMapScale = 0.05f;
        float dispMapOffset = -1.f;
        float dispMapBias = dispMapScale / 2.0f;

        m_G_Shader->SetFloat("dispMapScale", dispMapScale);
        m_G_Shader->SetFloat("dispMapOffset", -dispMapBias + dispMapBias * dispMapOffset);

    // Point light ---------------------------------------
        m_G_Shader->SetVec3("pointLight.base.color", glm::vec3(1, 1, 1));
        m_G_Shader->SetFloat("pointLight.base.intensity", 100.f);
        m_G_Shader->SetFloat("pointLight.attenuation.constant", 0.f);
        m_G_Shader->SetFloat("pointLight.attenuation.linear", 0.f);
        m_G_Shader->SetFloat("pointLight.attenuation.exponent", 1.f);
        m_G_Shader->SetVec3("pointLight.position", glm::vec3(0, 10, 0));
    //

        for(Light* light : m_Lights)
        {
            light->Update(*m_G_Shader);
        }

        for(Type::uint32 i = 0; i < m_Models.size(); i++)
        {
            Model model = m_Models[i];

            m_G_Shader->SetVec3("baseColor", model.m_Color);
            m_G_Shader->Update(model.m_Transform, *m_Camera, m_SpotLight->GetShadowInfo(), m_DirectionalLight->GetShadowInfo());
            model.m_Mesh->Draw();
        }

        scene->view<RenderableComponent, TransformComponent>().each([&](RenderableComponent& model, TransformComponent& trans)
        {
            m_G_Shader->SetVec3("baseColor", glm::vec3(1, 1, 1));
            m_G_Shader->Update(trans.m_Transform, *m_Camera, m_SpotLight->GetShadowInfo(), m_DirectionalLight->GetShadowInfo());

            glBindVertexArray(model.m_VAO);
            glDrawElements(GL_TRIANGLES, model.m_DrawCount, GL_UNSIGNED_INT, 0);
        });

    // Render quad
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_QuadShader->Bind();

        m_PositionTexture->Bind(GL_TEXTURE0);
        m_NormalTexture->Bind(GL_TEXTURE1);
        m_AlbedoTexture->Bind(GL_TEXTURE2);

        m_QuadMesh->Draw();
    }

    void Renderer::PostRender()
    {
        m_Window->SwapBuffers();
    }

    void Renderer::OnEvent(Event::Naive* event)
    {
        static size_t event1 = MouseMotionEvent::GetStaticType();
        static size_t event2 = KeyDownEvent::GetStaticType();
        static size_t event3 = KeyUpEvent::GetStaticType();
        static size_t event4 = AnyItemActiveChangedEvent::GetStaticType();

        if(event1 == event->GetType())
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
        else if(event2 == event->GetType())
        {
            KeyDownEvent* eve = (KeyDownEvent*)event;

            m_Keys[eve->m_KeyCode] = true;

            if(eve->m_KeyCode == SDL_SCANCODE_K)
            {
                m_RotateCamera = !m_RotateCamera;
            }
        }
        else if(event3 == event->GetType())
        {
            KeyUpEvent* eve = (KeyUpEvent*)event;

            m_Keys[eve->m_KeyCode] = false;
        }
        else if(event4 == event->GetType())
        {
            AnyItemActiveChangedEvent* eve = (AnyItemActiveChangedEvent*)event;

            m_Controlls = eve->m_State;
        }
    }

}}}