#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World04::Initialize()
    {
        auto material = GET_RESOURCE(Material, "materials/multi.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/Buddha.obj", glm::vec3{ 0 }, glm::vec3{ -90, 0 , 0 });

        for (int i = 0; i < 3; i++) {
            m_lights[i].type = light_t::lightType::Point;
            m_lights[i].position = glm::vec3{ randomf(-5,5),randomf(1,8),randomf(-5,5) };
            m_lights[i].direction = glm::vec3{ 0,-1,0 };
            m_lights[i].color = glm::vec3{ randomf(0.0f,1.0f), randomf(0.0f,1.0f), randomf(0.0f,1.0f) };
            m_lights[i].intensity = 1;
            m_lights[i].range = 10;
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

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0], 0.1f);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
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

        ImGui::ColorEdit3("Ambient", &lightAmbient[0]);
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
        auto material = m_model->GetMaterial();
        material->ProcessGui();
        material->Bind();
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        for (int i = 0; i < 3; i++) {
            std::string name = "lights[" + std::to_string(i) + "]";

            material->GetProgram()->SetUniform(name + ".type", m_lights[i].type);
            material->GetProgram()->SetUniform(name + ".innerAngle", glm::radians(m_lights[i].innerAngle));
            material->GetProgram()->SetUniform(name + ".outerAngle", glm::radians(m_lights[i].outerAngle));
            material->GetProgram()->SetUniform(name + ".position", m_lights[i].position);
            material->GetProgram()->SetUniform(name + ".direction", glm::normalize(m_lights[i].direction));
            material->GetProgram()->SetUniform(name + ".intensity", m_lights[i].intensity);
            material->GetProgram()->SetUniform("ambientColor", lightAmbient);
            material->GetProgram()->SetUniform(name + ".range", m_lights[i].range);
            material->GetProgram()->SetUniform(name + ".diffuseColor", m_lights[i].color);
        }

        // view
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 2, 4 }, glm::vec3{0,0,0}, glm::vec3{ 0, 1, 0 });
        material->GetProgram()->SetUniform("view", view);

        // projection
        float aspectration = ((float)ENGINE.GetSystem<Renderer>()->GetWidth() / ENGINE.GetSystem<Renderer>()->GetHeight());
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), aspectration, 0.01f, 1000.0f);
        material->GetProgram()->SetUniform("projection", projection);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_model->Draw(GL_TRIANGLES);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
