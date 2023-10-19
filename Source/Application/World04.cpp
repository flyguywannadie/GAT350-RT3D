#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World04::Initialize()
    {
        auto material = GET_RESOURCE(Material, "materials/grid.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/Buddha.obj", glm::vec3{ 0 }, glm::vec3{ -90, 0 , 0 });

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
        ImGui::DragFloat3("Position", &lightPosition[0], 0.1f);
        ImGui::ColorEdit3("Ambient", &lightAmbient[0]);
        ImGui::ColorEdit3("Diffuse", &lightDiffuse[0]);
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
        material->GetProgram()->SetUniform("light.position", lightPosition);
        material->GetProgram()->SetUniform("ambientColor", lightAmbient);
        material->GetProgram()->SetUniform("light.diffuseColor", lightDiffuse);

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
