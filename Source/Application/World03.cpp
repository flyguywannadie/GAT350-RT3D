#include "World03.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

#define INTERLEAVE

namespace nc
{
    bool World03::Initialize()
    {
        m_program = GET_RESOURCE(Program, "shaders/unlit_color.prog");
        m_program->Use();

#ifdef INTERLEAVE
        //vertex data  (position,   color)
        float vertexData[] = {
            -0.25f, -0.25f, 0.0f,   1.0f, 0.0f, 0.0f,
             0.25f, -0.25f, 0.0f,   0.0f, 1.0f, 0.0f,
             0.25f,  0.25f, 0.0f,   0.0f, 0.0f, 1.0f,
            -0.25f,  0.25f, 0.0f,   0.0f, 0.0f, 1.0f
        };

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 6 * sizeof(GLfloat));
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 0);

#else
        //vertex data
        float positionData[] = {
            -0.25f, -0.25f, 0.0f,
            0.25f, -0.25f, 0.0f,
            0.25f,  0.25f, 0.0f,
            -0.25f,  0.25f, 0.0f
        };

        float colorData[] =
        {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f
        };

        GLuint vbo[2];
        glGenBuffers(2, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(0, vbo[0], 0, 3 * sizeof(GLfloat));
        // color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(1, vbo[1], 0, 3 * sizeof(GLfloat));
#endif // INTERLEAVE

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
        ImGui::DragFloat3("Scale", &m_transform.scale[0]);
        ImGui::End();

        m_transform.rotation.z += (180 * dt);
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
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 4, 5 }, m_transform.position, glm::vec3{ 0, 1, 0 });
        m_program->SetUniform("view", view);

        // projection
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 1000.0f);
        m_program->SetUniform("projection", projection);

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