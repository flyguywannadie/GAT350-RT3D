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
            -0.25f, -0.25f, 0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
             0.25f, -0.25f, 1.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
             0.25f,  0.25f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
            -0.25f,  0.25f, 0.0f,   1.0f, 1.0f, 1.0f,  0.0f, 1.0f
        };

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 8 * sizeof(GLfloat));

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 0);

        // texture
        glEnableVertexAttribArray(2);
        glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat));
        glVertexAttribBinding(2, 0);

        return true;
    }

    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0]);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0]);
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
        m_program->SetUniform("offset", glm::vec2{ m_time, 0 });

        // tiling texture
        m_program->SetUniform("tiling", glm::vec2{ m_time, 1 });

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World03::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glBindVertexArray(m_vao);
        glDrawArrays(GL_QUADS, 0, 4);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
