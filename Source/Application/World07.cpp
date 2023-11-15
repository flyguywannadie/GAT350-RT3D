#include "World07.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtx/color_space.hpp>

namespace nc
{
    bool World07::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene_shadow.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateDepthTexture(512, 512);
        ADD_RESOURCE("depth_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateDepthbuffer(texture);
        ADD_RESOURCE("depth_buffer", framebuffer);

        // set depth texture to debug sprite
        auto material = GET_RESOURCE(Material, "materials/sprite.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }

        return true;
    }

    void World07::Shutdown()
    {
    }

    void World07::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World07::Draw(Renderer& renderer)
    {

        // ***PASS 1*** //
        auto framebuffer = GET_RESOURCE(Framebuffer, "depth_buffer");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.ClearDepth();
        auto program = GET_RESOURCE(Program, "shaders/shadow_depth.prog");
        program->Use();

        auto lights = m_scene->GetComponents<LightComponent>();
        for (auto light : lights) {
            if (light->castShadow) {
                glm::mat4 shadowMatrix = light->GetShadowMatrix();
                program->SetUniform("shadowVP", shadowMatrix);
            }
        }

        auto models = m_scene->GetComponents<ModelComponent>();
        for (auto model : models) {
            program->SetUniform("model", model->m_owner->transform.GetMatrix());
            model->model->Draw();
        }

        framebuffer->Unbind();
        // ***PASS 2*** //
        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);
        //m_scene->GetActorByName("postprocess")->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
