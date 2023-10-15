#include "World03.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World03::Initialize()
    {
        m_program = GET_RESOURCE(Program, "shaders/unlit_texture.prog");
        m_program->Use();

        m_texture = GET_RESOURCE(Texture, "textures/llama.jpg");
        m_texture->Bind();
        m_texture->SetActive(GL_TEXTURE0);

        //vertex data  (position,   color)
        float vertexData[] = {
            -0.25f, -0.25f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
             0.25f, -0.25f, 1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
             0.25f,  0.25f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
            -0.25f,  0.25f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f
        };

        m_vertexBuffer = GET_RESOURCE(VertexBuffer, "vb");
        m_vertexBuffer->CreateVertexBuffer(sizeof(vertexData), 4, vertexData);
        m_vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GLfloat), 0);                  // position 
        m_vertexBuffer->SetAttribute(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(float));  // color 
        m_vertexBuffer->SetAttribute(2, 2, 8 * sizeof(GLfloat), 6 * sizeof(float));  // texcoord

        return true;
    }

    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0], 0.1f);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
        ImGui::End();

        ImGui::Begin("UV");
        ImGui::DragFloat2("Offset", &uvOffset[0], 0.01f);
        ImGui::DragFloat2("Tiling", &uvTiling[0], 0.01f);
        ImGui::End();

        //m_transform.rotation.z += (180 * dt);
        m_time += dt;

        m_transform.position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? dt * m_speed : 0);
        m_transform.position.x += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt * m_speed : 0);
        m_transform.position.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? -dt * m_speed : 0);
        m_transform.position.y += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? dt * m_speed : 0);
        m_transform.position.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? -dt * m_speed : 0);
        m_transform.position.z += (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? dt * m_speed : 0);

        // model matrix
        //glm::mat4 position = glm::translate(glm::mat4{ 1 }, m_position);
        //glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, glm::radians(m_angle), glm::vec3{0,0,1});
        //glm::mat4 model = position * rotation;

        m_program->SetUniform("model", m_transform.GetMatrix());

        // view
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 1, 1 }, glm::vec3{0,0,0}, glm::vec3{ 0, 1, 0 });
        m_program->SetUniform("view", view);

        // projection
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 1000.0f);
        m_program->SetUniform("projection", projection);

        // offset texture
        m_program->SetUniform("offset", uvOffset);

        // tiling texture
        m_program->SetUniform("tiling", uvTiling);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World03::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        m_vertexBuffer->Draw(GL_QUADS);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
