#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtx/color_space.hpp>

namespace nc
{
    bool World06::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene_framebuffer.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(512, 512);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }

        return true;
    }

    void World06::Shutdown()
    {
    }

    void World06::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        // set post process gui
        ImGui::Begin("Post-Process");
        ImGui::SliderFloat("Intensity", &m_blend, 0.0f, 1.0f);

        bool effect = (params & INVERT_MASK);
        if (ImGui::Checkbox("Invert", &effect)) {
            if (effect) params |= INVERT_MASK;
            else params &= ~INVERT_MASK;
        }
        effect = (params & GRAYSCALE_MASK);
        if (ImGui::Checkbox("Grayscale", &effect)) {
            if (effect) params |= GRAYSCALE_MASK;
            else params &= ~GRAYSCALE_MASK;
        }
        effect = (params & COLOR_TINT_MASK);
        if (ImGui::Checkbox("Color Tint", &effect)) {
            if (effect) params |= COLOR_TINT_MASK;
            else params &= ~COLOR_TINT_MASK;
        }
        if (effect) {
            ImGui::ColorEdit3("Tint", glm::value_ptr(tint_color), 0.1f);
        }
        effect = (params & SCANLINE_MASK);
        if (ImGui::Checkbox("Scanline", &effect)) {
            if (effect) params |= SCANLINE_MASK;
            else params &= ~SCANLINE_MASK;
        }
        if (effect) {
            ImGui::DragInt("Scan Size", &scanlineSize, 1, 1, 200);
        }
        effect = (params & GRAIN_MASK);
        if (ImGui::Checkbox("Grain", &effect)) {
            if (effect) params |= GRAIN_MASK;
            else params &= ~GRAIN_MASK;
        }
        effect = (params & CSGOKNIFE_MASK);
        if (ImGui::Checkbox("CSGO Knife", &effect)) {
            if (effect) params |= CSGOKNIFE_MASK;
            else params &= ~CSGOKNIFE_MASK;
        }

        ImGui::End();

        m_time += dt;

        // set post process shader
        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
        if (program) {
            program->Use();
            program->SetUniform("intensity", m_blend);
            program->SetUniform("params", params);
            program->SetUniform("tint", tint_color);
            program->SetUniform("scanlineSize", scanlineSize);
            program->SetUniform("time", m_time);
        }

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        m_scene->GetActorByName("postprocess")->active = false;

        // ***PASS 1*** //
        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame(glm::vec3{ 0, 0, 0 });
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        m_scene->GetActorByName("postprocess")->active = true;
        // ***PASS 2*** //
        renderer.ResetViewport();
        renderer.BeginFrame();
        //m_scene->Draw(renderer);
        m_scene->GetActorByName("postprocess")->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
