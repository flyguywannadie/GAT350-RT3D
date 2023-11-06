#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtx/color_space.hpp>

namespace nc
{
    bool World04::Initialize()
    {
        m_material = GET_RESOURCE(Material, "materials/squirrel.mtrl");
        m_model = std::make_shared<Model>();
        m_model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });

        for (int i = 0; i < 3; i++) {
            m_lights[i].type = light_t::lightType::Point;
            m_lights[i].position = glm::vec3{ randomf(-5,5),randomf(1,8),randomf(-5,5) };
            m_lights[i].direction = glm::vec3{ 0,-1,0 };
            m_lights[i].color = glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
            m_lights[i].intensity = 1;
            m_lights[i].range = 16;
            m_lights[i].innerAngle = 10.0f;
            m_lights[i].outerAngle = 30.0f;
        }

        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();


        ImGui::Begin("Scene");
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(lightAmbient));
        ImGui::Separator();

        for (int i = 0; i < 3; i++)
        {
            std::string name = "light" + std::to_string(i);
            if (ImGui::Selectable(name.c_str(), m_selected == i)) m_selected = i;
        }
        ImGui::End();




        ImGui::Begin("Light");
        const char* types[] = {"Point", "Directional", "Spot"};
        ImGui::Combo("Type", (int*)(&m_lights[m_selected].type), types, 3);

        if (m_lights[m_selected].type != light_t::Directional)   ImGui::DragFloat3("Position", glm::value_ptr(m_lights[m_selected].position), 0.1f);
        if (m_lights[m_selected].type != light_t::Point)  ImGui::DragFloat3("Direction", glm::value_ptr(m_lights[m_selected].direction), 0.1f);
        if (m_lights[m_selected].type == light_t::Spot) {
            ImGui::DragFloat("Inner Angle", &m_lights[m_selected].innerAngle, 1, 0, m_lights[m_selected].outerAngle);
            ImGui::DragFloat("Outer Angle", &m_lights[m_selected].outerAngle, 1, m_lights[m_selected].innerAngle, 90);
        }

        ImGui::DragFloat("Intensity", &m_lights[m_selected].intensity, 0.1f, 0, 10);

        if (m_lights[m_selected].type != light_t::Directional)   ImGui::DragFloat("Range", &m_lights[m_selected].range, 0.1f, 0.1f, 100.0f);

        ImGui::ColorEdit3("Diffuse", glm::value_ptr(m_lights[m_selected].color));
        ImGui::End();

        //m_transform.rotation.z += (180 * dt);
        m_time += dt;

        //m_material->offset.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? -dt * m_speed : 0);
        //m_material->offset.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? dt * m_speed : 0);
        //m_material->offset.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? -dt * m_speed : 0);
        //m_material->offset.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? dt * m_speed : 0);

       // m_transform.rotation.x += dt * 100;
        //m_transform.rotation.y += dt * 100;
        //m_transform.rotation.z += dt * 100;

        // model matrix
        //glm::mat4 position = glm::translate(glm::mat4{ 1 }, m_position);
        //glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, glm::radians(m_angle), glm::vec3{0,0,1});
        //glm::mat4 model = position * rotation;

        // texture tiling and offset
        m_material->ProcessGui();
        m_material->Bind();
        m_material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        for (int i = 0; i < 3; i++) {
            std::string name = "lights[" + std::to_string(i) + "]";

            m_material->GetProgram()->SetUniform(name + ".type", m_lights[i].type);
            m_material->GetProgram()->SetUniform(name + ".innerAngle", glm::radians(m_lights[i].innerAngle));
            m_material->GetProgram()->SetUniform(name + ".outerAngle", glm::radians(m_lights[i].outerAngle));
            m_material->GetProgram()->SetUniform(name + ".position", m_lights[i].position);
            m_material->GetProgram()->SetUniform(name + ".direction", glm::normalize(m_lights[i].direction));
            m_material->GetProgram()->SetUniform(name + ".intensity", m_lights[i].intensity);
            m_material->GetProgram()->SetUniform("ambientColor", lightAmbient);
            m_material->GetProgram()->SetUniform(name + ".range", m_lights[i].range);
            m_material->GetProgram()->SetUniform(name + ".diffuseColor", m_lights[i].color);
        }

        // view
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 2, 4 }, glm::vec3{0,0,0}, glm::vec3{ 0, 1, 0 });
        m_material->GetProgram()->SetUniform("view", view);

        // projection
        float aspectration = ((float)ENGINE.GetSystem<Renderer>()->GetWidth() / ENGINE.GetSystem<Renderer>()->GetHeight());
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), aspectration, 0.01f, 1000.0f);
        m_material->GetProgram()->SetUniform("projection", projection);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        /*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_model->Draw(GL_TRIANGLES);*/
        m_material->Bind();
        m_model->Draw();

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
