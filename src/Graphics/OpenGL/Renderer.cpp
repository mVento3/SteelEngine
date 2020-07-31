#include "Graphics/OpenGL/Renderer.h"

#include "SDL_events.h"

#include "RuntimeReflection/Reflection.h"

#include "Profiler/ScopeTimer.h"

#include "Logger/Logger.h"

#include "Core/Core.h"

#include "Editor/ImGUI/EditorWindows/DeferredBuffersViewWindow.h"

namespace SteelEngine { namespace Graphics { namespace OpenGL {

    entt::entity Renderer::AddModel(IMesh* mesh, ISceneManager* scene, const Transform& transform, bool castShadow)
    {
        Mesh* casted = (Mesh*)mesh;
        entt::registry& reg = scene->GetCurrentScene();

        auto model = reg.create();

        reg.assign<RenderableComponent>(model, RenderableComponent(casted, m_G_Shader, transform, castShadow));
        ShaderComponent& shader = reg.assign<ShaderComponent>(model);
        // DirectionalLightComponent& dirLightComp = reg.assign<DirectionalLightComponent>(model, DirectionalLightComponent());
        CameraComponent& camera = reg.assign<CameraComponent>(model, CameraComponent(1920 / 1080));

        // dirLightComp.m_FBO = m_DirectionalLight->GetShadowFramebuffer()->GetFBO();
        // dirLightComp.m_TextureID = m_DirectionalLight->GetShadowMapTexture()->GetTextureID();
        // dirLightComp.m_Width = m_DirectionalLight->GetShadowMapTexture()->GetTextureWidth();
        // dirLightComp.m_Height = m_DirectionalLight->GetShadowMapTexture()->GetTextureHeight();

        // dirLightComp.m_Uniforms[BaseLight::BASE_COLOR] = m_G_Shader->GetUniformLocation("directionalLight.base.color");
        // dirLightComp.m_Uniforms[BaseLight::INTENSITY] = m_G_Shader->GetUniformLocation("directionalLight.base.intensity");
        // dirLightComp.m_Uniforms[DirectionalLightComponent::ROTATION] = m_G_Shader->GetUniformLocation("directionalLight.direction");

        // dirLightComp.m_Base = BaseLight{ glm::vec3(1, 1, 1), 0.1f };

        // dirLightComp.m_Transform = m_DirectionalLightTransform;

        shader.m_ShaderID = m_G_Shader->GetShaderID();
        shader.m_Uniforms[ShaderComponent::MODEL_U] = m_G_Shader->GetUniformLocation("model");
        shader.m_Uniforms[ShaderComponent::PROJECTION_U] = m_G_Shader->GetUniformLocation("projection");
        shader.m_Uniforms[ShaderComponent::VIEW_U] = m_G_Shader->GetUniformLocation("view");
        shader.m_Uniforms[ShaderComponent::LIGHT_MATRIX_U] = m_G_Shader->GetUniformLocation("lightMatrix");
        shader.m_Uniforms[ShaderComponent::LIGHT_MATRIX2_U] = m_G_Shader->GetUniformLocation("lightMatrix2");
        shader.m_Uniforms[ShaderComponent::AMBIENT_LIGHT] = m_G_Shader->GetUniformLocation("ambientLight");
        shader.m_Uniforms[ShaderComponent::BASE_COLOR] = m_G_Shader->GetUniformLocation("baseColor");
        shader.m_Uniforms[ShaderComponent::EYE_POSITION] = m_G_Shader->GetUniformLocation("eyePosition");
        shader.m_Uniforms[ShaderComponent::SPECULAR_INTENSITY] = m_G_Shader->GetUniformLocation("specularIntensity");
        shader.m_Uniforms[ShaderComponent::SPECULAR_POWER] = m_G_Shader->GetUniformLocation("specularPower");
        shader.m_Uniforms[ShaderComponent::DISP_MAP_SCALE] = m_G_Shader->GetUniformLocation("dispMapScale");
        shader.m_Uniforms[ShaderComponent::DISP_MAP_OFFSET] = m_G_Shader->GetUniformLocation("dispMapOffset");

        shader.m_BaseColor = m_G_Shader->m_BaseColor;
        shader.m_AmbientLight = m_G_Shader->m_AmbientLight;
        shader.m_SpecularIntensity = m_G_Shader->m_SpecularIntensity;
        shader.m_SpecularPower = m_G_Shader->m_SpecularPower;
        shader.m_DispMapScale = m_G_Shader->m_DispMapScale;
        shader.m_DispMapOffset = m_G_Shader->m_DispMapOffset;

        return model;
    }

    Renderer::Renderer()
    {
        m_RotateCamera = false;

        m_QuadShader = new Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/quadShader");
        m_QuadMesh = new QuadMesh();

        m_Texture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2.jpg");
        m_NormalMapTexture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2_normal.jpg");
        m_DispMapTexture = new Texture("D:/Projects/C++/SteelEngine/bin/Resources/Textures/bricks2_disp.jpg");

        m_PositionTexture = new Texture(GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, false);
        m_NormalTexture = new Texture(GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, false);
        m_AlbedoTexture = new Texture(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, false);
        m_ShadowTexture = new Texture(GL_R32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, false);

        m_G_Shader = new TestShader();
        m_G_Buffer = new Framebuffer(1920, 1080,
        {
            new Framebuffer::Attachment(m_PositionTexture, GL_COLOR_ATTACHMENT0),
            new Framebuffer::Attachment(m_NormalTexture, GL_COLOR_ATTACHMENT1),
            new Framebuffer::Attachment(m_AlbedoTexture, GL_COLOR_ATTACHMENT2)
        });

        float dispMapScale = 0.05f;
        float dispMapOffset = -1.f;
        float dispMapBias = dispMapScale / 2.0f;

        m_G_Shader->m_BaseColor = glm::vec3(1, 1, 1);
        m_G_Shader->m_AmbientLight = glm::vec3(0.1f, 0.1f, 0.1f);
        m_G_Shader->m_SpecularIntensity = 2.f;
        m_G_Shader->m_SpecularPower = 32.f;
        m_G_Shader->m_DispMapScale = dispMapScale;
        m_G_Shader->m_DispMapOffset = -dispMapBias + dispMapBias * dispMapOffset;

        m_ShadowShader = new Shader("D:/Projects/C++/SteelEngine/bin/Resources/Shaders/OpenGL/shadowShader");
        m_ShadowCamera = new Camera(Transform(), 1920 / 1080);

        m_DirectionalLightTransform.SetRotation(
            glm::quat(glm::rotate(glm::radians(-45.f), glm::vec3(1, 0, 0))) *
            glm::quat(glm::rotate(glm::radians(45.f), glm::vec3(0, 1, 0)))
        );

        m_Camera = new Camera(Transform(glm::vec3(0, 0, -10)), 1920 / 1080);

        m_SpotLight = new SpotLight(
            PointLight{ BaseLight{ glm::vec3(1, 1, 1), 100.f },
            Attenuation{ 0, 0, 1 }, glm::vec3(0, 0, 0) }, Quaternion(glm::rotate(glm::radians(90.f), glm::vec3(0, 1, 0))), glm::radians(70.f));

        m_SpotLight->GetPointLight().m_Position = glm::vec3(0, 1, 6);
        m_SpotRotation = glm::quat(glm::rotate(glm::radians(180.f), glm::vec3(0, 1, 0)));
        m_SpotLight->SetRotation(m_SpotRotation);

        m_DirectionalLight = new DirectionalLight(BaseLight{ glm::vec3(1, 1, 1), 0.1f }, m_DirectionalLightTransform.GetRotation());

        m_Lights.push_back(m_DirectionalLight);
        m_Lights.push_back(m_SpotLight);

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
            SE_ERROR("Error while initializing glew: %s", glewGetErrorString(err));

            return SE_FALSE;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_CLAMP);
        glDepthFunc(GL_LESS);

    // Models in scene -------------------
        m_Texture->Setup();

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
            light->Setup(*m_G_Shader);
        }

        entt::registry& scene = m_Scene->GetCurrentScene();
        entt::entity light = scene.create();

        DirectionalLightComponent& dirLightComp = scene.assign<DirectionalLightComponent>(light, DirectionalLightComponent());
        ShadowShaderComponent& shader = scene.assign<ShadowShaderComponent>(light);
        ShadowCameraComponent& camera = scene.assign<ShadowCameraComponent>(light);

        camera.m_Far = m_ShadowCamera->m_Far;
        camera.m_Near = m_ShadowCamera->m_Near;
        camera.m_Fov = m_ShadowCamera->m_Fov;
        camera.m_Transform = m_ShadowCamera->GetTransform();
        camera.m_Projection = m_ShadowCamera->GetProjection();

        shader.m_ShaderID = m_ShadowShader->GetShaderID();
        shader.m_Uniforms[ShadowShaderComponent::MODEL_U] = m_ShadowShader->GetUniformLocation("model");
        shader.m_Uniforms[ShadowShaderComponent::PROJECTION_U] = m_ShadowShader->GetUniformLocation("projection");
        shader.m_Uniforms[ShadowShaderComponent::VIEW_U] = m_ShadowShader->GetUniformLocation("view");

        dirLightComp.m_FBO = m_DirectionalLight->GetShadowFramebuffer()->GetFBO();
        dirLightComp.m_TextureID = m_DirectionalLight->GetShadowMapTexture()->GetTextureID();
        dirLightComp.m_Width = m_DirectionalLight->GetShadowMapTexture()->GetTextureWidth();
        dirLightComp.m_Height = m_DirectionalLight->GetShadowMapTexture()->GetTextureHeight();

        dirLightComp.m_Uniforms[BaseLight::BASE_COLOR] = m_G_Shader->GetUniformLocation("directionalLight.base.color");
        dirLightComp.m_Uniforms[BaseLight::INTENSITY] = m_G_Shader->GetUniformLocation("directionalLight.base.intensity");
        dirLightComp.m_Uniforms[DirectionalLightComponent::ROTATION] = m_G_Shader->GetUniformLocation("directionalLight.direction");

        dirLightComp.m_Base = BaseLight{ glm::vec3(1, 1, 1), 0.1f };

        dirLightComp.m_Transform = m_DirectionalLightTransform;

        Event::GlobalEvent::Add<ViewportSizeChangedEvent>(this);

        Event::GlobalEvent::Broadcast(Editor::ImGUI::DeferredBuffersViewWindow::AddFramebuffer{ m_PositionTexture->GetTextureID() });
        Event::GlobalEvent::Broadcast(Editor::ImGUI::DeferredBuffersViewWindow::AddFramebuffer{ m_NormalTexture->GetTextureID() });
        Event::GlobalEvent::Broadcast(Editor::ImGUI::DeferredBuffersViewWindow::AddFramebuffer{ m_AlbedoTexture->GetTextureID() });

        return SE_TRUE;
    }

    void Renderer::Update()
    {
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

        // m_Scene->GetCurrentScene().view<CameraComponent>().each([&](CameraComponent& camera)
        // {
        //     Transform& camTrans = camera.m_Transform;

        //     if(!m_Controlls)
        //     {
        //         if(m_Keys[SDL_SCANCODE_W])
        //         {
        //             camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetForward() * 0.1f);
        //         }

        //         if(m_Keys[SDL_SCANCODE_S])
        //         {
        //             camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetBackward() * 0.1f);
        //         }

        //         if(m_Keys[SDL_SCANCODE_A])
        //         {
        //             camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetRight() * 0.1f);
        //         }

        //         if(m_Keys[SDL_SCANCODE_D])
        //         {
        //             camTrans.SetPosition(camTrans.GetPosition() + camTrans.GetRotation().GetLeft() * 0.1f);
        //         }
        //     }
        // });

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
    // Not sure if that is necessary right now when we have custom viewport
        // Type::uint32 w;
        // Type::uint32 h;

        // m_Window->GetWindowSize(w, h);

        // // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        // glViewport(0, 0, w, h);

        // glClearColor(0.2f, 0.2f, 0.2f, 1);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Render(entt::registry& scene)
    {
    // Shadows
        {
            m_ShadowShader->Bind();

            scene.view<
                DirectionalLightComponent,
                ShadowShaderComponent
            >().each([&](
                DirectionalLightComponent& light,
                ShadowShaderComponent& shadowShader
            )
            {
                glViewport(0, 0, light.m_Width, light.m_Height);
                glBindFramebuffer(GL_FRAMEBUFFER, light.m_FBO);

                glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                Camera& camera = *m_ShadowCamera;
                Transform& camTrans = camera.GetTransform();

                camera.SetProjection(light.m_Projection);
                camTrans.SetPosition(glm::vec3(0, 0, 0));
                camTrans.SetRotation(light.m_Transform.GetRotation() * glm::quat(glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0))));

                const glm::mat4& view = glm::lookAt(
                    camTrans.GetPosition(),
                    camTrans.GetPosition() + camTrans.GetRotation().GetForward(),
                    glm::normalize(camTrans.GetRotation().GetUp())
                );

                light.m_LightMatrix = camera.GetProjection() * view;

                const glm::mat4& projection = camera.GetProjection();

                scene.view<RenderableComponent>().each([&](RenderableComponent& model)
                {
                    if(model.m_CastShadow)
                    {
                        const glm::mat4& modelM = model.m_Transform.GetModel();

                        glUniformMatrix4fv(shadowShader.m_Uniforms[ShaderComponent::MODEL_U], 1, GL_FALSE, &modelM[0][0]);
                        glUniformMatrix4fv(shadowShader.m_Uniforms[ShaderComponent::PROJECTION_U], 1, GL_FALSE, &projection[0][0]);
                        glUniformMatrix4fv(shadowShader.m_Uniforms[ShaderComponent::VIEW_U], 1, GL_FALSE, &view[0][0]);

                        glBindVertexArray(model.m_VAO);
                        glDrawElements(GL_TRIANGLES, model.m_DrawCount, GL_UNSIGNED_INT, 0);
                    }
                });
            });
        }

        {
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

            float dispMapScale = 0.05f;
            float dispMapOffset = -1.f;
            float dispMapBias = dispMapScale / 2.0f;

            m_G_Shader->m_AmbientLight = glm::vec3(0.1f, 0.1f, 0.1f);
            m_G_Shader->m_SpecularIntensity = 2.f;
            m_G_Shader->m_SpecularPower = 32.f;
            m_G_Shader->m_DispMapScale = dispMapScale;
            m_G_Shader->m_DispMapOffset = -dispMapBias + dispMapBias * dispMapOffset;

        // Point light ---------------------------------------
            // m_G_Shader->SetVec3("pointLight.base.color", glm::vec3(1, 1, 1));
            // m_G_Shader->SetFloat("pointLight.base.intensity", 100.f);
            // m_G_Shader->SetFloat("pointLight.attenuation.constant", 0.f);
            // m_G_Shader->SetFloat("pointLight.attenuation.linear", 0.f);
            // m_G_Shader->SetFloat("pointLight.attenuation.exponent", 1.f);
            // m_G_Shader->SetVec3("pointLight.position", glm::vec3(0, 10, 0));
        //

            scene.view<DirectionalLightComponent>().each([&](DirectionalLightComponent& dir)
            {
                glm::vec3 rot = dir.m_Transform.GetRotation().GetForward();

                glUniform3f(dir.m_Uniforms[BaseLight::BASE_COLOR], dir.m_Base.m_Color.x, dir.m_Base.m_Color.y, dir.m_Base.m_Color.z);
                glUniform1f(dir.m_Uniforms[BaseLight::INTENSITY], dir.m_Base.m_Intensity);
                glUniform3f(dir.m_Uniforms[DirectionalLightComponent::ROTATION], rot.x, rot.y, rot.z);
            });

            glm::mat4 lightMatrix = m_SpotLight->GetShadowInfo()->m_LightMatrix;

            m_G_Shader->UpdateCustom(*m_Camera);

            scene.view<RenderableComponent, ShaderComponent>().each([&](RenderableComponent& model, ShaderComponent& shader)
            {
                shader.m_BaseColor = glm::vec3(1, 1, 1);

                Camera& camera = *m_Camera;
                glm::mat4 modelM = model.m_Transform.GetModel();
                lightMatrix = lightMatrix * modelM;

                glm::mat4 projection = camera.GetProjection();
                Transform& camTrans = camera.GetTransform();
                // glm::mat4 view = m_Camera->GetView();

                const glm::mat4& view = glm::lookAt(
                    camTrans.GetPosition(),
                    camTrans.GetPosition() + camTrans.GetRotation().GetForward(),
                    glm::normalize(camTrans.GetRotation().GetUp())
                );

                glUniformMatrix4fv(shader.m_Uniforms[ShaderComponent::MODEL_U], 1, GL_FALSE, &modelM[0][0]);
                glUniformMatrix4fv(shader.m_Uniforms[ShaderComponent::PROJECTION_U], 1, GL_FALSE, &projection[0][0]);
                glUniformMatrix4fv(shader.m_Uniforms[ShaderComponent::VIEW_U], 1, GL_FALSE, &view[0][0]);

                // glm::vec3 eyePosition = camTrans.GetPosition();
                // glm::vec3 ambient = shader.m_AmbientLight;
                // glm::vec3 base = shader.m_BaseColor;

                // glUniform3f(shader.m_Uniforms[ShaderComponent::EYE_POSITION], eyePosition.x, eyePosition.y, eyePosition.z);

                // glUniform3f(shader.m_Uniforms[ShaderComponent::AMBIENT_LIGHT], ambient.x, ambient.y, ambient.z);
                // glUniform3f(shader.m_Uniforms[ShaderComponent::BASE_COLOR], base.x, base.y, base.z);

                // glUniform1f(shader.m_Uniforms[ShaderComponent::SPECULAR_INTENSITY], shader.m_SpecularIntensity);
                // glUniform1f(shader.m_Uniforms[ShaderComponent::SPECULAR_POWER], shader.m_SpecularPower);

                // glUniform1f(shader.m_Uniforms[ShaderComponent::DISP_MAP_SCALE], shader.m_DispMapScale);
                // glUniform1f(shader.m_Uniforms[ShaderComponent::DISP_MAP_OFFSET], shader.m_DispMapOffset);

                scene.view<DirectionalLightComponent>().each([&](DirectionalLightComponent& dir)
                {
                    glUniformMatrix4fv(shader.m_Uniforms[ShaderComponent::LIGHT_MATRIX_U], 1, GL_FALSE, &lightMatrix[0][0]);

                    lightMatrix = dir.m_LightMatrix * modelM;

                    glUniformMatrix4fv(shader.m_Uniforms[ShaderComponent::LIGHT_MATRIX2_U], 1, GL_FALSE, &lightMatrix[0][0]);
                });

                glBindVertexArray(model.m_VAO);
                glDrawElements(GL_TRIANGLES, model.m_DrawCount, GL_UNSIGNED_INT, 0);
            });

        // Render quad
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // m_QuadShader->Bind();

        // m_PositionTexture->Bind(GL_TEXTURE0);
        // m_NormalTexture->Bind(GL_TEXTURE1);
        // m_AlbedoTexture->Bind(GL_TEXTURE2);

        // m_QuadMesh->Draw();
    }

    void Renderer::PostRender()
    {

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
                // m_Scene->GetCurrentScene().view<CameraComponent>().each([&](CameraComponent& camera)
                // {
                //     Transform& camTrans = camera.m_Transform;

                //     camTrans.SetRotation(glm::normalize(glm::angleAxis(deltaPos.x * -0.001f, glm::vec3(0, 1, 0)) * camTrans.GetRotation()));
                //     camTrans.SetRotation(glm::normalize(glm::angleAxis(deltaPos.y * 0.001f, camTrans.GetRotation().GetRight()) * camTrans.GetRotation()));
                // });

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

    void* Renderer::GetFinalTexture()
    {
    // TODO: Here might be problem
        return (void*)m_AlbedoTexture->GetTextureID();
    }

    void Renderer::operator()(const ViewportSizeChangedEvent& event)
    {
        // m_Scene->GetCurrentScene().view<CameraComponent>().each([&](CameraComponent& camera)
        // {
        //     camera.m_Projection = glm::perspectiveFov(camera.m_FOV, (float)event.m_X, (float)event.m_Y, camera.m_Near, camera.m_Far);
        // });

        m_Camera->SetProjection(glm::perspectiveFov(m_Camera->m_Fov, (float)event.m_X, (float)event.m_Y, m_Camera->m_Near, m_Camera->m_Far));
    }

}}}